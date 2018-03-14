#include "Analysis.h"
#include "ofMain.h"

#include "ofxCvColorImage.h"
#include "ofxCv/Utilities.h"
#include "ofxCv/Wrappers.h"

#include "bgsubcnt/bgsubcnt.h"
#include "convexHull/ofxConvexHull.h"

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
    _colorFrame.allocate(_settings.image_size_W, _settings.image_size_H);

    _inputFrameDraw.allocate(_settings.image_size_W, _settings.image_size_H);
    _imageProcessedDraw.allocate(_settings.image_size_W, _settings.image_size_H);

    //mog = cv::bgsubcnt::createBackgroundSubtractorCNT(3, true, 15, true);
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
        
        ofxCvColorImage frame;
        if (grabber.get(frame))
        {
            frame.resize(_settings.image_size_W, _settings.image_size_H);
            updateFrame(frame);
        }
    }
}


void AnalysisThread::updateFrame(ofxCvColorImage& frame)
{
    _inputFrame = frame;

    _imageProcessed = frame;
    _imageProcessed.blur(_settings.blur_amount * 2 + 1);

    for(auto i=0; i<_settings.erode_open_count; i++)
        _imageProcessed.erode();

    for (auto i = 0; i<_settings.dillate_count; i++)
        _imageProcessed.dilate();
    
    for (auto i = 0; i<_settings.erode_close_count; i++)
        _imageProcessed.erode();

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

    // kinect gives RGBA, in OF we nedd to have RGB
    auto& pix = grabber.kinect.getColorSource()->getPixels();

    ofPixels rgbPix(pix);
    rgbPix.setImageType(OF_IMAGE_COLOR);
    rgbPix.resize(_settings.image_size_W, _settings.image_size_H);
    _colorFrame.setFromPixels(rgbPix);

//	ofPixels grayPix(pix);
//	grayPix.setImageType(OF_IMAGE_GRAYSCALE);
//	grayPix.resize(_settings.image_size_W, _settings.image_size_H);

//	ofxCvGrayscaleImage input;
//	input.allocate(grayPix.getWidth(), grayPix.getHeight());
//	input.setFromPixels(grayPix);
//
//	if (!_imageCanny.bAllocated ||
//		 _imageCanny.getWidth()  != input.getWidth() ||
//		 _imageCanny.getHeight() != input.getHeight())
//	{
//		_imageCanny.allocate(input.getWidth(), input.getHeight());
//	}
//	cvCanny(input.getCvImage(), _imageCanny.getCvImage(), mouseX/5, mouseY/5, 3);
//	_imageCanny.flagImageChanged();

    {
        std::lock_guard<std::mutex> lock(_drawUpdateMutex);
        
        _inputFrameDraw = _inputFrame;
        _imageProcessedDraw = _imageProcessed;
        _imageCannyDraw = _imageCanny;
    }
}


void AnalysisThread::draw() 
{
    ofSetHexColor(0xffffff);
    std::lock_guard<std::mutex> lock(_drawUpdateMutex);

    _imageProcessedDraw.draw(spacing, spacing,							   preview_W, preview_H);
    _inputFrameDraw.draw(    spacing, spacing + (preview_H + spacing) * 1, preview_W, preview_H);
    _imageProcessedGray.draw(spacing, spacing + (preview_H + spacing) * 2, preview_W, preview_H);
    _imageCannyDraw.draw(spacing, spacing + (preview_H + spacing) * 3, preview_W, preview_H);


    _colorFrame.draw(spacing + preview_W + spacing, spacing, _settings.image_size_W / 2, _settings.image_size_H / 2);
    grabber.kinect.getBodySource()->drawProjected(spacing + preview_W + spacing, spacing, _settings.image_size_W / 2, _settings.image_size_H / 2, ofxKFW2::ProjectionCoordinates::ColorCamera);

    vector<ofxCvBlob> blobs;
    if (getBlobs(blobs))
        drawBlobs(blobs);
}

void AnalysisThread::drawBlobs( vector<ofxCvBlob>& blobs)
{
    ofRectangle rect;
    rect.set(spacing + preview_W + spacing, spacing, _settings.image_size_W / 2, _settings.image_size_H / 2);

    drawBlobs(rect, blobs);
}


void AnalysisThread::drawBlobs(ofRectangle& rect, vector<ofxCvBlob> blobs) 
{
    float x = rect.x;
    float y = rect.y;
    float w = rect.width;
    float h = rect.height;

    float scale_x = w / _settings.image_size_W;
    float scale_y = h / _settings.image_size_H ;

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
        ofFill();
        ofBeginShape();
        for (int j = 0; j<blobs[i].nPts; j++) 
        {
            ofVertex(blobs[i].pts[i].x, blobs[i].pts[i].y);
        }
        ofEndShape();

    }
    ofPopMatrix();
    ofPopStyle();
}