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

	analysis->update();
    analysis->mouseX = mouseX;
    analysis->mouseY = mouseY;

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
		float w =  _settings->image_size_W / 2;
		float h =  _settings->image_size_H / 2;

		ofRectangle rect = ofRectangle(spacing + preview_W + spacing, spacing, w, h);
		if ( rect.inside(x,y) )
		{
			ofVec2f startPoint(analysis->sensingWindow.x * w,  analysis->sensingWindow.y * h);

			analysis->sensingWindow.width  = (x - startPoint.x - rect.x) / w;
			analysis->sensingWindow.height = (y - startPoint.y - rect.y) / h;

			analysis->sensingTrans.distortedCorners[0] = analysis->sensingWindow.getTopLeft();
			analysis->sensingTrans.distortedCorners[1] = analysis->sensingWindow.getTopRight();
			analysis->sensingTrans.distortedCorners[2] = analysis->sensingWindow.getBottomRight();
			analysis->sensingTrans.distortedCorners[3] = analysis->sensingWindow.getBottomLeft();

			analysis->sensingTrans.update();

		}
	}
}

//--------------------------------------------------------------
void ofAppGui::mousePressed(int x, int y, int button)
{
	// right button
	if ( button == 2 )
	{
		ofRectangle rect = ofRectangle(spacing + preview_W + spacing, spacing, _settings->image_size_W / 2, _settings->image_size_H / 2);
		if ( rect.inside(x,y) )
		{
			float w =  _settings->image_size_W / 2;
			float h =  _settings->image_size_H / 2;

			analysis->sensingWindow.x = (x - rect.x) / w; // przejscie do znormalizowanej przestrzeni
			analysis->sensingWindow.y = (y - rect.y) / h;

			analysis->sensingTrans.distortedCorners[0] = analysis->sensingWindow.getTopLeft();
			analysis->sensingTrans.distortedCorners[1] = analysis->sensingWindow.getTopRight();
			analysis->sensingTrans.distortedCorners[2] = analysis->sensingWindow.getBottomRight();
			analysis->sensingTrans.distortedCorners[3] = analysis->sensingWindow.getBottomLeft();
			analysis->sensingTrans.update();
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
