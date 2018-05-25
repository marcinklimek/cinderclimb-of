#include "Analysis.h"
#include "ofMain.h"

#include "ofxCvColorImage.h"
#include "ofxCv/Utilities.h"
#include "ofxCv/Wrappers.h"

#include "convexHull/ofxConvexHull.h"

AnalysisThread::AnalysisThread(const std::shared_ptr<ofSettings>& settings) : mouse_x(0), mouse_y(0), sensing_window(0,0, 1, 1), quit_(false),
                                                                              grabber_(settings), convex_hull_()
{
	cv_mem_storage_ = cvCreateMemStorage( 1000 );

	settings_ = settings;
	startThread();
}

AnalysisThread::~AnalysisThread()
{
    stop();

	//-- clean up.  
	if( cv_mem_storage_ != nullptr )  
		cvReleaseMemStorage( &cv_mem_storage_ );
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
    image_processed_gray_.allocate(settings_->image_size_W, settings_->image_size_H);
    color_frame_.allocate(settings_->image_size_W, settings_->image_size_H);

    input_frame_public_.allocate(settings_->image_size_W, settings_->image_size_H);
    image_processed_public_.allocate(settings_->image_size_W, settings_->image_size_H);
    image_processed_gray_public_.allocate(settings_->image_size_W, settings_->image_size_H);
    color_frame_public_.allocate(settings_->image_size_W, settings_->image_size_H);
}

bool AnalysisThread::update_public()
{
	std::lock_guard<std::mutex> lock(update_mutex_);

	joints_public_ = joints_;
	blobs_path_public_ = blobs_path_;

    input_frame_public_ = input_frame_;
    image_processed_public_ = image_processed_;
    image_processed_gray_public_ = image_processed_gray_;
    color_frame_public_ = color_frame_;

	return false;
}

void AnalysisThread::threadedFunction()
{
    while (!quit_)
    {
		grabber_.update();
        
		ofxCvColorImage frame;
		frame.allocate(1920, 1080);
		if (grabber_.get(frame))
		{
			frame.resize(settings_->image_size_W, settings_->image_size_H);
			frame.mirror(false, true);

			update_frame(frame);
			update_joints();

			update_public();
		}
	}
}


void AnalysisThread::update_frame(ofxCvColorImage& frame)
{
    if (frame.getWidth() == 0 || frame.getHeight() == 0)
        return;
    
    input_frame_ = frame;
    image_processed_ = frame;

    //_imageProcessed.blur(_settings->blur_amount * 2 + 1);

    //for(auto i=0; i<_settings->erode_open_count; i++)
    //    _imageProcessed.erode();

    //for (auto i = 0; i<_settings->dillate_count; i++)
    //    _imageProcessed.dilate();
    //
    //for (auto i = 0; i<_settings->erode_close_count; i++)
    //    _imageProcessed.erode();

    image_processed_gray_ = image_processed_;
    contour_finder_.findContours(image_processed_gray_, settings_->area_min, settings_->area_max, 10, true); // find holes
	
	blobs_path_.clear();
	for (auto& blob : contour_finder_.blobs)
	{
		std::vector<ofPoint> filtered;

		for(auto& point: blob.pts)
		{
			ofPoint v;
			v.x = point.x / settings_->image_size_W;
			v.y = point.y / settings_->image_size_H;

			v = ofxHomography::toScreenCoordinates(v, sensing_trans.homography);
			filtered.emplace_back(v);
		}
		ofPolyline poly( filtered );
		//poly.simplify();
		//poly = poly.getSmoothed(3);
		blobs_path_.emplace_back(poly);
	}

	 // for (auto& blob : blobs_)
	 // {
	 // 	vector<ofPoint> points_out;
	 // 	simplify_dp(blob.pts, points_out, settings_->tolerance/1000.0f);
  
	 // 	blob.pts = points_out;
	 // 	blob.nPts = points_out.size();
  
	 // 	ofPolyline path(blob.pts);
	 // }

    //if (_settings->useConvexHull)
    //{
    //    for (auto i = 0; i < contourFinder.blobs.size(); i++)
    //    {
    //        contourFinder.blobs[i].pts = convexHull.getConvexHull(contourFinder.blobs[i].pts);
    //        contourFinder.blobs[i].nPts = contourFinder.blobs[i].pts.size();
    //    }
    //}

    // kinect gives RGBA, in OF we nedd to have RGB
    auto& pix = grabber_.kinect.getColorSource()->getPixels();

    ofPixels rgbPix(pix);
    rgbPix.setImageType(OF_IMAGE_COLOR);
    rgbPix.resize(settings_->image_size_W, settings_->image_size_H);
    color_frame_.setFromPixels(rgbPix);
	color_frame_.mirror(false, true);
}


