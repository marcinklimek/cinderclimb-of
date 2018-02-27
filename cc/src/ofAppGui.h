#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxPanel.h"
#include "ofxVideoRecorder/src/ofxVideoRecorder.h"
#include <background_segm.hpp>

#include "Grabber.h"
#include "Recorder.h"
#include "Settings.h"
#include "Analysis.h"
#include "Calibrator.h"

class ofAppGui : public ofBaseApp
{
public:
    void setup() override;
    void setupGui();
    void exit() override;
    void updateRecorder();

    void update() override;
    void draw() override;
    void keyPressed(int key) override;
    void keyReleased(int key) override;
    void mouseMoved(int x, int y) override;
    void mouseDragged(int x, int y, int button) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void mouseEntered(int x, int y) override;
    void mouseExited(int x, int y) override;
    void windowResized(int w, int h) override;
    void dragEvent(ofDragInfo dragInfo) override;
    void gotMessage(ofMessage msg) override;
    
    
    ofRecorder recorder;
    cv::Ptr<AnalysisThread> analysis;

    ofxPanel gui;


    ofSoundStream soundStream;
    int sampleRate;
    int channels;


    vector<ofxCvBlob> currentBlobs;

    ofFbo recordFbo;
    ofPixels recordPixels;
	bool isCalibration;

private:



    ofSettings _settings;
};
