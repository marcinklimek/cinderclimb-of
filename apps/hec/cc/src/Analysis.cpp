#include "Analysis.h"
#include "ofMain.h"

#include "ofxCvColorImage.h"
#include "ofxCv/Utilities.h"
#include "ofxCv/Wrappers.h"

#include "convexHull/ofxConvexHull.h"

AnalysisThread::AnalysisThread(std::shared_ptr<ofSettings> settings) : _quit(false), mouseX(0), mouseY(0), grabber(settings),
                                                                       convexHull(), sensingWindow(0,0, 1, 1)
{
	
	_settings = settings;
	startThread();
}

AnalysisThread::~AnalysisThread()
{
    stop();
}

void AnalysisThread::stop()
{
	_quit = true;
	waitForThread();
}

void AnalysisThread::setup()
{
    _inputFrame.allocate(_settings->image_size_W, _settings->image_size_H);
    _imageProcessed.allocate(_settings->image_size_W, _settings->image_size_H);
    _imageProcessedGray.allocate(_settings->image_size_W, _settings->image_size_H);
    _colorFrame.allocate(_settings->image_size_W, _settings->image_size_H);

    _inputFramePublic.allocate(_settings->image_size_W, _settings->image_size_H);
    _imageProcessedPublic.allocate(_settings->image_size_W, _settings->image_size_H);
    _imageProcessedGrayPublic.allocate(_settings->image_size_W, _settings->image_size_H);
    _colorFramePublic.allocate(_settings->image_size_W, _settings->image_size_H);
}

bool AnalysisThread::update()
{
	std::lock_guard<std::mutex> lock(_drawUpdateMutex);

	_jointsPublic = _joints;
	_blobsPublic = _blobs;
	
    _inputFramePublic = _inputFrame;
    _imageProcessedPublic = _imageProcessed;
    _imageProcessedGrayPublic = _imageProcessedGray;
    _colorFramePublic = _colorFrame;

	return false;
}


void AnalysisThread::threadedFunction()
{
    while (!_quit)
    {
		grabber.update();
        
		ofxCvColorImage frame;
		frame.allocate(1920, 1080);
		if (grabber.get(frame))
		{
			frame.resize(_settings->image_size_W, _settings->image_size_H);
			//frame.mirror(false, true);

			updateFrame(frame);
			updateJoints();
		}
	}
}

bool AnalysisThread::getBlobs(vector<ofxCvBlob>& blobs)
{
	blobs = contourFinder.blobs;

    return true;
}

void AnalysisThread::updateFrame(ofxCvColorImage& frame)
{
    if (frame.getWidth() == 0 || frame.getHeight() == 0)
        return;
    
    _inputFrame = frame;

    _imageProcessed = frame;
    //_imageProcessed.blur(_settings->blur_amount * 2 + 1);

    //for(auto i=0; i<_settings->erode_open_count; i++)
    //    _imageProcessed.erode();

    //for (auto i = 0; i<_settings->dillate_count; i++)
    //    _imageProcessed.dilate();
    //
    //for (auto i = 0; i<_settings->erode_close_count; i++)
    //    _imageProcessed.erode();

    _imageProcessedGray = _imageProcessed;
    //contourFinder.findContours(_imageProcessedGray, _settings->area_min, _settings->area_max, 50, false); // find holes

    //if (_settings->useConvexHull)
    //{
    //    for (auto i = 0; i < contourFinder.blobs.size(); i++)
    //    {
    //        contourFinder.blobs[i].pts = convexHull.getConvexHull(contourFinder.blobs[i].pts);
    //        contourFinder.blobs[i].nPts = contourFinder.blobs[i].pts.size();
    //    }
    //}

    // kinect gives RGBA, in OF we nedd to have RGB
    auto& pix = grabber.kinect.getColorSource()->getPixels();

    ofPixels rgbPix(pix);
    rgbPix.setImageType(OF_IMAGE_COLOR);
    rgbPix.resize(_settings->image_size_W, _settings->image_size_H);
    _colorFrame.setFromPixels(rgbPix);

	//{
	//	std::lock_guard<std::mutex> lock(_drawUpdateMutex);
	//	_blobs = contourFinder.blobs;
	//}
}



