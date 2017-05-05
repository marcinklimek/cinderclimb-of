#include "Analysis.h"
#include "ofxCvColorImage.h"
#include "ofxCv/Utilities.h"
#include <imgproc.hpp>

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
    colorImg.allocate(image_size_W, image_size_H);
    grayImage.allocate(image_size_W, image_size_H);

    grayMask.allocate(image_size_W, image_size_H);
    grayBackground.allocate(image_size_W, image_size_H);

    mog = cv::createBackgroundSubtractorMOG2(250, 16, false);
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
    colorImg.setFromPixels(frame);

    blurImg = colorImg;
    blurImg.blurGaussian(_settings.blur_amount + (_settings.blur_amount % 2) + 1);
    
    grayImage = blurImg;
    grayImage.brightnessContrast(_settings.brightness, _settings.contrast);

    // bkg removal
    cv::Mat img, fgimg;
    img = ofxCv::toCv(grayImage.getPixels());

    mog->apply(img, fgMaskMOG2, _settings.learingRate);

    ofImage foregroundImg;
    ofxCv::toOf(fgMaskMOG2, foregroundImg);
    grayMask = foregroundImg.getPixels();

    //grayMask.blurGaussian(_settings.blur_amount + (_settings.blur_amount % 2) + 1);
    grayMask.threshold(_settings.threshold);

    grayMask.erode(_settings.erode_count);
    grayMask.dilate(_settings.dillate_count);

    grayMask.dilate(_settings.dillate_count);
    grayMask.erode(_settings.erode_count);

    // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
    // also, find holes is set to true so we will get interior contours as well....
    contourFinder.findContours(grayMask, _settings.area_min, _settings.area_max, 15, false); // find holes

    ///
    mog->getBackgroundImage(fgimg);
    ofxCv::toOf(fgimg, foregroundImg);
    grayBackground = foregroundImg.getPixels();
    ///

    analyzed.send(contourFinder.blobs);
}


void AnalysisThread::draw() const
{
    // draw the incoming, the grayscale, the bg and the thresholded difference
    ofSetHexColor(0xffffff);

    ofRectangle rect;

    rect.set(spacing, spacing, preview_W, preview_H);
    grayBackground.draw(rect);

    rect.set(spacing, spacing + (preview_H + spacing) * 1, preview_W, preview_H);
    grayMask.draw(rect);

    rect.set(spacing, spacing + (preview_H + spacing) * 2, preview_W, preview_H);
    grayImage.draw(rect);

    // then draw the contours:
    rect.set(spacing + preview_W + spacing, spacing, image_size_W, image_size_H);

    ofFill();
    ofSetHexColor(0x333333);
    ofDrawRectangle(rect);
    ofSetHexColor(0xffffff);
    colorImg.draw(rect);

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