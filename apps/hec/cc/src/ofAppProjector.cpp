#include "ofAppProjector.h"

#include "Uber.h"

//--------------------------------------------------------------
void ofAppProjector::setup() 
{
	ofAddListener( _settings->get_gui_parameters().parameterChangedE(), this, &ofAppProjector::listenerFunction);

	fbo.allocate(1024, 768);
    fbo.begin();
		ofClear(0,0,0,255);
    fbo.end();

	ofSetColor(255,255,255);
	ofSetBackgroundColor(0,0,0);

	//cout << ofGetWindowPositionX() << endl;
	//cout << ofGetWindowPositionY() << endl;

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
    lua.doScript(_settings->get_script(), true);

    // call the script's setup() function
    lua.scriptSetup();
	
}

void ofAppProjector::listenerFunction(ofAbstractParameter& e)
{
    if ( e.getName() == "script" )
    {
        reloadScript();
    }
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
void ofAppProjector::keyPressed(int key) 
{
    switch (key) 
	{

	case 'f':
		ofToggleFullscreen();
		return;

  	case ' ':
    case 'r':
        reloadScript();
        break;

    case 'c':
		isCalibration = !isCalibration;

    	if ( !isCalibration )
			reloadScript();
    	
        break;

    case OF_KEY_LEFT:
		if ( _settings->currentScript - 1 >= 0 )
			--_settings->currentScript;

        break;

    case OF_KEY_RIGHT:
        if ( ( _settings->currentScript + 1 ) < _settings->scripts.size() )
			++_settings->currentScript;

        break;

    default: 
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

	cout << "Loaded: " << _settings->get_script() << endl;

    lua.doScript(_settings->get_script(), true);
    lua.scriptSetup();
}