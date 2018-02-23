#pragma once

//#define USE_STANDARD_CAMERA

#include "ofPixels.h"
#include "ofVideoGrabber.h"
#include "Settings.h"
#include "ofxKinectForWindows2/src/ofxKinectForWindows2.h"

class ofGrabber
{
	ofSettings& _settings;

	ICoordinateMapper* coordinateMapper;

	

	vector<ofVec2f> colorCoords;
	int numBodiesTracked;
	bool bHaveAllStreams;

public:

	ofImage bodyIndexImg, foregroundImg;
	ofImage colorImg;

	ofGrabber(ofSettings& settings);
	~ofGrabber();

    bool getPixels(ofPixels &frame);

	void update();
	void draw();

	ofxKFW2::Device kinect;
};
