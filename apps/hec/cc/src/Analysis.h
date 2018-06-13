#pragma once

#include "ofxCvContourFinder.h"
#include "Settings.h"
#include "ofxCvColorImage.h"
#include "Grabber.h"
#include "convexHull/ofxConvexHull.h"
#include "ofxKinectForWindows2/src/ofxKinectForWindows2.h"
#include "ofxHomography.h"
#include "ofxCv/RunningBackground.h"


class AnalysisThread  : public ofThread
{
public:

    AnalysisThread() = delete;
    AnalysisThread(const AnalysisThread&) = delete;
    AnalysisThread(AnalysisThread&&) = delete;
    AnalysisThread& operator=(const AnalysisThread&) = delete;
    AnalysisThread& operator=(AnalysisThread&&) = delete;

    explicit AnalysisThread(const std::shared_ptr<ofSettings>& settings);
    virtual ~AnalysisThread();

    void setup();

	void stop();

	void draw();
    void draw_blobs(vector<ofPolyline>& blobs) const;
	void draw_joints(vector<ofVec2f>& joints) const;
	
	void update_joints();
	int get_num_bodies() const;
	int get_num_joints() const;
	ofVec2f get_joint(int jointIdx) const;

	int get_num_blobs() const;
	
	std::vector<ofVec2f> get_blob(size_t idx) const;
	ofRectangle get_blob_min_max(size_t idx) const;
	bool point_in_blobs(const ofPoint p, float distance);

	void threadedFunction() override;
    void average(ofxCvShortImage& image_input_a, ofxCvShortImage& image_input_b) const;
    void inRange(ofxCvShortImage & image_input_a, float min, float max) const;
    void foreground(ofxCvShortImage& image_input_a, ofxCvShortImage& image_mask) const;
    void resetChanged(bool& state);
    std::vector<ofVec2f> get_joints(const int body_index) const;
	void set_fbo_texture(const ofTexture& of_texture);

	float mouse_x;
    float mouse_y;

    ofVec2f projector_screen_size;

	ofRectangle sensing_window;

	HomoTrans sensing_trans;
	ofPixels fbo_pixels;
    int averageTimer;

private:

	bool quit_;

    void update_frame();
    
    std::shared_ptr<ofSettings> settings_;

    ofGrabber grabber_;
    ofxConvexHull convex_hull_;

    ofxCvShortImage input_frame_;
	ofxCvColorImage input_frame_public_;

    ofxCvShortImage image_processed_;
	ofxCvColorImage image_processed_public_;

	ofxCvShortImage backgroundModel_;
    ofxCvGrayscaleImage backgroundModel_public_;

    ofxCvShortImage image_foreground_;
	ofxCvGrayscaleImage image_foreground_public_;

    ofxCvColorImage color_frame_;
	ofxCvColorImage color_frame_public_;

    ofxCvContourFinder contour_finder_;

	mutable std::mutex update_mutex_;
	
	std::vector<ofVec2f> joints_;
	std::vector<ofVec2f> joints_public_;

	vector<ofPolyline> blobs_path_;
	vector<ofPolyline> blobs_path_public_;

    unsigned long long fps_;
    unsigned long long frame_counter_;
    

	bool update_public();
};
