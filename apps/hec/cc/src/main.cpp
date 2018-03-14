#include "ofMain.h"
#include "ofAppGui.h"
#include "ofAppProjector.h"

//========================================================================
int main()
{
    ofGLFWWindowSettings settings;
    settings.setSize(1024, 768);
    settings.resizable = true;
    shared_ptr<ofAppBaseWindow> projector_window = ofCreateWindow(settings);
    projector_window->setVerticalSync(true);

    settings.setSize(1200, 1000);
    settings.setPosition(ofVec2f(5, 20));
    settings.resizable = true;
    
    // share main's OpenGL resources with gui
    //settings.shareContextWith = projector_window;	
    shared_ptr<ofAppBaseWindow> gui_window = ofCreateWindow(settings);
    gui_window->setVerticalSync(true);

    shared_ptr<ofAppGui> gui_app(new ofAppGui);
    shared_ptr<ofAppProjector> projector_app(new ofAppProjector);
    projector_app->gui = gui_app;

    ofRunApp(gui_window, gui_app);
    ofRunApp(projector_window, projector_app);
    
    ofSetLogLevel(OF_LOG_VERBOSE);

    ofRunMainLoop();
}
