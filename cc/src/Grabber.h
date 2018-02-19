#pragma once

#include "ofPixels.h"
#include "ofDirectShowGrabber.h"

class ofGrabber
{
    ofDirectShowGrabber vidGrabber;

public:

	ofGrabber();

    bool getPixels(ofPixels &frame);

    int getWidth() const;
    int getHeight() const;
};
