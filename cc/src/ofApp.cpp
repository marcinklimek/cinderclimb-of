#include "ofApp.h"
#include "ofxCv/Utilities.h"



//--------------------------------------------------------------
void ofApp::setup()
{
    analysis = new AnalysisThread(_settings);
    analysis->setup();
    setupGui();

    
}

//--------------------------------------------------------------
void ofApp::setupGui()
{
    gui.setup(_settings.get_gui_parameters());
    gui.setPosition(spacing + preview_W + spacing, spacing);

    ofSetBackgroundAuto(false);
    ofSetBackgroundColor(0);
    ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::exit()
{
    recorder.stop();
}

void ofApp::updateRecorder()
{
    //recorder.update(colorImg.getPixels());
}



//--------------------------------------------------------------
void ofApp::update()
{
    ofBackground(100, 100, 100);
}

//--------------------------------------------------------------
void ofApp::draw()
{
    analysis->draw();

    vector<ofxCvBlob> blobs;
    if (analysis->getBlobs(blobs))
    {
        currentBlobs = blobs;
    }

    analysis->drawBlobs(currentBlobs);
    


    ofSetHexColor(0xffffff);
    stringstream reportStr;
    reportStr << "fps: " << ofGetFrameRate();

    ofRectangle rect;
    rect.set(spacing + preview_W + spacing, spacing, image_size_W, image_size_H);
    ofDrawBitmapString(reportStr.str(), rect.getX() + spacing, rect.getY() + spacing);
}

//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs& args)
{
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
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
        default: break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}
