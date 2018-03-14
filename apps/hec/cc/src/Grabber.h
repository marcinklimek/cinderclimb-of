#pragma once

//#define USE_STANDARD_CAMERA

#include "ofPixels.h"
#include "ofVideoGrabber.h"
#include "Settings.h"
#include "ofxKinectForWindows2/src/ofxKinectForWindows2.h"
#include "ofxCvColorImage.h"

class ofGrabber
{
	ofSettings& _settings;

	std::shared_ptr<ICoordinateMapper> coordinateMapper;

	vector<ofVec2f> colorCoords;
	int numBodiesTracked;
	bool haveAllStreams;

public:

	ofxCvColorImage bodyIndex;
	ofImage colorImg;

	ofGrabber(ofSettings& settings);
	~ofGrabber();

    bool getPixels(ofPixels &frame);

	std::shared_ptr<ICoordinateMapper> getCoordinateMapper() const { return coordinateMapper; }

	void update();
	void draw();
	bool get(ofxCvColorImage& frame);

	ofxKFW2::Device kinect;
};
