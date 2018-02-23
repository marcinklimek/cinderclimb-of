#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main()
{
    ofGLFWWindowSettings settings;
    settings.width = 1024;
    settings.height = 768;
    settings.resizable = true;
    shared_ptr<ofAppBaseWindow> projector_window = ofCreateWindow(settings);
    projector_window->setVerticalSync(false);

    settings.width = 1200;
    settings.height = 1000;
    settings.setPosition(ofVec2f(5, 20));
    settings.resizable = true;
    
    // share main's OpenGL resources with gui
    settings.shareContextWith = projector_window;	
    shared_ptr<ofAppBaseWindow> gui_window = ofCreateWindow(settings);
    gui_window->setVerticalSync(false);

    shared_ptr<ofApp> mainApp(new ofApp);

    ofAddListener(gui_window->events().draw, mainApp.get(), &ofApp::drawGui);
    ofRunApp(gui_window, mainApp);
    ofRunMainLoop();
}
