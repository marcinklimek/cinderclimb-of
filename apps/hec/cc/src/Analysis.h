#pragma once

#include "ofxCvContourFinder.h"
#include "Settings.h"
#include "ofxCvColorImage.h"
#include "Grabber.h"
#include "convexHull/ofxConvexHull.h"
#include "ofxKinectForWindows2/src/ofxKinectForWindows2.h"

class AnalysisThread  : public ofThread
{
public:

    AnalysisThread() = delete;
    AnalysisThread(const AnalysisThread&) = delete;
    AnalysisThread(AnalysisThread&&) = delete;
    AnalysisThread& operator=(const AnalysisThread&) = delete;
    AnalysisThread& operator=(AnalysisThread&&) = delete;

    explicit AnalysisThread(std::shared_ptr<ofSettings> settings);
    virtual ~AnalysisThread();

    void setup();
	bool update();
	void stop();

	void draw();
    void drawBlobs(vector<ofxCvBlob>& blobs) const;
	void drawJoints(vector<ofVec2f>& joints);
	void drawBlobs(ofRectangle& rect, vector<ofxCvBlob> blobs);
	
	void updateJoints();
	int getNumJoints() const;
	ofVec2f getJoint(int jointIdx) const;

    bool getBlobs(vector<ofxCvBlob>& blobs);
    
	void threadedFunction() override;

    float mouseX;
    float mouseY;

    ofVec2f projectorScreenSize;

	ofRectangle sensingWindow;

private:

	bool _quit;
    void updateFrame(ofxCvColorImage& frame);
    
    std::shared_ptr<ofSettings> _settings;

    ofGrabber grabber;
    ofxConvexHull convexHull;

    ofxCvColorImage _inputFrame;
    ofxCvColorImage _imageProcessed;
    ofxCvGrayscaleImage _imageProcessedGray;
    ofxCvColorImage _colorFrame;

    ofxCvContourFinder contourFinder;

	mutable std::mutex _drawUpdateMutex;
	
	std::vector<ofVec2f> _joints;
	vector<ofxCvBlob> _blobs;

	vector<ofxCvBlob> _blobsPublic;
	std::vector<ofVec2f> _jointsPublic;

    ofxCvColorImage _inputFramePublic;
    ofxCvColorImage _imageProcessedPublic;
    ofxCvGrayscaleImage _imageProcessedGrayPublic;
    ofxCvColorImage _colorFramePublic;
};
