#pragma once

//#define USE_STANDARD_CAMERA

#include "ofPixels.h"
#include "ofVideoGrabber.h"
#include "Settings.h"
#include "ofxKinectForWindows2/src/ofxKinectForWindows2.h"
#include "ofxCvColorImage.h"
#include "ofxCvShortImage.h"


class ofGrabber
{
	std::shared_ptr<ofSettings> _settings;

    bool haveAllStreams;
	ICoordinateMapper* coordinateMapper;
	vector<ofVec2f> depthCoords;
    int numBodiesTracked;

	vector<ofColor>	depthLookupTable;
	float nearClipping;
    float farClipping;
	int colorMapIndex;

public:

    ofxCvShortImage depthIndex;

	ofxCvColorImage colorIndex;

	ofGrabber(std::shared_ptr<ofSettings> settings);
	~ofGrabber();

	ICoordinateMapper* getCoordinateMapper() const { return coordinateMapper; }

	void update();
	void updateRawDepthLookupTable();
	bool get(ofxCvShortImage& frame);
    
    int numBodies() const { return numBodiesTracked; }

	ofxKFW2::Device kinect;
};
