#include "Grabber.h"
#include "ofApp.h"
#include <videoio.hpp>
#include "ofxCv/Utilities.h"

ofGrabber::ofGrabber()
{

    vidGrabber.setVerbose(true);
	vidGrabber.setup(image_size_W, image_size_H);

	cout << "Available devices" << endl;
	for( auto device : vidGrabber.listDevices() )
	{
		cout << device.deviceName << " " << device.hardwareName << " " << device.id << endl;

		for(auto format : device.formats)
		{
			cout << "  " << format.width << "x" << format.height << " format: " << format.pixelFormat;


			cout << "rates: ";
			for(auto rate: format.framerates)
			{
				cout << rate << " ";
			}
			
			cout << endl;
		}
	}

	cout.flush();
}

bool ofGrabber::getPixels(ofPixels &frame)
{
	//cv::Mat mat;
	//cv::OutputArray cvFrame(mat);

	//cap >> mat;
	//ofxCv::toOf(mat, frame);

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