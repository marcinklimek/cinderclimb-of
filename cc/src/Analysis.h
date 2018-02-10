#pragma once
#include "ofxCvContourFinder.h"
#include <background_segm.hpp>

#include "Settings.h"
#include "ofxCvColorImage.h"
#include "Grabber.h"
#include "thread"

class AnalysisThread : public ofThread {
public:
    explicit AnalysisThread(ofSettings& settings);
    virtual ~AnalysisThread();
    void setup();
    void analyze(ofPixels & pixels);

    void draw() const;
    void drawBlobs(vector<ofxCvBlob>& blobs) const;
    void drawBlobs(ofRectangle& rect, vector<ofxCvBlob> blobs) const;
    bool getBlobs(vector<ofxCvBlob>& blobs);

    void stop();

private:
    void threadedFunction() override;

    void updateFrame(ofPixels& frame);
    
    ofSettings& _settings;

    ofGrabber grabber;

    ofThreadChannel<ofPixels> toAnalyze;
    ofThreadChannel<vector<ofxCvBlob>> analyzed;

    ofxCvColorImage _inputFrame;
	ofxCvColorImage _imageProcessed;
    ofxCvGrayscaleImage _mask;
	ofxCvColorImage _background;

	// just for drawing purposses
	ofxCvColorImage _inputFrameDraw;
	ofxCvColorImage _imageProcessedDraw;
	ofxCvGrayscaleImage _maskDraw;
	ofxCvColorImage _backgroundDraw;

    //bkg removal
    cv::Ptr<cv::BackgroundSubtractorMOG2> mog;
    cv::Mat fgMaskMOG2;


    ofxCvContourFinder contourFinder;
    bool _quit;

	mutable std::mutex _drawUpdateMutex;
};
