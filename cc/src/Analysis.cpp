#include "Analysis.h"
#include "ofxCvColorImage.h"
#include "ofxCv/Utilities.h"
#include <imgproc.hpp>

AnalysisThread::AnalysisThread(ofSettings& settings) : _settings(settings)
{
    startThread();
}

AnalysisThread::~AnalysisThread()
{
    toAnalyze.close();
    analyzed.close();
    waitForThread(true);
}

void AnalysisThread::setup()
{
    colorImg.allocate(image_size_W, image_size_H);
    grayImage.allocate(image_size_W, image_size_H);

    grayMask.allocate(image_size_W, image_size_H);

    mog = cv::createBackgroundSubtractorMOG2(250, 16, false);
}

void AnalysisThread::analyze(ofPixels & pixels)
{
    toAnalyze.send(std::move(pixels));
}

bool AnalysisThread::getPixels(ofPixels & pixels)
{
    bool newFrame = false;

    // get last analized, if consumer-thread is too slow
    while (analyzed.tryReceive(pixels))
    {
        newFrame = true;
    }

    return newFrame;
}



void AnalysisThread::threadedFunction()
{
    ofPixels pixels;

    while (toAnalyze.receive(pixels))
    {
        updateFrame(pixels);

        //analyzed.send(std::move(pixels));
    }
}

void AnalysisThread::updateFrame(ofPixels& frame)
{
    colorImg.ofxCvImage::setFromPixels(frame);

    grayImage = colorImg;

    grayImage.blur(_settings.blur_amount + (_settings.blur_amount % 2) + 1);
    grayImage.brightnessContrast(_settings.brightness, _settings.contrast);

    // bkg removal
    cv::Mat img, fgimg;
    img = ofxCv::toCv(grayImage.getPixels());

    mog->apply(img, fgMaskMOG2);

    ofImage foregroundImg;
    ofxCv::toOf(fgMaskMOG2, foregroundImg);
    grayMask = foregroundImg.getPixels();

    //grayMask.blur(_settings.blur_amount + (_settings.blur_amount % 2) + 1);
    //grayMask.threshold(_settings.threshold);

    for (int i = 0; i < _settings.circle_size; i++)
    {
        grayMask.dilate(_settings.dillate_count);
        grayMask.erode(_settings.erode_count);
    }

    // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
    // also, find holes is set to true so we will get interior contours as well....
    contourFinder.findContours(grayMask, _settings.area_min, _settings.area_max, 15, false); // find holes
}


void AnalysisThread::draw()
{
    // draw the incoming, the grayscale, the bg and the thresholded difference
    ofSetHexColor(0xffffff);

    ofRectangle rect;

    rect.set(spacing, spacing, preview_W, preview_H);
    colorImg.draw(rect);

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

    // we could draw the whole contour finder
    contourFinder.draw(rect);

    for (int i = 0; i < contourFinder.blobs.size(); i++)
    {
        contourFinder.blobs[i].draw(rect.getX(), rect.getY());
    }

    // finally, a report:
    ofSetHexColor(0xffffff);
    stringstream reportStr;
    reportStr << "num blobs found " << contourFinder.nBlobs << ", fps: " << ofGetFrameRate();

    ofDrawBitmapString(reportStr.str(), rect.getX() + spacing, rect.getY() + spacing);
}