#include "ofMain.h"
#include "ofAppGui.h"
#include "ofAppProjector.h"



//========================================================================
int main()
{
	ofGLFWWindowSettings projector_settings;
	ofGLFWWindowSettings gui_settings;
	
	std::shared_ptr<ofSettings> app_settings;
	
	ofSetLogLevel(OF_LOG_VERBOSE);

	app_settings = std::make_shared<ofSettings>();

	projector_settings.setSize(1024, 768);
    projector_settings.resizable = false;
	
    shared_ptr<ofAppBaseWindow> projector_window = ofCreateWindow(projector_settings);
	projector_window->setVerticalSync(true);

	
	gui_settings.setSize(1600, 1000);
    gui_settings.setPosition(ofVec2f(0, 25));
    gui_settings.resizable = true;

    // share main's OpenGL resources with gui
    gui_settings.shareContextWith = projector_window;	
    shared_ptr<ofAppBaseWindow> gui_window = ofCreateWindow(gui_settings);
    gui_window->setVerticalSync(false);
	
    shared_ptr<ofAppGui> gui_app(new ofAppGui);
    shared_ptr<ofAppProjector> projector_app(new ofAppProjector);
	gui_app->_settings = app_settings;
	projector_app->_settings = app_settings;

    shared_ptr<AnalysisThread> analysis = std::make_shared<AnalysisThread>(app_settings);
    analysis->setup();

	analysis->projector_screen_size = ofVec2f(projector_window->getScreenSize());

    gui_app->analysis = analysis;
    projector_app->analysis = analysis;

    projector_app->gui = gui_app;
	gui_app->projector_app = projector_app;

    ofRunApp(gui_window, gui_app);
    ofRunApp(projector_window, projector_app);
    
    

    ofRunMainLoop();
}
