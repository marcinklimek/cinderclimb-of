#include <Windows.h>
#include <ppl.h>

#include "Analysis.h"
#include "ofMain.h"

#include "ofxCvColorImage.h"
#include "ofxCv/Utilities.h"
#include "ofxCv/Wrappers.h"

#include "convexHull/ofxConvexHull.h"
#include "bgsubcnt/bgsubcnt.h"
#include "ofxCv/ContourFinder.h"
#include "ofxCvFloatImage.h"


AnalysisThread::AnalysisThread(const std::shared_ptr<ofSettings>& settings) : mouse_x(0), mouse_y(0), quit_(false),
                                                                       settings_ (settings), grabber_(settings), fps_(0), backgroundModel_set(true)

{
    const concurrency::SchedulerPolicy policy(2, concurrency::MinConcurrency, 16, concurrency::MaxConcurrency, 32);
    Concurrency::Scheduler::SetDefaultSchedulerPolicy(policy);

    settings_->resetBackground.addListener(this, &AnalysisThread::resetChanged);
    averageTimer = settings_->resetBackgroundTime;

    fps_timer.setPeriod(1);
    backgroundModel_timer.setPeriod(settings_->resetBackgroundTime);

    startThread();
}

AnalysisThread::~AnalysisThread()
{
    stop();
}

void AnalysisThread::stop()
{
	quit_ = true;
	waitForThread();
}

void AnalysisThread::setup()
{
    input_frame_.allocate(settings_->image_size_W, settings_->image_size_H);
    image_processed_.allocate(settings_->image_size_W, settings_->image_size_H);
	backgroundModel_.allocate(settings_->image_size_W, settings_->image_size_H);
    backgroundModel_.set(UINT16_MAX);
    backgroundModel_public_.allocate(settings_->image_size_W, settings_->image_size_H);

    image_foreground_.allocate(settings_->image_size_W, settings_->image_size_H);
    color_frame_.allocate(settings_->image_size_W, settings_->image_size_H);

    input_frame_public_.allocate(settings_->image_size_W, settings_->image_size_H);
    image_processed_public_.allocate(settings_->image_size_W, settings_->image_size_H);
    image_foreground_public_.allocate(settings_->image_size_W, settings_->image_size_H);
    color_frame_public_.allocate(settings_->image_size_W, settings_->image_size_H);
}

bool AnalysisThread::update_public()
{
	std::lock_guard<std::mutex> lock(update_mutex_);

	joints_public_ = joints_;
	blobs_path_public_ = blobs_path_;

    input_frame_.contrastStretch();

    input_frame_public_ = input_frame_;
    image_processed_public_ = image_processed_;
    image_foreground_public_ = image_foreground_;
    backgroundModel_public_ = backgroundModel_;
    color_frame_public_ = color_frame_;

	return false;
}



void AnalysisThread::threadedFunction()
{
    while (!quit_)
    {
		grabber_.update();

		if (grabber_.get(input_frame_))
		{
			input_frame_.mirror(false, true);

			update_frame();
			update_joints();
			update_public();
            
		    frame_counter_++;
		}

        if (fps_timer.tick())
        {
            fps_ = frame_counter_;
            frame_counter_ = 0;
        }
	}
}

void AnalysisThread::average(ofxCvShortImage& image_input_a, ofxCvShortImage& image_input_b) const
{
    auto& input_a = image_input_a.getShortPixelsRef();
    const auto input_b = image_input_b.getShortPixelsRef();

    concurrency::parallel_for(0, DEPTH_SIZE, [&](int i)
    {
        const long v1 = input_a[i];
        const long v2 = input_b[i];

        const auto v = std::min(v1, v2);
        
        if (v > 0)
            input_a[i] = v;
    });
}

void AnalysisThread::inRange(ofxCvShortImage& image_input_a,  float min, float max) const
{
    auto& input_a = image_input_a.getShortPixelsRef();

    concurrency::parallel_for(0, DEPTH_SIZE, [&](int i)
    {
        const auto v = input_a[i];
        
        if (v > min && v < max)
            input_a[i] = v;
        else
            input_a[i] = 0;
    });
}

