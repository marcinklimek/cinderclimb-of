#pragma once
#include "ofParameter.h"

constexpr int image_size_W = 640;
constexpr int image_size_H = 480;
constexpr int preview_W = 320;
constexpr int preview_H = 240;
constexpr int spacing = 10;

class ofSettings
{
public:

    ofSettings();

    ofParameterGroup parameters;
    ofParameter<float> radius;
    ofParameter<ofColor> color;

    ofParameter<float> threshold;

    ofParameter<int> blur_amount;
    ofParameter<int> erode_count;
    ofParameter<int> dillate_count;
    ofParameter<int> circle_size;

    ofParameter<float> area_min;
    ofParameter<float>  area_max;

    ofParameter<float> brightness;
    ofParameter<float>  contrast;


    ofParameterGroup& get_gui_parameters();

};
