#include "ofAppProjector.h"

#include "Uber.h"

//--------------------------------------------------------------
void ofAppProjector::setup() {

	//cout << ofGetWindowPositionX() << endl;
	//cout << ofGetWindowPositionY() << endl;

	ofSetWindowPosition(3600, 0);


    // scripts to run
    scripts.push_back("scripts/game_01.lua");
	scripts.push_back("scripts/test.lua");
    currentScript = 0;

    uberObject.analysis = analysis;

    // init the lua state
    lua.init();

    // listen to error events
    lua.addListener(this);
    
    // add Uber object
    luaopen_UberObject(lua);

    // run a script
    // true = change working directory to the script's parent dir
    // so lua will find scripts with relative paths via require
    // note: changing dir does *not* affect the OF data path
    lua.doScript(scripts[currentScript], true);

    // call the script's setup() function
    lua.scriptSetup();
}

//--------------------------------------------------------------
void ofAppProjector::update() {
    // call the script's update() function
	lua_gc(lua, LUA_GCCOLLECT, 0);
    lua.scriptUpdate();
}

//--------------------------------------------------------------
void ofAppProjector::draw() {

	ofPushMatrix();
    
	ofTranslate(0, 0, 0.0);
    ofScale(100.0, 100.0, 100.0);

	if (isCalibration)
		ofBackground(ofColor::dimGrey);
	else
		ofBackground(ofColor::black);

	ofEnableAlphaBlending();
    // call the script's draw() function
    lua.scriptDraw();
	
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(0, 0, 0.0);
    ofScale(1.0, 1.0, 1.0);
	lua.scriptDrawUI();
	ofPopMatrix();
}

void ofAppProjector::exit()
{
    // call the script's exit() function
    lua.scriptExit();

    // clear the lua state
    lua.clear();
}

//--------------------------------------------------------------
void ofAppProjector::keyPressed(int key) {
    switch (key) {

	case 'f':

		ofToggleFullscreen();
		return;
    case 'r':
        reloadScript();
        break;

    case 'c':
		isCalibration = !isCalibration;
        break;

    case OF_KEY_LEFT:
        prevScript();
        break;

    case OF_KEY_RIGHT:
        nextScript();
        break;

    case ' ':
        lua.doString("print(\"this is a lua string saying you hit the space bar!\")");
        break;
    }

    lua.scriptKeyPressed(key);
}

//--------------------------------------------------------------
void ofAppProjector::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofAppProjector::mouseMoved(int x, int y) {
    lua.scriptMouseMoved(x, y);
}

//--------------------------------------------------------------
void ofAppProjector::mouseDragged(int x, int y, int button) {
    lua.scriptMouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofAppProjector::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofAppProjector::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofAppProjector::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofAppProjector::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofAppProjector::windowResized(int w, int h) 
{
	analysis->projectorScreenSize = ofVec2f(w, h);
}

//--------------------------------------------------------------
void ofAppProjector::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofAppProjector::dragEvent(ofDragInfo dragInfo) {

}

//--------------------------------------------------------------
void ofAppProjector::errorReceived(string& msg) {
    ofLogNotice() << "got a script error: " << msg;
}

//--------------------------------------------------------------
void ofAppProjector::reloadScript() {
    
    // exit, reinit the lua state, and reload the current script
    lua.scriptExit();

    lua.init();
    // add Uber object
    luaopen_UberObject(lua);

    lua.doScript(scripts[currentScript], true);
    lua.scriptSetup();
}

void ofAppProjector::nextScript() {
    currentScript++;
    if (currentScript > scripts.size() - 1) {
        currentScript = 0;
    }
    reloadScript();
}

void ofAppProjector::prevScript() {
    if (currentScript == 0) {
        currentScript = scripts.size() - 1;
    }
    else {
        currentScript--;
    }
    reloadScript();
}

