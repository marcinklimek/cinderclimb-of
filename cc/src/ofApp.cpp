#include "ofApp.h"


constexpr int image_size_W = 640;
constexpr int image_size_H = 480;

//--------------------------------------------------------------
void ofApp::setup()
{
    vidGrabber.setVerbose(true);
    vidGrabber.setup(image_size_W, image_size_H);
    colorImg.allocate(image_size_W, image_size_H);
    grayImage.allocate(image_size_W, image_size_H);
    grayBg.allocate(image_size_W, image_size_H);
    grayDiff.allocate(image_size_W, image_size_H);

    bLearnBakground = true;
    threshold = 80;

    setupGui();
}

//--------------------------------------------------------------
void ofApp::setupGui()
{
    parameters.setName("parameters");
    parameters.add(radius.set("radius", 50, 1, 100));
    parameters.add(color.set("color", 100, ofColor(0, 0), 255));
    gui.setup(parameters);
    ofSetBackgroundColor(0);

    vidRecorder.setVideoCodec("mpeg4");
    vidRecorder.setVideoBitrate("2500k");
    vidRecorder.setFfmpegLocation("c:/workspace/climbing/ffmpeg/bin/ffmpeg.exe");

    bRecording = false;
    ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::exit()
{
    vidRecorder.close();
}

void ofApp::updateRecorder()
{
    int millisNow = ofGetElapsedTimeMillis();
    int millisSinceLastCheck = millisNow - lastTimeCheck;
    if (millisSinceLastCheck > (1000 / 30))
    {
        lastTimeCheck = millisNow;

        if (vidRecorder.isInitialized())
        {
            bool success = vidRecorder.addFrame(grayImage.getPixels());
            if (!success)
            {
                ofLogWarning("This frame was not added!");
            }
        }
    }
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
    grayDiff.threshold(threshold);

    //
    grayDiff.blurHeavily();
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

    vidGrabber.update();
    if (vidGrabber.isFrameNew())
    {
        auto frame = vidGrabber.getPixels();
        updateFrame(frame);
    }
 
    updateRecorder();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    // draw the incoming, the grayscale, the bg and the thresholded difference
    ofSetHexColor(0xffffff);

    ofRectangle rect;

    constexpr int preview_W = 320;
    constexpr int preview_H = 240;
    constexpr int spacing = 10;

    rect.set(spacing, spacing, preview_W, preview_H);
    colorImg.draw(rect);

    rect.set(spacing, spacing + (preview_H + spacing) * 1, preview_W, preview_H);
    grayImage.draw(rect);
    
    rect.set(spacing, spacing + (preview_H + spacing) * 2, preview_W, preview_H);
    grayBg.draw(rect);

    rect.set(spacing, spacing + (preview_H + spacing) * 3, preview_W, preview_H);
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
            << "threshold " << threshold << " (press: +/-)" << endl
            << "num blobs found " << contourFinder.nBlobs << ", fps: " << ofGetFrameRate();
    
    ofDrawBitmapString(reportStr.str(), rect.getX(), rect.getY());
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
        case '+':
            threshold++;
            if (threshold > 255) threshold = 255;
            break;
        case '-':
            threshold--;
            if (threshold < 0) threshold = 0;
            break;

        case 'v':
        {
            bRecording = !bRecording;
            if (bRecording && !vidRecorder.isInitialized())
            {
                cout << fileName + ofGetTimestampString() + fileExt << " " << vidGrabber.getWidth() << " " << vidGrabber.getHeight() << endl;
                vidRecorder.setup(fileName + ofGetTimestampString() + fileExt, vidGrabber.getWidth(), vidGrabber.getHeight(), 30.0f);
                // Start recording
                vidRecorder.start();
            }
            else if (!bRecording && vidRecorder.isInitialized())
            {
                vidRecorder.setPaused(true);
            }
            else if (bRecording && vidRecorder.isInitialized())
            {
                vidRecorder.setPaused(false);
            }
            break;
        }
        case 'c':

        {
            bRecording = false;
            vidRecorder.close();
            break;
        }
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
