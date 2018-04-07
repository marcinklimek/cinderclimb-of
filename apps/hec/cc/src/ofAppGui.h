#pragma once

#include "ofMain.h"
#include "Settings.h"
#include "Analysis.h"
#include "ofxGui/src/ofxPanel.h"

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
    
    
    //ofRecorder recorder;
    std::shared_ptr<AnalysisThread> analysis;

    ofxPanel gui;


    ofSoundStream soundStream;
    int sampleRate;
    int channels;


    vector<ofxCvBlob> currentBlobs;

    ofFbo recordFbo;
    ofPixels recordPixels;
    bool isCalibration;

    std::shared_ptr<ofSettings> _settings;
};
