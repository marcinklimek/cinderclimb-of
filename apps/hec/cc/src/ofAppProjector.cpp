#include "ofAppProjector.h"

#include "Uber.h"

//--------------------------------------------------------------
void ofAppProjector::setup() 
{
	fbo.allocate(1024, 768);
    fbo.begin();
		ofClear(0,0,0,255);
    fbo.end();

	ofSetColor(255,255,255);
	ofSetBackgroundColor(0,0,0);

	//cout << ofGetWindowPositionX() << endl;
	//cout << ofGetWindowPositionY() << endl;

    // scripts to run

	scripts.push_back("scripts/game_03-box.lua");
	scripts.push_back("scripts/game_03-position.lua");
	scripts.push_back("scripts/game_03-totoro.lua");
	
	scripts.push_back("scripts/game_03-trail.lua");
	scripts.push_back("scripts/game_01-coins.lua");
	scripts.push_back("scripts/ue_banner.lua");
    currentScript = 0;

    uber_object.analysis = analysis;

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
void ofAppProjector::update() 
{
    // call the script's update() function
	lua_gc(lua, LUA_GCCOLLECT, 0);
    lua.scriptUpdate();

	ofEnableAlphaBlending();
	fbo.begin();

	ofPushMatrix();
    
	ofTranslate(0, 0, 0.0);
    ofScale(100.0, 100.0, 100.0);

	lua.scriptDrawFBO();
	
	ofPopMatrix();
	fbo.end();

	analysis->set_fbo_texture( fbo.getTexture() );
}

//--------------------------------------------------------------
void ofAppProjector::draw() 
{
	ofPushMatrix();
    
	ofTranslate(0, 0, 0.0);
    ofScale(100.0, 100.0, 100.0);

	if (isCalibration)
		ofBackground(ofColor::floralWhite);
	
    // call the script's draw() function
    lua.scriptDraw();
	
	ofPopMatrix();
}

void ofAppProjector::draw_fbo()
{
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	fbo.draw(0, 0, 1024, 768);
	ofPopStyle();
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

    default: ;
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
	analysis->projector_screen_size = ofVec2f(w, h);
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
void ofAppProjector::reloadScript() 
{
    fbo.begin();
		ofClear(0,0,0,0);
    fbo.end();

    // exit, reinit the lua state, and reload the current script
    lua.scriptExit();

    lua.init();
    // add Uber object
    luaopen_UberObject(lua);

	cout << "Loaded: " << scripts[currentScript] << endl;

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

