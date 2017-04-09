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

ofRecorder::ofRecorder(): bRecording(false), lastTimeCheck(0)
{
    vidRecorder.setVideoCodec("mpeg4");
    vidRecorder.setVideoBitrate("2500k");
    vidRecorder.setFfmpegLocation("ffmpeg/bin/ffmpeg.exe");
}

void ofRecorder::start(int w, int h, float fps)
{
    stop();
    
    if (bRecording && !vidRecorder.isInitialized())
    {
        cout << fileName + ofGetTimestampString() + fileExt << " " << w << " " << h << endl;
        vidRecorder.setup(fileName + ofGetTimestampString() + fileExt, w, h, fps);

        // Start recording
        vidRecorder.start();
        bRecording = true;
    }
    else if (!bRecording && vidRecorder.isInitialized())
    {
        vidRecorder.setPaused(true);
    }
    else if (bRecording && vidRecorder.isInitialized())
    {
        vidRecorder.setPaused(false);
    }
}


void ofRecorder::stop()
{
    bRecording = false;
    vidRecorder.close();
}

void ofRecorder::update(ofPixels& frame)
{
    int millisNow = ofGetElapsedTimeMillis();
    int millisSinceLastCheck = millisNow - lastTimeCheck;
    if (millisSinceLastCheck > (1000 / 30))
    {
        lastTimeCheck = millisNow;

        if (vidRecorder.isInitialized())
        {
            bool success = vidRecorder.addFrame(frame);
            if (!success)
            {
                ofLogWarning("This frame was not added!");
            }
        }
    }
}

ofSettings::ofSettings()
{
    parameters.setName("Parameters");

    parameters.add(threshold.set("threshold", 80, 0, 255));
    parameters.add(blur_amount.set("blur amount", 10, 1, 30));
    parameters.add(erosion_size.set("erosion size", 3, 1, 1000));
    parameters.add(circle_size.set("circle size", 30, 1, 100));
    parameters.add(area_min.set("area min", 1000.0f, 1.0f, 500000.0f));
    parameters.add(area_max.set("area max", 100000.0f, 1.0f, 500000.0f));
}

ofParameterGroup& ofSettings::get_gui_parameters()
{
    return parameters;
}

//--------------------------------------------------------------
void ofApp::setup()
{
    colorImg.allocate(image_size_W, image_size_H);
    grayImage.allocate(image_size_W, image_size_H);
    grayBg.allocate(image_size_W, image_size_H);
    grayDiff.allocate(image_size_W, image_size_H);

    bLearnBakground = true;


    setupGui();
}

//--------------------------------------------------------------
void ofApp::setupGui()
{

    gui.setup(_settings.get_gui_parameters());
    gui.setPosition(spacing + preview_W + spacing, spacing);

    ofSetBackgroundColor(0);


    ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::exit()
{
    recorder.stop();
}

void ofApp::updateRecorder()
{
    recorder.update(colorImg.getPixels());
}

void ofApp::updateFrame(ofPixels& frame)
{
    colorImg.ofxCvImage::setFromPixels(frame);

    grayImage = colorImg;
    if (bLearnBakground == true)
    {
        grayBg = grayImage; // the = sign copys the pixels from grayImage into grayBg (operator overloading)
        bLearnBakground = false;
    }

    // take the abs value of the difference between background and incoming and then threshold:
    grayDiff.absDiff(grayBg, grayImage);

    grayDiff.blur(_settings.blur_amount);
    grayDiff.threshold(_settings.threshold);

    //
    
    grayDiff.erode();
    grayDiff.dilate();
    grayDiff.dilate();
    grayDiff.erode();

    // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
    // also, find holes is set to true so we will get interior contours as well....
    contourFinder.findContours(grayDiff, 20, (640 * 480) / 3, 10, true); // find holes
}

//--------------------------------------------------------------
void ofApp::update()
{
    ofBackground(100, 100, 100);
    
    ofPixels frame;
    if ( grabber.getPixels(frame) )
    { 
        updateFrame(frame);
        updateRecorder();
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    // draw the incoming, the grayscale, the bg and the thresholded difference
    ofSetHexColor(0xffffff);

    ofRectangle rect;

    rect.set(spacing, spacing, preview_W, preview_H);
    colorImg.draw(rect);

    rect.set(spacing, spacing + (preview_H + spacing) * 1, preview_W, preview_H);
    grayDiff.draw(rect);

    // then draw the contours:
    rect.set(spacing + preview_W + spacing, spacing, image_size_W, image_size_H);

    ofFill();
    ofSetHexColor(0x333333);
    ofDrawRectangle(rect);
    ofSetHexColor(0xffffff);

    // we could draw the whole contour finder
    contourFinder.draw(rect);

    for (int i = 0; i < contourFinder.blobs.size(); i++)
    {
        contourFinder.blobs[i].draw(rect.getX(), rect.getY());
    }

    // finally, a report:
    ofSetHexColor(0xffffff);
    stringstream reportStr;
    reportStr << "bg subtraction and blob detection" << endl
            << "press ' ' to capture bg" << endl
            << "num blobs found " << contourFinder.nBlobs << ", fps: " << ofGetFrameRate();
    
    ofDrawBitmapString(reportStr.str(), rect.getX()+ spacing, rect.getY()+ spacing);
}

//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs& args)
{
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key)
    {
        case ' ':
            bLearnBakground = true;
            break;

        case 'v':
        {
            recorder.start(grabber.getWidth(), grabber.getHeight(), 30.0f);

            break;
        }
        case 'c':
        {
            recorder.stop();
            break;
        }
        default: break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}
