#include "ofAppGui.h"
#include "ofxCv/Utilities.h"
#include "Calibrator.h"
#include "memory"


//--------------------------------------------------------------
void ofAppGui::setup()
{
    isCalibration = true;
	

    setupGui();
}

//--------------------------------------------------------------
void ofAppGui::setupGui()
{
    gui.setup(_settings->get_gui_parameters());
    gui.setPosition(spacing + _settings->image_size_W, spacing + _settings->image_size_H);

    ofSetBackgroundAuto(false);
    ofSetBackgroundColor(0);
    ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofAppGui::exit()
{
    //recorder.stop();
}

void ofAppGui::updateRecorder()
{
    //recorder.update(colorImg.getPixels());
}


//--------------------------------------------------------------
void ofAppGui::update()
{
    ofBackground(100, 100, 100);

	
    analysis->mouse_x = mouseX;
    analysis->mouse_y = mouseY;

    if (isCalibration)
    {
        return;
    }
}

//--------------------------------------------------------------
void ofAppGui::draw()
{
	ofEnableAlphaBlending();

    analysis->draw();

    ofSetHexColor(0xffffff);
    stringstream reportStr;
    reportStr << "fps: " << ofGetFrameRate();

    ofRectangle rect;
    rect.set(spacing + preview_W + spacing, spacing, _settings->image_size_W, _settings->image_size_H);
    ofDrawBitmapString(reportStr.str(), rect.getX() + spacing, rect.getY() + spacing);


	gui.draw();
}

//--------------------------------------------------------------
void ofAppGui::keyPressed(int key)
{
    switch (key)
    {
        case 'v':
        {
            //recorder.start(grabber.getWidth(), grabber.getHeight(), 30.0f);

            break;
        }
        case 'c':
        {
            //recorder.stop();
            break;
        }
        case 'b':
        {
            isCalibration = !isCalibration;
            break;
        }
        default: break;
    }
}

//--------------------------------------------------------------
void ofAppGui::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofAppGui::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofAppGui::mouseDragged(int x, int y, int button)
{
	// right button
	if ( button == 2 )
	{
		float w =  _settings->image_size_W;
		float h =  _settings->image_size_H;

		ofRectangle rect = ofRectangle(spacing + preview_W + spacing, spacing, w, h);
		if ( rect.inside(x,y) )
		{
			const ofVec2f start_point(analysis->sensing_window.x * w,  analysis->sensing_window.y * h);

			analysis->sensing_window.width  = (x - start_point.x - rect.x) / w;
			analysis->sensing_window.height = (y - start_point.y - rect.y) / h;

			analysis->sensing_trans.distortedCorners[0] = analysis->sensing_window.getTopLeft();
			analysis->sensing_trans.distortedCorners[1] = analysis->sensing_window.getTopRight();
			analysis->sensing_trans.distortedCorners[2] = analysis->sensing_window.getBottomRight();
			analysis->sensing_trans.distortedCorners[3] = analysis->sensing_window.getBottomLeft();

			analysis->sensing_trans.update();

		}
	}
}

//--------------------------------------------------------------
void ofAppGui::mousePressed(int x, int y, int button)
{
	// right button
	if ( button == 2 )
	{
		ofRectangle rect = ofRectangle(spacing + preview_W + spacing, spacing, _settings->image_size_W, _settings->image_size_H);
		if ( rect.inside(x,y) )
		{
			float w =  _settings->image_size_W;
			float h =  _settings->image_size_H;

			analysis->sensing_window.x = (x - rect.x) / w; // przejscie do znormalizowanej przestrzeni
			analysis->sensing_window.y = (y - rect.y) / h;

			analysis->sensing_trans.distortedCorners[0] = analysis->sensing_window.getTopLeft();
			analysis->sensing_trans.distortedCorners[1] = analysis->sensing_window.getTopRight();
			analysis->sensing_trans.distortedCorners[2] = analysis->sensing_window.getBottomRight();
			analysis->sensing_trans.distortedCorners[3] = analysis->sensing_window.getBottomLeft();
			analysis->sensing_trans.update();
		}
	}
}

//--------------------------------------------------------------
void ofAppGui::mouseReleased(int x, int y, int button)
{
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
