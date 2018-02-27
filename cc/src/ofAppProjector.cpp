#include "ofAppProjector.h"

//--------------------------------------------------------------
void ofAppProjector::setup(){
	ofBackground(255);
	ofSetCircleResolution(100);
}

//--------------------------------------------------------------
void ofAppProjector::update()
{
	ofBackground(10, 10, 10);
}

//--------------------------------------------------------------
void ofAppProjector::draw()
{
	ofSetHexColor(0xffffff);
	ofDrawCircle(ofGetWidth()*0.5,ofGetWidth()*0.5, 50);

	ofDrawBitmapString(ofGetFrameRate(), 20, 20);
}

//--------------------------------------------------------------
void ofAppProjector::keyPressed(int key){

}

//--------------------------------------------------------------
void ofAppProjector::keyReleased(int key){

}

//--------------------------------------------------------------
void ofAppProjector::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofAppProjector::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofAppProjector::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofAppProjector::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofAppProjector::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofAppProjector::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofAppProjector::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofAppProjector::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofAppProjector::dragEvent(ofDragInfo dragInfo){ 

}
