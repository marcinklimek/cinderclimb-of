#pragma once
#include "ofParameter.h"
#include "ofRectangle.h"

constexpr int DEPTH_WIDTH = 512;
constexpr int DEPTH_HEIGHT = 424;
constexpr int DEPTH_SIZE = DEPTH_WIDTH * DEPTH_HEIGHT;

constexpr int COLOR_WIDTH = 1920;
constexpr int COLOR_HEIGHT = 1080;
constexpr int COLOR_SIZE = COLOR_WIDTH * COLOR_HEIGHT;

constexpr int preview_W = 256;
constexpr int preview_H = 212;
constexpr int spacing = 10;

class ofSettings
{
public:

    ofSettings();

	void scan_for_scripts();
	string get_script();

	int image_size_W = DEPTH_WIDTH;
	int image_size_H = DEPTH_HEIGHT;

    ofRectangle color_preview_pos;

    ofParameterGroup parameters;

	ofParameter<int> colorMapIndex;

    ofParameter<float> tolerance;
	ofParameter<int> smoothing;

    ofParameter<int> blur_amount;
    ofParameter<int> erode_open_count;
	ofParameter<int> erode_close_count;
    ofParameter<int> dillate_count;

    ofParameter<int> blur_amount2;
    ofParameter<int> erode_open_count2;
	ofParameter<int> erode_close_count2;
    ofParameter<int> dillate_count2;

    ofParameter<float> area_min;
    ofParameter<float>  area_max;

	ofParameter<int> nearClipping;
	ofParameter<int> farClipping;

    ofParameter<bool> resetBackground;
    ofParameter<int> resetBackgroundTime;

	ofParameter<float> thresholdValue;
    ofParameter<float> epsilon;

    ofParameterGroup& get_gui_parameters();

	vector<string> scripts;
	ofParameter<int> currentScript;
};