void AnalysisThread::foreground(ofxCvShortImage& image_input_a,  ofxCvShortImage& image_mask) const
{
    auto& input_a = image_input_a.getShortPixelsRef();
    const auto input_mask = image_mask.getShortPixelsRef();

    concurrency::parallel_for(0, DEPTH_SIZE, [&](int i) 
    {
        const auto v = input_a[i];
        const auto m = input_mask[i] - settings_->epsilon;
        
        if (v < m)
            input_a[i] = v;
        else
            input_a[i] = 0;
    });
}

void AnalysisThread::resetChanged(bool& state)
{
    if (state)
    {
        backgroundModel_set = true;
        backgroundModel_timer.setPeriod( settings_->resetBackgroundTime);
        backgroundModel_.set(UINT16_MAX);
    }
}


void AnalysisThread::update_frame()
{
    if (input_frame_.getWidth() == 0 || input_frame_.getHeight() == 0)
        return;

    image_processed_ = input_frame_;
    inRange( image_processed_, settings_->nearClipping, settings_->farClipping);
    image_processed_.contrastStretch(settings_->nearClipping, settings_->farClipping);

    if( settings_->blur_amount > 0 )
	 {
	 	if( settings_->blur_amount % 2 == 0 )
	 		settings_->blur_amount += 1;
 
	     image_processed_.blurGaussian(settings_->blur_amount);
	 }
 
    for(auto i=0; i<settings_->erode_open_count; i++)
        image_processed_.erode();

    for (auto i = 0; i<settings_->dillate_count; i++)
        image_processed_.dilate();

    for (auto i = 0; i<settings_->erode_close_count; i++)
        image_processed_.erode();


	if (backgroundModel_set)
	{
		average(backgroundModel_, image_processed_);
        
        if ( backgroundModel_timer.tick() )
        {
            backgroundModel_set = false;
            settings_->resetBackground = false;
        }

        return;
	}

    image_foreground_ = image_processed_;

    foreground(image_foreground_, backgroundModel_);
    

    if( settings_->blur_amount2 > 0 )
    {
	    if( settings_->blur_amount2 % 2 == 0 )
		    settings_->blur_amount2 += 1;
	
	    image_foreground_.blur(settings_->blur_amount2);
    }

    for(auto i=0; i<settings_->erode_open_count2; i++)
        image_foreground_.erode();

    for (auto i = 0; i<settings_->dillate_count2; i++)
        image_foreground_.dilate();
    
    for (auto i = 0; i<settings_->erode_close_count2; i++)
        image_foreground_.erode();

	//
    ofxCvGrayscaleImage temp;
    temp.allocate(image_foreground_.width, image_foreground_.height);
    temp = image_foreground_;

    contour_finder_.findContours(temp, settings_->area_min, settings_->area_max, 10, true); // find holes
	
	blobs_path_.clear();
	for (const auto& blob : contour_finder_.blobs)
	{
		std::vector<ofPoint> filtered;

		for(const auto& point: blob.pts)
		{
			ofPoint v;
			v.x = point.x / settings_->image_size_W;
			v.y = point.y / settings_->image_size_H;

			v = ofxHomography::toScreenCoordinates(v, sensing_window.get_homography());
			filtered.emplace_back(v);
		}
		ofPolyline poly( filtered );
		
		poly = poly.getSmoothed( settings_->smoothing );
		poly.simplify( settings_->tolerance );
		
		blobs_path_.emplace_back(poly);
	}

    color_frame_ = grabber_.colorIndex;
	color_frame_.mirror(false, true);
}


void AnalysisThread::draw() 
{
	std::lock_guard<std::mutex> lock(update_mutex_);

    ofSetHexColor(0xffffff);
	
    input_frame_public_.draw(       spacing,                               spacing,					            preview_W, preview_H);
    image_processed_public_.draw(	spacing + preview_W + spacing,         spacing                            , preview_W, preview_H);
    image_foreground_public_.draw(	spacing + (preview_W + spacing) * 2,   spacing                            , preview_W, preview_H);
    backgroundModel_public_.draw(	spacing + (preview_W + spacing) * 3,   spacing                            , preview_W, preview_H);

    color_frame_public_.draw( settings_->color_preview_pos );

    const float w =  settings_->image_size_W;
    const float h =  settings_->image_size_H;

    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate(settings_->color_preview_pos.getX(), settings_->color_preview_pos.getY());

	draw_blobs(blobs_path_public_);
	draw_joints(joints_public_);

    ofSetHexColor(0xffffff);
    stringstream report_str;
    report_str << "fps: " << fps_;
    ofDrawBitmapString(report_str.str(), spacing, spacing);


    sensing_window.draw(settings_->color_preview_pos.getWidth(), settings_->color_preview_pos.getHeight());

    ofPopStyle();
    ofPopMatrix();
}

