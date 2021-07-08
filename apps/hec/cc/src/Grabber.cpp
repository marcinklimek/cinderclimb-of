#include "Grabber.h"
#include "ofxColorMap/src/ofxColorMap.h"


ofGrabber::ofGrabber(std::shared_ptr<ofSettings> settings) 
{
	_settings = settings;
    kinect.open();

    if ( !kinect.isOpen() )
    {
		// display message box  
		MessageBox(0, _T("Brak kamery Kinect"), _T("Error"),   MB_OK | MB_ICONERROR);
    	exit(EXIT_FAILURE);
    }
	
    kinect.initDepthSource();
    kinect.initColorSource();
    kinect.initInfraredSource();
    kinect.initBodySource();
    kinect.initBodyIndexSource();

    if (kinect.getSensor()->get_CoordinateMapper(&coordinateMapper) < 0) 
    {
		MessageBox(0, _T("Problem z uzyskaniem CoordinateMapper"), _T("Error"),   MB_OK | MB_ICONERROR);
    	exit(EXIT_FAILURE);    	
    }

    haveAllStreams = false;

    depthIndex.allocate(DEPTH_WIDTH, DEPTH_HEIGHT);

    colorIndex.allocate(DEPTH_WIDTH, DEPTH_HEIGHT);

	nearClipping  = settings->nearClipping;
	farClipping   = settings->farClipping;
	colorMapIndex = -1;

	updateRawDepthLookupTable();
}

ofGrabber::~ofGrabber()
{
	coordinateMapper->Release();
    kinect.close();
}

bool ofGrabber::get(ofxCvShortImage& frame)
{
    if (kinect.isFrameNew() && haveAllStreams)
    {
        if (depthIndex.getWidth() == 0 || depthIndex.getHeight() == 0)
            return false;

        frame = depthIndex;
        

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
    
    
    const auto colorPixRaw = kinect.getColorSource()->getPixels().getData();
    auto& colorPix = kinect.getColorSource()->getPixels();

	
    // Make sure there's some data here, otherwise the cam probably isn't ready yet
    if (!depthPix.size() || !colorPix.size() )
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

    colorIndex.set(0);
    const auto color_index_raw = colorIndex.getPixels().getData();

    std::vector<DepthSpacePoint> depthSpacePoints( COLOR_SIZE );
    kinect.getDepthSource()->getDepthInColorFrameMapping(depthSpacePoints);

    auto dsp_pos = 0;
    auto color_pos = 0;


    for (auto i = 0; i < COLOR_SIZE; i++) 
    {
        const auto depth_x = static_cast<int>( depthSpacePoints[dsp_pos].X + 0.5f);
        const auto depth_y = static_cast<int>( depthSpacePoints[dsp_pos].Y + 0.5f);

        if ( ( 0 <= depth_x ) && ( depth_x < DEPTH_WIDTH ) && ( 0 <= depth_y ) && ( depth_y < DEPTH_HEIGHT ) )
        {
            const unsigned int depth_pos = (depth_y * DEPTH_WIDTH + depth_x) * 3;
            
            color_index_raw[depth_pos    ] = colorPixRaw[color_pos];
            color_index_raw[depth_pos + 1] = colorPixRaw[color_pos + 1];
            color_index_raw[depth_pos + 2] = colorPixRaw[color_pos + 2];
        }

        dsp_pos++;
        color_pos += 4;
    }


    auto& depth_pix_ref = depthIndex.getShortPixelsRef();
    depth_pix_ref.setFromPixels(depthPixRaw, DEPTH_WIDTH, DEPTH_HEIGHT, 1);
    
   //  auto index = 0;
   //  for (auto y = 0; y < DEPTH_HEIGHT; y++) 
   //  {
   //      for (auto x = 0; x < DEPTH_WIDTH; x++) 
   //      {
   //          const auto v = depthPixRaw[index];
	  //       const auto& value = depthLookupTable[v];
			// depthColor.getPixels().setColor(x, y, value);
   //
			// index++;
   //      }
   //  }
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

