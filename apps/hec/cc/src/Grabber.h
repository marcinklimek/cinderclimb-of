#pragma once

//#define USE_STANDARD_CAMERA

#include "ofPixels.h"
#include "ofVideoGrabber.h"
#include "Settings.h"
#include "ofxKinectForWindows2/src/ofxKinectForWindows2.h"
#include "ofxCvColorImage.h"

class ofGrabber
{
	std::shared_ptr<ofSettings> _settings;

	ICoordinateMapper* coordinateMapper;

	vector<ofVec2f> colorCoords;
	int numBodiesTracked;
	bool haveAllStreams;

public:

	ofxCvColorImage bodyIndex;
	ofImage colorImg;

	ofGrabber(std::shared_ptr<ofSettings> settings);
	~ofGrabber();

    bool getPixels(ofPixels &frame);

	ICoordinateMapper* getCoordinateMapper() const { return coordinateMapper; }

	void update();
	void draw();
	bool get(ofxCvColorImage& frame);

	ofxKFW2::Device kinect;
};
