#include "Analysis.h"
#include "ofxCvColorImage.h"
#include "bgsubcnt/bgsubcnt.h"
#include "ofxCv/Utilities.h"

AnalysisThread::AnalysisThread(ofSettings& settings) : _settings(settings), _quit(false)
{
    startThread();
}

AnalysisThread::~AnalysisThread()
{
    toAnalyze.close();
    analyzed.close();
    stop();
    waitForThread(true);
}

void AnalysisThread::setup()
{
    //_inputFrame.setUseTexture(false);
    _inputFrame.allocate(image_size_W, image_size_H);

    //_imageProcessed.setUseTexture(false);
    _imageProcessed.allocate(image_size_W, image_size_H);
    
    //_mask.setUseTexture(false);
    _mask.allocate(image_size_W, image_size_H);

    //_background.setUseTexture(false);
    _background.allocate(image_size_W, image_size_H);

    
    _inputFrameDraw.allocate(image_size_W, image_size_H);

    _imageProcessedDraw.allocate(image_size_W, image_size_H);

    _maskDraw.allocate(image_size_W, image_size_H);

    _backgroundDraw.allocate(image_size_W, image_size_H);

    mog = cv::bgsubcnt::createBackgroundSubtractorCNT(3, true, 4, true);
    //mog = cv::createBackgroundSubtractorMOG2(100, 8, false);
}

void AnalysisThread::analyze(ofPixels & pixels)
{
    toAnalyze.send(std::move(pixels));
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
    //_inputFrame.blurGaussian(_settings.blur_amount.get() + (_settings.blur_amount % 2) + 1);

    _imageProcessed = _inputFrame;

    // bkg removal
    cv::Mat img, fgimg;

    img = ofxCv::toCv(_imageProcessed);

    //mog->apply(img, fgMaskMOG2, _settings.learingRate);
    
	//ofImage foregroundImg;
    //ofxCv::toOf(fgMaskMOG2, foregroundImg);
	//foregroundImg.update();

    //_mask = foregroundImg;
		
    _mask.blurGaussian(_settings.blur_amount + (_settings.blur_amount % 2) + 1);

    for(int i=0; i<_settings.erode_open_count.get(); i++)
        _mask.erode(); // _settings.erode_count.get()

    for (int i = 0; i<_settings.dillate_count.get(); i++)
        _mask.dilate();  //_settings.dillate_count.get()

    for (int i = 0; i<_settings.erode_close_count.get(); i++)
        _mask.erode(); // _settings.erode_count.get()

    _mask.threshold(_settings.threshold);

    // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
    // also, find holes is set to true so we will get interior contours as well....
    contourFinder.findContours(_mask, _settings.area_min, _settings.area_max, 50, false); // find holes

    ///
    mog->getBackgroundImage(fgimg);
    //ofxCv::toOf(fgimg, foregroundImg);
    //_background = foregroundImg.getPixels();
    ///

    std::lock_guard<std::mutex> lock(_drawUpdateMutex);

    _backgroundDraw = _background;
    _inputFrameDraw = _inputFrame;
    _imageProcessedDraw = _imageProcessed;
    _maskDraw = _mask;

    analyzed.send(contourFinder.blobs);
}


void AnalysisThread::draw() const
{
    ofSetHexColor(0xffffff);

    ofRectangle rect;

    rect.set(spacing, spacing, preview_W, preview_H);

    {
        std::lock_guard<std::mutex> lock(_drawUpdateMutex);
        _imageProcessedDraw.draw(rect);
    }

    rect.set(spacing, spacing + (preview_H + spacing) * 1, preview_W, preview_H);
    {
        std::lock_guard<std::mutex> lock(_drawUpdateMutex);
        _backgroundDraw.draw(rect);
    }

    rect.set(spacing, spacing + (preview_H + spacing) * 2, preview_W, preview_H);

    {
        std::lock_guard<std::mutex> lock(_drawUpdateMutex);
        _maskDraw.draw(rect);
    }

    // then draw the contours:
    rect.set(spacing + preview_W + spacing, spacing, image_size_W, image_size_H);

    ofFill();
    ofSetHexColor(0x333333);
    ofDrawRectangle(rect);
    ofSetHexColor(0xffffff);
    _inputFrameDraw.draw(rect);
}


void AnalysisThread::drawBlobs( vector<ofxCvBlob>& blobs) const
{
    ofRectangle rect;
    rect.set(spacing + preview_W + spacing, spacing, image_size_W, image_size_H);

    drawBlobs(rect, blobs);
}

void AnalysisThread::drawBlobs(ofRectangle& rect, vector<ofxCvBlob> blobs) const 
{
    float x = rect.x;
    float y = rect.y;
    float w = rect.width;
    float h = rect.height;


    float scalex = w / image_size_W;
    float scaley = h / image_size_H;;


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