#include "Grabber.h"
#include "ofApp.h"

ofGrabber::ofGrabber()
{
    vidGrabber.setVerbose(true);
    vidGrabber.setup(image_size_W, image_size_H);
}

bool ofGrabber::getPixels(ofPixels &frame)
{
    vidGrabber.update();

    if (vidGrabber.isFrameNew())
    {
        frame = vidGrabber.getPixels();
        return true;
    }

    return false;
}

int ofGrabber::getWidth() const
{
    return vidGrabber.getWidth();
}

int ofGrabber::getHeight() const
{
    return vidGrabber.getHeight();
}