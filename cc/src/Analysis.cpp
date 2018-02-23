#include "Analysis.h"

#include "ofxCvColorImage.h"
#include "ofxCv/Utilities.h"
#include "ofxCv/Wrappers.h"

#include "bgsubcnt/bgsubcnt.h"
#include "convexHull/ofxConvexHull.h"

#define COLOR_WIDTH 1920
#define COLOR_HEIGHT 1080

AnalysisThread::AnalysisThread(ofSettings& settings) : _settings(settings), _quit(false), grabber(settings)
{

    startThread();
}

AnalysisThread::~AnalysisThread()
{
    analyzed.close();
    stop();
    waitForThread(true);
}

void AnalysisThread::setup()
{
    _inputFrame.allocate(_settings.image_size_W, _settings.image_size_H);
    _imageProcessed.allocate(_settings.image_size_W, _settings.image_size_H);
	_imageProcessedGray.allocate(_settings.image_size_W, _settings.image_size_H);
    _colorFrame.allocate(COLOR_WIDTH, COLOR_HEIGHT);

	_inputFrameDraw.allocate(_settings.image_size_W, _settings.image_size_H);
	_imageProcessedDraw.allocate(_settings.image_size_W, _settings.image_size_H);

	mog = cv::bgsubcnt::createBackgroundSubtractorCNT(3, true, 15, true);
    //mog = cv::createBackgroundSubtractorMOG2(50, 8, false);
}

bool AnalysisThread::getBlobs(vector<ofxCvBlob>& blobs)
{
    bool newFrame = false;
    
    // get last analized, if consumer-thread is too slow
    while (analyzed.tryReceive(blobs))
    {
        newFrame = true;
    }

    return newFrame;
}

void AnalysisThread::stop()
{
    _quit = true;
}

void AnalysisThread::threadedFunction()
{
    while (!_quit)
    {
		grabber.update();
		
        ofPixels frame;
        if (grabber.getPixels(frame))
        {
            updateFrame(frame);
        }
    }
}


void AnalysisThread::updateFrame(ofPixels& frame)
{
	_inputFrame.setFromPixels(frame);
	_imageProcessed.setFromPixels(frame);

	_imageProcessed.blur(_settings.blur_amount * 2 + 1, CV_MEDIAN);
	_imageProcessed.erode(_settings.erode_open_count);
	_imageProcessed.dilate(_settings.dillate_count);
	_imageProcessed.erode(_settings.erode_close_count);
	
	_imageProcessedGray = _imageProcessed;
    contourFinder.findContours(_imageProcessedGray, _settings.area_min, _settings.area_max, 50, false); // find holes

	if (_settings.useConvexHull)
	{
		for (auto i = 0; i < contourFinder.blobs.size(); i++)
		{
			contourFinder.blobs[i].pts = convexHull.getConvexHull(contourFinder.blobs[i].pts);
			contourFinder.blobs[i].nPts = contourFinder.blobs[i].pts.size();
		}
	}

    analyzed.send(contourFinder.blobs);

	{
		std::lock_guard<std::mutex> lock(_drawUpdateMutex);
		auto& pix = grabber.kinect.getColorSource()->getPixels();

		ofPixels rgbPix(pix);
		rgbPix.setImageType(OF_IMAGE_COLOR);
		_colorFrame.setFromPixels(rgbPix);

		_inputFrameDraw = _inputFrame;
		_imageProcessedDraw = _imageProcessed;
	}
}


void AnalysisThread::draw() const
{
    ofSetHexColor(0xffffff);
	std::lock_guard<std::mutex> lock(_drawUpdateMutex);

    ofRectangle rect;
    rect.set(spacing, spacing, preview_W, preview_H);
	_imageProcessedDraw.draw(rect);

    rect.set(spacing, spacing + (preview_H + spacing) * 1, preview_W, preview_H);
	_inputFrameDraw.draw(rect);

    rect.set(spacing, spacing + (preview_H + spacing) * 2, preview_W, preview_H);
	_colorFrame.draw(rect);

	rect.set(spacing + preview_W + spacing, spacing, COLOR_WIDTH/2, COLOR_HEIGHT/2);
	_colorFrame.draw(rect);

	grabber.kinect.getBodySource()->drawProjected(spacing + preview_W + spacing, spacing, COLOR_WIDTH / 2, COLOR_HEIGHT / 2, ofxKFW2::ProjectionCoordinates::ColorCamera);
}


void AnalysisThread::drawBlobs( vector<ofxCvBlob>& blobs) const
{
    ofRectangle rect;
    rect.set(spacing + preview_W + spacing, spacing, COLOR_WIDTH / 2, COLOR_HEIGHT / 2);

    drawBlobs(rect, blobs);
}

void AnalysisThread::drawBlobs(ofRectangle& rect, vector<ofxCvBlob> blobs) const 
{
    float x = rect.x;
    float y = rect.y;
    float w = rect.width;
    float h = rect.height;


    float scalex = w / (COLOR_WIDTH / 2);
    float scaley = h / (COLOR_HEIGHT / 2);;


    ofPushStyle();
    // ---------------------------- draw the bounding rectangle
    ofSetHexColor(0xDD00CC);
    ofPushMatrix();
    ofTranslate(x, y, 0.0);
    ofScale(scalex, scaley, 0.0);
    
    ofNoFill();
    for (int i = 0; i<(int)blobs.size(); i++) {
        ofDrawRectangle(blobs[i].boundingRect.x, blobs[i].boundingRect.y,
            blobs[i].boundingRect.width, blobs[i].boundingRect.height);
    }

    // ---------------------------- draw the blobs
    ofSetColor(0, 180, 180, 128);

    for (int i = 0; i<(int)blobs.size(); i++) {
        
        ofFill();
        ofBeginShape();
        for (int j = 0; j<blobs[i].nPts; j++) {
            ofVertex(blobs[i].pts[j].x, blobs[i].pts[j].y);
        }
        ofEndShape();

    }
    ofPopMatrix();
    ofPopStyle();
}