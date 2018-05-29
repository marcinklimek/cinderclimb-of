#include "ofMain.h"
#include "ofAppGui.h"
#include "ofAppProjector.h"

ofGLFWWindowSettings settings;
std::shared_ptr<ofSettings> app_settings;

//========================================================================
int main()
{
	app_settings = std::make_shared<ofSettings>();

	settings.width = 1024;
	settings.height = 768;
    
    settings.resizable = false;
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
	gui_app->_settings = app_settings;

    shared_ptr<AnalysisThread> analysis = std::make_shared<AnalysisThread>(app_settings);
    analysis->setup();

	analysis->projector_screen_size = ofVec2f(projector_window->getScreenSize());

    gui_app->analysis = analysis;
    projector_app->analysis = analysis;

    projector_app->gui = gui_app;
	gui_app->projector_app = projector_app;

    ofRunApp(gui_window, gui_app);
    ofRunApp(projector_window, projector_app);
    
    ofSetLogLevel(OF_LOG_VERBOSE);

    ofRunMainLoop();
}
