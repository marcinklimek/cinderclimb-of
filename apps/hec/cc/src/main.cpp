#include "ofMain.h"
#include "ofAppGui.h"
#include "ofAppProjector.h"

//========================================================================
int main()
{
    ofGLFWWindowSettings settings;
	settings.width = 1024;
	settings.height = 768;
    
    settings.resizable = true;
    shared_ptr<ofAppBaseWindow> projector_window = ofCreateWindow(settings);
    projector_window->setVerticalSync(true);

	settings.width = 1200;
	settings.height = 1000;
    settings.setPosition(ofVec2f(5, 20));
    settings.resizable = true;


    // share main's OpenGL resources with gui
    //settings.shareContextWith = projector_window;	
    shared_ptr<ofAppBaseWindow> gui_window = ofCreateWindow(settings);
    gui_window->setVerticalSync(true);

    shared_ptr<ofAppGui> gui_app(new ofAppGui);
    shared_ptr<ofAppProjector> projector_app(new ofAppProjector);


    shared_ptr<AnalysisThread> analysis = std::make_shared<AnalysisThread>(gui_app->_settings);
    analysis->setup();

    gui_app->analysis = analysis;
    projector_app->analysis = analysis;

    projector_app->gui = gui_app;

    ofRunApp(gui_window, gui_app);
    ofRunApp(projector_window, projector_app);
    
    ofSetLogLevel(OF_LOG_VERBOSE);

    ofRunMainLoop();
}
