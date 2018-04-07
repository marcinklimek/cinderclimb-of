﻿#pragma once

#include "ofxCvContourFinder.h"
#include "Settings.h"
#include "ofxCvColorImage.h"
#include "Grabber.h"
#include "convexHull/ofxConvexHull.h"

class AnalysisThread : public ofThread {
public:

    AnalysisThread() = delete;
    AnalysisThread(const AnalysisThread&) = delete;
    AnalysisThread(AnalysisThread&&) = delete;
    AnalysisThread& operator=(const AnalysisThread&) = delete;
    AnalysisThread& operator=(AnalysisThread&&) = delete;

    explicit AnalysisThread(std::shared_ptr<ofSettings> settings);
    virtual ~AnalysisThread();

    void setup();

    void draw();
    void drawBlobs(vector<ofxCvBlob>& blobs);
    void drawBlobs(ofRectangle& rect, vector<ofxCvBlob> blobs);
    bool getBlobs(vector<ofxCvBlob>& blobs);

    void getBody();

    void stop();
    
    double mouseX;
    double mouseY;

private:
    void threadedFunction() override;

    void updateFrame(ofxCvColorImage& frame);
    
    std::shared_ptr<ofSettings> _settings;

    ofGrabber grabber;
    ofxConvexHull convexHull;

    ofThreadChannel<vector<ofxCvBlob>> analyzed;

    ofxCvColorImage _inputFrame;
    ofxCvColorImage _imageProcessed;
    ofxCvGrayscaleImage _imageProcessedGray;
    ofxCvColorImage _colorFrame;

    // just for drawing purposses
    ofxCvColorImage _inputFrameDraw;
    ofxCvColorImage _imageProcessedDraw;

    ofxCvContourFinder contourFinder;
    bool _quit;

    mutable std::mutex _drawUpdateMutex;
    
    ofxCvGrayscaleImage _imageCanny;
    ofxCvGrayscaleImage _imageCannyDraw;
};
