#pragma once

#include "ofxCvContourFinder.h"
#include "Settings.h"
#include "ofxCvColorImage.h"
#include "Grabber.h"
#include "convexHull/ofxConvexHull.h"
#include "ofxKinectForWindows2/src/ofxKinectForWindows2.h"
#include "ofxHomography.h"


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

	void stop();

	void draw();
    void draw_blobs(vector<ofPolyline>& blobs) const;
	void draw_joints(vector<ofVec2f>& joints) const;
	
	void update_joints();
	int get_num_bodies() const;
	int get_num_joints() const;
	ofVec2f get_joint(int joint_idx) const;

	int get_num_blobs() const;
	
	std::vector<ofVec2f> get_blob(size_t idx) const;
	ofRectangle get_blob_min_max(size_t idx) const;
	bool point_in_blobs(const ofPoint p, float distance);

	void threadedFunction() override;
	std::vector<ofVec2f> get_joints(const int body_index) const;

	float mouse_x;
    float mouse_y;

    ofVec2f projector_screen_size;

	ofRectangle sensing_window;

	HomoTrans sensing_trans;

private:

	bool quit_;
    void update_frame(ofxCvColorImage& frame);
    
    std::shared_ptr<ofSettings> settings_;

    ofGrabber grabber_;
    ofxConvexHull convex_hull_;

    ofxCvColorImage input_frame_;
	ofxCvColorImage input_frame_public_;

    ofxCvColorImage image_processed_;
	ofxCvColorImage image_processed_public_;

    ofxCvGrayscaleImage image_processed_gray_;
	ofxCvGrayscaleImage image_processed_gray_public_;

    ofxCvColorImage color_frame_;
	ofxCvColorImage color_frame_public_;

    ofxCvContourFinder contour_finder_;

	mutable std::mutex update_mutex_;
	
	std::vector<ofVec2f> joints_;
	std::vector<ofVec2f> joints_public_;

	vector<ofPolyline> blobs_path_;
	vector<ofPolyline> blobs_path_public_;
	

	CvMemStorage* cv_mem_storage_;

	void simplify_dp(const vector<ofPoint>& contour_in, vector<ofPoint>& contour_out, float tolerance) const;

	bool update_public();
};
