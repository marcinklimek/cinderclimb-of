#pragma once
#include "ofxCvContourFinder.h"
#include <background_segm.hpp>

#include "Settings.h"
#include "ofxCvColorImage.h"

class AnalysisThread : public ofThread {
public:
    explicit AnalysisThread(ofSettings& settings);
    virtual ~AnalysisThread();
    void setup();
    void analyze(ofPixels & pixels);

    void draw();

    bool getPixels(ofPixels & pixels);

private:
    void threadedFunction() override;

    void updateFrame(ofPixels& frame);
    
    ofSettings& _settings;

    ofThreadChannel<ofPixels> toAnalyze;
    ofThreadChannel<ofPixels> analyzed;


    ofxCvColorImage colorImg;
    ofxCvGrayscaleImage grayImage;

    ofxCvGrayscaleImage grayMask;

    //bkg removal
    cv::Ptr<cv::BackgroundSubtractorMOG2> mog;
    cv::Mat fgMaskMOG2;


    ofxCvContourFinder contourFinder;
};