void AnalysisThread::draw_blobs(vector<ofPolyline>& blobs) const
{
    ofPushStyle();
    ofPushMatrix();

    // ---------------------------- draw the blobs
    ofSetColor(0, 180, 180);

    for (auto& blob : blobs)
    {
		blob.draw();
    }

    ofPopMatrix();
    ofPopStyle();
}


void AnalysisThread::draw_joints( vector<ofVec2f>& joints) const
{
/*
	const auto w = settings_->image_size_W;
	const auto h = settings_->image_size_H;

	const auto x = sensing_window.x * w;
	const auto y = sensing_window.y * h;

    ofPushStyle();

    ofSetHexColor(0xDD00CC);
    ofPushMatrix();
    ofTranslate(x, y, 0.0);
    ofScale(1.0, 1.0, 1.0);
    
    ofFill();
    ofSetColor(250, 250, 250);
	
    for (const auto joint : joints) 
        ofDrawCircle(joint.x * w * sensing_window.width, joint.y * h * sensing_window.height, 2);

    ofPopMatrix();
    ofPopStyle();
*/
}

void AnalysisThread::update_joints()
{
/*
	const int body_index = 0;

	if (grabber_.numBodies() > 0)
	{
		joints_.clear();
		joints_ = grabber_.kinect.getBodySource()->getProjectedJointsVector(body_index, ofxKinectForWindows2::ProjectionCoordinates::ColorCamera);	

		std::vector<ofVec2f> filtered;

		for( const auto& joint: joints_)
		{
			if ( sensing_window.inside(joint) )
			{
				ofVec2f v = joint;
				v.x = 1.0f + v.x * -1.0f;
				v = ofxHomography::toScreenCoordinates(v, sensing_trans.homography);
				filtered.emplace_back(v);
			}
		}

		joints_ = filtered;
	}
*/
}

int AnalysisThread::get_num_bodies() const
{
	std::lock_guard<std::mutex> lock(update_mutex_);

	return grabber_.numBodies();
}

int AnalysisThread::get_num_joints() const
{
	std::lock_guard<std::mutex> lock(update_mutex_);

	return joints_public_.size();
}

ofVec2f AnalysisThread::get_joint(const int jointIdx) const
{
	if (get_num_joints() == 0)
		return {0, 0};

	std::lock_guard<std::mutex> lock(update_mutex_);
	
	return joints_public_[jointIdx];
}

std::vector<ofVec2f> AnalysisThread::get_joints(const int body_index) const
{
	if (get_num_joints() == 0)
		return std::vector<ofVec2f>{ {0, 0} };

	std::lock_guard<std::mutex> lock(update_mutex_);
	return joints_public_;
}

void AnalysisThread::set_fbo_texture(const ofTexture& of_texture)
{
	of_texture.readToPixels(fbo_pixels);
}

int AnalysisThread::get_num_blobs() const
{
	std::lock_guard<std::mutex> lock(update_mutex_);

	return blobs_path_public_.size();
}

std::vector<ofVec2f> AnalysisThread::get_blob(const size_t idx) const
{
	std::lock_guard<std::mutex> lock(update_mutex_);

	std::vector<ofVec2f> points_2d;
		
	for( auto point: blobs_path_public_[idx].getVertices())
		points_2d.emplace_back(point.x, point.y);

	return points_2d;
}

ofRectangle AnalysisThread::get_blob_min_max(const size_t idx) const
{
	std::lock_guard<std::mutex> lock(update_mutex_);

	return blobs_path_public_[idx].getBoundingBox();
}	

bool AnalysisThread::point_in_blobs(const ofPoint p, float distance)
{
	std::lock_guard<std::mutex> lock(update_mutex_);

	for(const auto& blob: blobs_path_public_)
	{
		if ( blob.inside(p.x, p.y) )
			return true;

		auto closest_point = blob.getClosestPoint( p );

		if (  fabsf(closest_point.distance(p)) <= distance )
		{
			return true;
		}
	}

	return false;
}