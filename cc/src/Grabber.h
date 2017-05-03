#pragma once

#include "ofPixels.h"
#include "ofVideoGrabber.h"

class ofGrabber
{
    ofVideoGrabber vidGrabber;

public:

    ofGrabber();

    bool getPixels(ofPixels &frame);

    int getWidth() const;
    int getHeight() const;
};
