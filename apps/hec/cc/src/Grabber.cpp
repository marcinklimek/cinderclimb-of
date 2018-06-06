#include "Grabber.h"
#include "ofxColorMap/src/ofxColorMap.h"

#define DEPTH_WIDTH 512
#define DEPTH_HEIGHT 424
#define DEPTH_SIZE DEPTH_WIDTH * DEPTH_HEIGHT

#define COLOR_WIDTH 1920
#define COLOR_HEIGHT 1080

ofGrabber::ofGrabber(std::shared_ptr<ofSettings> settings) 
{
	_settings = settings;
    kinect.open();
    kinect.initDepthSource();
    kinect.initColorSource();
    kinect.initInfraredSource();
    kinect.initBodySource();
    kinect.initBodyIndexSource();

    if (kinect.getSensor()->get_CoordinateMapper(&coordinateMapper) < 0) 
    {
        ofLogError() << "Could not acquire CoordinateMapper!";
    }

    numBodiesTracked = 0;
    haveAllStreams = false;

	depthIndex.allocate(COLOR_WIDTH, COLOR_HEIGHT);
    bodyIndex.allocate(COLOR_WIDTH, COLOR_HEIGHT);
    colorCoords.resize(DEPTH_WIDTH * DEPTH_HEIGHT);

	nearClipping  = settings->nearClipping;
	farClipping   =  settings->farClipping;
	colorMapIndex = -1;

	updateRawDepthLookupTable();
}

ofGrabber::~ofGrabber()
{
	coordinateMapper->Release();
    kinect.close();
}

bool ofGrabber::get(ofxCvColorImage& frame)
{
    if (kinect.isFrameNew() && haveAllStreams)
    {
        //if (bodyIndex.getWidth() == 0 || bodyIndex.getHeight() == 0)
        //    return false;

        //frame.setFromPixels(bodyIndex.getPixels());
        //return bodyIndex.getPixels().size();

        if (depthIndex.getWidth() == 0 || depthIndex.getHeight() == 0)
            return false;

        frame.setFromPixels(depthIndex.getPixels());
        return depthIndex.getPixels().size();
    }

    return false;
}


void ofGrabber::update()
{
    kinect.update();

    // Get pixel data
	const auto depthPixRaw = kinect.getDepthSource()->getPixels().getData();
	auto& depthPix = kinect.getDepthSource()->getPixels();
    auto& bodyIndexPix = kinect.getBodyIndexSource()->getPixels();
    auto& colorPix = kinect.getColorSource()->getPixels();
	
    // Make sure there's some data here, otherwise the cam probably isn't ready yet
    if (!depthPix.size() || !bodyIndexPix.size() || !colorPix.size() )
    {
        haveAllStreams = false;
        return;
    }

	haveAllStreams = true;

	if ( nearClipping != _settings->nearClipping || farClipping != _settings->farClipping || colorMapIndex != _settings->colorMapIndex)
	{
		nearClipping = _settings->nearClipping;
		farClipping =  _settings->farClipping;
		colorMapIndex = _settings->colorMapIndex;

		updateRawDepthLookupTable();
	}

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
    coordinateMapper->MapDepthFrameToColorSpace(DEPTH_SIZE, depthPixRaw, DEPTH_SIZE, reinterpret_cast<ColorSpacePoint*>(colorCoords.data()));
    bodyIndex.set(0);
	depthIndex.set(0);

	int index = 0;
    // Loop through the depth image
    for (int y = 0; y < DEPTH_HEIGHT; y++) 
    {
        for (int x = 0; x < DEPTH_WIDTH; x++) 
        {
            
			ofVec2f mappedCoord = colorCoords[index];
			if (mappedCoord.x < 0 || mappedCoord.y < 0 || mappedCoord.x >= COLOR_WIDTH || mappedCoord.y >= COLOR_HEIGHT) 
            {
				++index;
                continue;
            }

            // This is the check to see if a given pixel is inside a tracked  body or part of the background.
            // If it's part of a body, the value will be that body's id (0-5), or will > 5 if it's
            // part of the background
            // More info here:
            // https://msdn.microsoft.com/en-us/library/windowspreview.kinect.bodyindexframe.aspx
            const float val = bodyIndexPix[index];
            if (val < bodies.size()) 
			{
	            // Give each tracked body a color value so we can tell
	            // them apart on screen
	            ofColor c = ofColor::fromHsb(val * 255 / bodies.size(), 200, 255);

	            // For a given (x,y) in the depth image, lets look up where that point would be
	            // in the color image
	            

	            // Mapped x/y coordinates in the color can come out as floats since it's not a 1:1 mapping
	            // between depth <-> color spaces i.e. a pixel at (100, 100) in the depth image could map
	            // to (405.84637, 238.13828) in color space
	            // So round the x/y values down to ints so that we can look up the nearest pixel
	            mappedCoord.x = floor(mappedCoord.x);
	            mappedCoord.y = floor(mappedCoord.y);

	            // Make sure it's within some sane bounds, and skip it otherwise


	            // Finally, pull the color from the color image based on its coords in
	            // the depth image
				bodyIndex.getPixels().setColor(mappedCoord.x, mappedCoord.y, c);
            }

			auto v = depthPixRaw[index];
	        const ofColor& value = depthLookupTable[v];
			depthIndex.getPixels().setColor(mappedCoord.x, mappedCoord.y, value);

			++index;
        }
    }

	depthIndex.dilate(); depthIndex.dilate();
	depthIndex.erode(); 

    bodyIndex.dilate(); bodyIndex.dilate();
    bodyIndex.erode(); 
}

void ofGrabber::updateRawDepthLookupTable()
{
	ofxColorMap colormap;

	colormap.setMapFromIndex( _settings->colorMapIndex );
	// IF YOU WANT NEAR TO BE BLACK AND FAR TO BE WHITE, SWITCH THESE
	unsigned char nearColor = 0;
	unsigned char farColor = 255;

	// HOW FAR BACK WE WANT TO GRADIENT BEFORE HITTING THE MAX DISTANCE.
	// EVERYTHING PAST THE MAX DISTANCE WILL BE CLAMPPED TO 'farColor'
	unsigned int maxDepthLevels = 8001;
	depthLookupTable.resize(maxDepthLevels);

	depthLookupTable[0] = 0;
	for (unsigned int i = 1; i < maxDepthLevels; i++)
	{
		const auto f = ofMap(i, nearClipping, farClipping, nearColor, farColor, false);
		
		if (f > static_cast<float>(nearColor) && f < static_cast<float>(farColor))
			depthLookupTable[i] = colormap.use(static_cast<int>(f));
		else
			depthLookupTable[i] = 0;
	}
}

