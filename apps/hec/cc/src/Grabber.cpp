#include "Grabber.h"

#define DEPTH_WIDTH 512
#define DEPTH_HEIGHT 424
#define DEPTH_SIZE DEPTH_WIDTH * DEPTH_HEIGHT

#define COLOR_WIDTH 1920
#define COLOR_HEIGHT 1080

ofGrabber::ofGrabber(ofSettings& settings) : _settings(settings)
{
	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();
	kinect.initInfraredSource();
	kinect.initBodySource();
	kinect.initBodyIndexSource();

	ICoordinateMapper* cp;
	if (kinect.getSensor()->get_CoordinateMapper(&cp) < 0) 
	{
		ofLogError() << "Could not acquire CoordinateMapper!";
	}
	coordinateMapper.reset(cp);

	numBodiesTracked = 0;
	haveAllStreams = false;

	bodyIndex.allocate(COLOR_WIDTH, COLOR_HEIGHT);
	colorCoords.resize(DEPTH_WIDTH * DEPTH_HEIGHT);
}

ofGrabber::~ofGrabber()
{
	kinect.close();
}

bool ofGrabber::get(ofxCvColorImage& frame)
{
	if (kinect.isFrameNew() && haveAllStreams)
	{
		frame.setFromPixels(bodyIndex.getPixels());
		return true;
	}

	return false;
}


void ofGrabber::update()
{
	kinect.update();

	// Get pixel data
	auto& depthPix = kinect.getDepthSource()->getPixels();
	auto& bodyIndexPix = kinect.getBodyIndexSource()->getPixels();
	auto& colorPix = kinect.getColorSource()->getPixels();
	
	// Make sure there's some data here, otherwise the cam probably isn't ready yet
	if (!depthPix.size() || !bodyIndexPix.size() || !colorPix.size()) 
	{
		haveAllStreams = false;
		return;
	}
	
	haveAllStreams = true;

	// Count number of tracked bodies
	numBodiesTracked = 0;
	auto& bodies = kinect.getBodySource()->getBodies();
	for (auto& body : bodies) 
	{
		if (body.tracked) 
		{
			numBodiesTracked++;
		}
	}

	// Do the depth space -> color space mapping
	// More info here:
	// https://msdn.microsoft.com/en-us/library/windowspreview.kinect.coordinatemapper.mapdepthframetocolorspace.aspx
	// https://msdn.microsoft.com/en-us/library/dn785530.aspx
	coordinateMapper->MapDepthFrameToColorSpace(DEPTH_SIZE, (UINT16*)depthPix.getData(), DEPTH_SIZE, (ColorSpacePoint*)colorCoords.data());

	bodyIndex.set(0);

	// Loop through the depth image
	for (int y = 0; y < DEPTH_HEIGHT; y++) {
		for (int x = 0; x < DEPTH_WIDTH; x++) {
			int index = (y * DEPTH_WIDTH) + x;
			
			// This is the check to see if a given pixel is inside a tracked  body or part of the background.
			// If it's part of a body, the value will be that body's id (0-5), or will > 5 if it's
			// part of the background
			// More info here:
			// https://msdn.microsoft.com/en-us/library/windowspreview.kinect.bodyindexframe.aspx
			const float val = bodyIndexPix[index];
			if (val >= bodies.size()) {
				continue;
			}

			// Give each tracked body a color value so we can tell
			// them apart on screen
			ofColor c = ofColor::fromHsb(val * 255 / bodies.size(), 200, 255);

			// For a given (x,y) in the depth image, lets look up where that point would be
			// in the color image
			ofVec2f mappedCoord = colorCoords[index];

			// Mapped x/y coordinates in the color can come out as floats since it's not a 1:1 mapping
			// between depth <-> color spaces i.e. a pixel at (100, 100) in the depth image could map
			// to (405.84637, 238.13828) in color space
			// So round the x/y values down to ints so that we can look up the nearest pixel
			mappedCoord.x = floor(mappedCoord.x);
			mappedCoord.y = floor(mappedCoord.y);

			// Make sure it's within some sane bounds, and skip it otherwise
			if (mappedCoord.x < 0 || mappedCoord.y < 0 || mappedCoord.x >= COLOR_WIDTH || mappedCoord.y >= COLOR_HEIGHT) 
			{
				continue;
			}

			// Finally, pull the color from the color image based on its coords in
			// the depth image
			bodyIndex.getPixels().setColor(mappedCoord.x,     mappedCoord.y,     c);
		}
	}

	bodyIndex.dilate(); bodyIndex.dilate();
	bodyIndex.erode(); bodyIndex.erode();
}

