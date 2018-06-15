#include "ofAppGui.h"
#include "ofxCv/Utilities.h"
#include "Calibrator.h"
#include "memory"
#include "ofxXmlSettings/src/ofxXmlSettings.h"


//--------------------------------------------------------------
void ofAppGui::setup()
{
	load_config();
    setupGui();
}

//--------------------------------------------------------------
void ofAppGui::setupGui()
{
    gui.setup(_settings->get_gui_parameters());

	
    gui.setPosition(spacing + _settings->image_size_W + 5*spacing, spacing + _settings->image_size_H);

    ofSetBackgroundAuto(false);
    ofSetBackgroundColor(0);
    ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofAppGui::exit()
{
	save_config();
    //recorder.stop();
}



//--------------------------------------------------------------
void ofAppGui::update()
{
    ofBackground(100, 100, 100);
	
    analysis->mouse_x = mouseX;
    analysis->mouse_y = mouseY;
}

//--------------------------------------------------------------
void ofAppGui::draw()
{
	ofEnableAlphaBlending();

    analysis->draw();

    // ofSetHexColor(0xffffff);
    // stringstream reportStr;
    // reportStr << "fps: " << ofGetFrameRate();
    //
    // ofRectangle rect;
    // rect.set( _settings->color_preview_pos.getX(), _settings->color_preview_pos.getY(), _settings->image_size_W, _settings->image_size_H);
    // ofDrawBitmapString(reportStr.str(), rect.getX() + spacing, rect.getY() + spacing);

	gui.draw();
}

//--------------------------------------------------------------
void ofAppGui::keyPressed(int key)
{
     switch (key)
     {
         case 'f': return;
         case 'r': return;

     	default: 
     		break;
     }

	projector_app->keyPressed(key);
}

//--------------------------------------------------------------
void ofAppGui::keyReleased(int key)
{
     switch (key)
     {
         case 'f': return;
         case 'r': return;

     	default: 
     		break;
     }

	projector_app->keyReleased(key);
}

//--------------------------------------------------------------
void ofAppGui::mouseMoved(int x, int y)
{
    auto rect = _settings->color_preview_pos;
    rect.scaleFromCenter(1.1);

    if (rect.inside(x,y))
    {
        const float mx = (x - _settings->color_preview_pos.getX()) / _settings->color_preview_pos.getWidth();
        const float my = (y - _settings->color_preview_pos.getY()) / _settings->color_preview_pos.getHeight();

        analysis->sensing_window.mouseMoved(mx, my);

        if ( analysis->sensing_window.is_changed() )
        {
		    save_config();   
        }
    }
}

//--------------------------------------------------------------
void ofAppGui::mouseDragged(int x, int y, int button)
{
    auto rect = _settings->color_preview_pos;
    rect.scaleFromCenter(1.1);

    if (rect.inside(x,y))
    {
        const float mx = (x - _settings->color_preview_pos.getX()) / _settings->color_preview_pos.getWidth();
        const float my = (y - _settings->color_preview_pos.getY()) / _settings->color_preview_pos.getHeight();

        analysis->sensing_window.mouseDragged(mx, my, button);

        if ( analysis->sensing_window.is_changed() )
        {
		    save_config();   
        }
    }
}



//--------------------------------------------------------------
void ofAppGui::mousePressed(int x, int y, int button)
{
    auto rect = _settings->color_preview_pos;
    rect.scaleFromCenter(1.1);

    if (rect.inside(x,y))
    {
        const float mx = (x - _settings->color_preview_pos.getX()) / _settings->color_preview_pos.getWidth();
        const float my = (y - _settings->color_preview_pos.getY()) / _settings->color_preview_pos.getHeight();

        analysis->sensing_window.mousePressed(mx, my, button);

        if ( analysis->sensing_window.is_changed() )
        {
		    save_config();   
        }
    }
}

//--------------------------------------------------------------
void ofAppGui::mouseReleased(int x, int y, int button)
{
    auto rect = _settings->color_preview_pos;
    rect.scaleFromCenter(1.1);

    if (rect.inside(x,y))
    {
        const float mx = (x - _settings->color_preview_pos.getX()) / _settings->color_preview_pos.getWidth();
        const float my = (y - _settings->color_preview_pos.getY()) / _settings->color_preview_pos.getHeight();

        analysis->sensing_window.mouseReleased(mx, my, button);

        if ( analysis->sensing_window.is_changed() )
        {
		    save_config();   
        }
    }
}

//--------------------------------------------------------------
void ofAppGui::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofAppGui::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofAppGui::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofAppGui::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofAppGui::dragEvent(ofDragInfo dragInfo)
{
}

void ofAppGui::load_config()
{
	if ( xml_settings_.loadFile("cc-settings.xml") )
	{
  		analysis->sensing_window.load(xml_settings_);
	}
}

void ofAppGui::save_config()
{
    analysis->sensing_window.save(xml_settings_);

	xml_settings_.saveFile("cc-settings.xml");
}