void AnalysisThread::draw() 
{
	std::lock_guard<std::mutex> lock(_drawUpdateMutex);

    ofSetHexColor(0xffffff);

    _imageProcessedPublic.draw(    spacing, spacing,					         preview_W, preview_H);
    _inputFramePublic.draw(        spacing, spacing + (preview_H + spacing) * 1, preview_W, preview_H);
    _imageProcessedGrayPublic.draw(spacing, spacing + (preview_H + spacing) * 2, preview_W, preview_H);
    _colorFramePublic.draw(        spacing + preview_W + spacing, spacing, _settings->image_size_W / 2, _settings->image_size_H / 2);

	float w =  _settings->image_size_W / 2;
	float h =  _settings->image_size_H / 2;

	ofSetColor(0, 0, 0xcc, 0x80);
	ofDrawRectangle(spacing + preview_W + spacing + sensingWindow.x * w, spacing + sensingWindow.y * h,
		            sensingWindow.width * w, spacing + sensingWindow.height * h);

	drawBlobs(_blobsPublic);
	drawJoints(_jointsPublic);
}

void AnalysisThread::drawBlobs( vector<ofxCvBlob>& blobs) const
{
    ofRectangle rect;
    rect.set(spacing + preview_W + spacing, spacing, _settings->image_size_W / 2, _settings->image_size_H / 2);

    float x = rect.x;
    float y = rect.y;
    float w = rect.width;
    float h = rect.height;

    float scale_x = w / _settings->image_size_W;
    float scale_y = h / _settings->image_size_H ;

    ofPushStyle();
    // ---------------------------- draw the bounding rectangle
    ofSetHexColor(0xDD00CC);
    ofPushMatrix();
    ofTranslate(x, y, 0.0);
    ofScale(scale_x, scale_y, 0.0);
    
    ofNoFill();
    for (auto i = 0; i<blobs.size(); i++) 
    {
        ofDrawRectangle(blobs[i].boundingRect.x, blobs[i].boundingRect.y,
            blobs[i].boundingRect.width, blobs[i].boundingRect.height);
    }

    // ---------------------------- draw the blobs
    ofSetColor(0, 180, 180);

    for (auto i = 0; i<blobs.size(); i++) 
    {
        ofBeginShape();
        for (int j = 0; j<blobs[i].pts.size(); j++) 
        {
            ofVertex(blobs[i].pts[j].x, blobs[i].pts[j].y);
        }
        ofEndShape();

    }
    ofPopMatrix();
    ofPopStyle();
}

void AnalysisThread::drawJoints( vector<ofVec2f>& joints)
{

    float w = _settings->image_size_W / 2;
    float h = _settings->image_size_H / 2;

    float x = spacing + preview_W + spacing + sensingWindow.x * w;
    float y = spacing + sensingWindow.y * h;

    ofPushStyle();

    ofSetHexColor(0xDD00CC);
    ofPushMatrix();
    ofTranslate(x, y, 0.0);
    ofScale(1.0, 1.0, 1.0);
    
    ofFill();
    ofSetColor(250, 250, 250);
	
    for (const auto joint : joints) 
        ofDrawCircle(joint.x * w * sensingWindow.width, joint.y * h * sensingWindow.height, 2);

    ofPopMatrix();
    ofPopStyle();
}

void AnalysisThread::updateJoints()
{
	int bodyIndex = 0;

	if (grabber.numBodies() > 0)
	{
		std::lock_guard<std::mutex> lock(_drawUpdateMutex);

		_joints.clear();
		_joints = grabber.kinect.getBodySource()->getProjectedJointsVector(bodyIndex, ofxKinectForWindows2::ProjectionCoordinates::ColorCamera);	

		std::vector<ofVec2f> filtered;

		for( const auto& joint: _joints)
		{
			if ( sensingWindow.inside(joint) )
			{
				ofVec2f v = joint;
				v = ofxHomography::toScreenCoordinates(v, sensingTrans.homography);
				filtered.emplace_back(v);
			}
		}

		_joints = filtered;
	}
}

int AnalysisThread::getNumJoints() const
{
	std::lock_guard<std::mutex> lock(_drawUpdateMutex);

	if (grabber.numBodies() == 0)
		return 0;

	return _jointsPublic.size();
}

ofVec2f AnalysisThread::getJoint(const int jointIdx) const
{
	if (getNumJoints() == 0)
		return {0, 0};

	std::lock_guard<std::mutex> lock(_drawUpdateMutex);
	
	ofVec2f p = _jointsPublic[jointIdx];
	p = p * ofVec2f(-1, 1) + ofVec2f(1, 0); // mirror
	return p * ofVec2f(10.24, 7.68);
}
