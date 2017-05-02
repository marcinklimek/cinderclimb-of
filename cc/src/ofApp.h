#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxPanel.h"
#include "ofxVideoRecorder/src/ofxVideoRecorder.h"

constexpr int image_size_W = 640;
constexpr int image_size_H = 480;
constexpr int preview_W = 320;
constexpr int preview_H = 240;
constexpr int spacing = 10;


class ofxVideoRecorderOutputFileCompleteEventArgs;

class ofGrabber
{
    ofVideoGrabber vidGrabber;

public:

    ofGrabber();

    bool getPixels(ofPixels &frame);

    int getWidth() const;
    int getHeight() const;
};


class ofRecorder
{
    ofxVideoRecorder vidRecorder;

    bool bRecording;
    string fileName;
    string fileExt;
    int lastTimeCheck;

public:

    void start(int w, int h, float fps);
    void stop();
    void update(ofPixels& frame);
    ofRecorder();


};

class ofSettings
{
public:

    ofSettings();

    ofParameterGroup parameters;
    ofParameter<float> radius;
    ofParameter<ofColor> color;

    ofParameter<float> threshold;

    ofParameter<int> blur_amount;
    ofParameter<int> erosion_size;
    ofParameter<int> circle_size;

    ofParameter<float> area_min;
    ofParameter<float>  area_max;
    
    ofParameterGroup& get_gui_parameters();
};

class ofApp : public ofBaseApp
{
public:
    void setup() override;
    void setupGui();
    void exit() override;
    void updateRecorder();
    void updateFrame(ofPixels& frame);
    void update() override;
    void draw() override;
    void drawGui(ofEventArgs& args);
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
    
    ofGrabber grabber;
    ofRecorder recorder;

    ofxCvColorImage colorImg;
    ofxCvGrayscaleImage grayImage;
    ofxCvGrayscaleImage grayBg;
    ofxCvGrayscaleImage grayDiff;
    cv::BackgroundSubtractorMOG2 mog;

    ofxCvContourFinder contourFinder;

    bool bLearnBakground;

    ofxPanel gui;


    ofSoundStream soundStream;
    int sampleRate;
    int channels;



    ofFbo recordFbo;
    ofPixels recordPixels;

private:



    ofSettings _settings;
};
