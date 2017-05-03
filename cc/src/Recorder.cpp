#include "Recorder.h"
#include "ofxVideoRecorder/src/ofxVideoRecorder.h"

ofRecorder::ofRecorder() : bRecording(false), lastTimeCheck(0)
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