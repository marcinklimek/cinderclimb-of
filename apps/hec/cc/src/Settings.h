#pragma once
#include "ofParameter.h"

constexpr int preview_W = 320;
constexpr int preview_H = 240;
constexpr int spacing = 10;

class ofSettings
{
public:

    ofSettings();

	float image_size_W = 1920/2;
	float image_size_H = 1080/2;

    ofParameterGroup parameters;
    //ofParameter<float> radius;
    //ofParameter<ofColor> color;

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

    //ofParameter<int> circle_size;

    ofParameter<float> area_min;
    ofParameter<float>  area_max;

    //ofParameter<float> brightness;
    //ofParameter<float>  contrast;
    //ofParameter<float>  learingRate;

	ofParameter<bool>  useConvexHull;
	//ofParameter<float> gamma;
	//ofParameter<float> amplify;

	ofParameter<float> nearClipping;
	ofParameter<float> farClipping;

    ofParameter<bool> resetBackground;
    ofParameter<float> learningTime;
	ofParameter<float> thresholdValue;

    ofParameterGroup& get_gui_parameters();

};
