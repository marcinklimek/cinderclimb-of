#pragma once
#include "ofMain.h"

#define NCOLORS 256

class ofxCvColorImage;
class ofxCvGrayscaleImage;

class ofxColorMap {
public:
    typedef vector<unsigned char> LookupTable;
    typedef vector<LookupTable> ColorTable;

    typedef vector<ofVec3f> ColorMapChannel;
    typedef vector<ColorMapChannel> ColorMap;

    static void makeLookupTable1D(const ColorMapChannel& data, LookupTable& table, size_t n, float gamma = 1.0f);

    void setMapFromName(string name);
    void setMapFromIndex(int n);
	int getMapIndex() const { return mapIndex_; }

    void setMap(const ColorMap& m);

    void apply(ofImage& img1, ofImage& img2);
	void apply(ofxCvColorImage& img1, ofxCvColorImage& img2);
    ofColor use(ofColor c);
    ofColor use(unsigned char value);

    ofxColorMap();
    ~ofxColorMap() {}
    static const map<string, ColorMap> namedMaps;

private:
	int mapIndex_;
    ColorTable lookup;
    static map<string, ColorMap> createNamedMaps();
};
