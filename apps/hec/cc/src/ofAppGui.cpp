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
    gui.setup(_settings.get_gui_parameters());
    gui.setPosition(spacing + _settings.image_size_W, spacing + _settings.image_size_H);

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
    analysis->draw();

    vector<ofxCvBlob> blobs;
    if (analysis->getBlobs(blobs))
    {
        currentBlobs = blobs;
    }

    analysis->drawBlobs(currentBlobs);
    
    for (const auto& blob : currentBlobs)
    {

    }


    ofSetHexColor(0xffffff);
    stringstream reportStr;
    reportStr << "fps: " << ofGetFrameRate();

    ofRectangle rect;
    rect.set(spacing + preview_W + spacing, spacing, _settings.image_size_W, _settings.image_size_H);
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
}

//--------------------------------------------------------------
void ofAppGui::mousePressed(int x, int y, int button)
{
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