void AnalysisThread::draw() 
{
	std::lock_guard<std::mutex> lock(update_mutex_);

    ofSetHexColor(0xffffff);

    image_processed_public_.draw(     spacing,                       spacing,					           preview_W,                   preview_H);
    input_frame_public_.draw(         spacing,                       spacing + (preview_H + spacing) * 1,  preview_W,                   preview_H);
    image_processed_gray_public_.draw(spacing,                       spacing + (preview_H + spacing) * 2,  preview_W,	                preview_H);
    color_frame_public_.draw(         spacing + preview_W + spacing, spacing,                              settings_->image_size_W, settings_->image_size_H);

	float w =  settings_->image_size_W;
	float h =  settings_->image_size_H;

	ofSetColor(0, 0, 0xcc, 0x80);
	ofDrawRectangle(spacing + preview_W + spacing + sensing_window.x * w, spacing + sensing_window.y * h,
		            sensing_window.width * w, spacing + sensing_window.height * h);

	draw_blobs(blobs_path_public_);
	draw_joints(joints_public_);
}

void AnalysisThread::draw_blobs( vector<ofPolyline>& blobs) const
{
    ofRectangle rect;
    rect.set(spacing + preview_W + spacing, spacing, settings_->image_size_W, settings_->image_size_H);

    float w = rect.width;
    float h = rect.height;

	const auto x = spacing + preview_W + spacing + sensing_window.x * w;
	const auto y = spacing + sensing_window.y * h;

	const float scale_x = w * sensing_window.width;//settings_->image_size_W;
	const float scale_y = h * sensing_window.height;//settings_->image_size_H ;

    ofPushStyle();
    // ---------------------------- draw the bounding rectangle
    ofSetHexColor(0xDD00CC);
    ofPushMatrix();
    ofTranslate(x, y, 0.0);
    ofScale(scale_x, scale_y, 0.0);
    
    ofNoFill();
    for (auto& blob : blobs)
    {
        ofDrawRectangle(blob.getBoundingBox().x, blob.getBoundingBox().y,
                        blob.getBoundingBox().width, blob.getBoundingBox().height);
    }

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
	const auto w = settings_->image_size_W;
	const auto h = settings_->image_size_H;

	const auto x = spacing + preview_W + spacing + sensing_window.x * w;
	const auto y = spacing + sensing_window.y * h;

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
}

void AnalysisThread::update_joints()
{
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
	
	//ofVec2f p = joints_public_[jointIdx];
	//p = p * ofVec2f(-1, 1) + ofVec2f(1, 0); // mirror
	return joints_public_[jointIdx];
}

std::vector<ofVec2f> AnalysisThread::get_joints(const int body_index) const
{
	if (get_num_joints() == 0)
		return std::vector<ofVec2f>{ {0, 0} };

	std::lock_guard<std::mutex> lock(update_mutex_);
	return joints_public_;
}


void AnalysisThread::simplify_dp( const vector<ofPoint>& contour_in, vector<ofPoint>& contour_out, const float tolerance ) const
{  
	//-- copy points.  

    const int numOfPoints = contour_in.size();

    CvPoint * cvpoints = new CvPoint[ numOfPoints ];  
	  
	for(auto i=0; i<numOfPoints; i++)  
	{
	    auto j = i % numOfPoints;  
		  
		cvpoints[ i ].x = contour_in[ j ].x;  
		cvpoints[ i ].y = contour_in[ j ].y;  
	}
	  
	//-- create contour.  
	  
	CvContour	contour;  
	CvSeqBlock	contour_block;  
	  
	cvMakeSeqHeaderForArray  
	(  
		CV_SEQ_POLYLINE,  
		sizeof(CvContour),  
		sizeof(CvPoint),  
		cvpoints,  
		numOfPoints,  
		reinterpret_cast<CvSeq*>(&contour),  
		&contour_block  
	);  

	  
	//-- simplify contour.  
	CvSeq *result = nullptr;  
	result = cvApproxPoly  
	(  
		&contour,  
		sizeof( CvContour ),  
		cv_mem_storage_,  
		CV_POLY_APPROX_DP,  
		cvContourPerimeter( &contour ) * tolerance,  
		0  
	);  
	  
	//-- contour out points.  
	contour_out.clear();  
	for(auto j=0; j<result->total; j++ )  
	{
		auto* pt = reinterpret_cast<CvPoint*>(cvGetSeqElem(result, j));  
		  
		contour_out.emplace_back(static_cast<float>(pt->x), static_cast<float>(pt->y));
	}  
	  
	delete[] cvpoints;  
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