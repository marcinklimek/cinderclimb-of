#pragma once

#include "ofMain.h"
#include "Settings.h"
#include "Analysis.h"
#include "ofxGuiExtended/src/ofxGuiExtended.h"
#include "ofxXmlSettings/src/ofxXmlSettings.h"

class ofAppGui : public ofBaseApp
{
public:
    void setup() override;
    void setup_gui();
	void listenerFunction(ofAbstractParameter& e);
	void exit() override;

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

	void load_config();
	void save_config();
    
    //ofRecorder recorder;
    std::shared_ptr<AnalysisThread> analysis;

    ofxGui gui;

    vector<ofxCvBlob> currentBlobs;

    ofFbo recordFbo;
    ofPixels recordPixels;

    std::shared_ptr<ofSettings> _settings;
	shared_ptr<ofBaseApp> projector_app;

private:
	ofxXmlSettings xml_settings_;


};
