#pragma once

#include "ofxLua.h"
#include "ofMain.h"
#include "ofAppGui.h"

class ofAppProjector : public ofBaseApp, ofxLuaListener
{

public:
    void setup();
	void listenerFunction(ofAbstractParameter& e);
	void update();
    void draw();
	void draw_fbo();
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);

    // ofxLua error callback
    void errorReceived(string& msg);
    void gotMessage(ofMessage msg);

    // script control
    void reloadScript();

    std::shared_ptr<ofBaseApp> gui;
    std::shared_ptr<AnalysisThread> analysis;

    ofxLua lua;
   
	bool isCalibration = false;
	shared_ptr<ofSettings> _settings;


private:

	ofFbo fbo;
};
