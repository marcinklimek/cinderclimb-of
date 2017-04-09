#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxPanel.h"
#include "ofxVideoRecorder/src/ofxVideoRecorder.h"

class ofxVideoRecorderOutputFileCompleteEventArgs;

class ofApp : public ofBaseApp{

    public:
        void setup();
    void setupGui();
    void exit();
    void updateRecorder();
    void updateFrame(ofPixels& frame);
    void update();
        void draw();
    void drawGui(ofEventArgs& args);
    void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);


        ofVideoGrabber 		vidGrabber;

        ofxCvColorImage			colorImg;

        ofxCvGrayscaleImage 	grayImage;
        ofxCvGrayscaleImage 	grayBg;
        ofxCvGrayscaleImage 	grayDiff;

        ofxCvContourFinder 	contourFinder;

        int 				threshold;
        bool				bLearnBakground;


        ofParameterGroup parameters;
        ofParameter<float> radius;
        ofParameter<ofColor> color;
        ofxPanel gui;



        ofxVideoRecorder    vidRecorder;
        ofSoundStream       soundStream;
        bool bRecording;
        int sampleRate;
        int channels;
        string fileName;
        string fileExt;
    
        int lastTimeCheck;

        ofFbo recordFbo;
        ofPixels recordPixels;
        
};
