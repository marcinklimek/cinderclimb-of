#include "ofxColorMap.h"
#include "assert.h"
#include "ofxCvGrayscaleImage.h"
#include "ofxCvColorImage.h"

const map<string, ofxColorMap::ColorMap> ofxColorMap::namedMaps = ofxColorMap::createNamedMaps();

ofxColorMap::ofxColorMap() {
    setMapFromName("gray");
	mapIndex_ = 26;
}

void ofxColorMap::makeLookupTable1D(const ColorMapChannel& data, LookupTable& table, size_t n, float gamma)
{
    // TODO: check validity of data (start with 0, ends with 1, strictly increasing)
    table.resize(n);

    size_t idx = 0;

    for(size_t i = 0; i < n; i++) {
        float r = (float)i/(n-1);

        assert(r >= data[idx].x); // data[0].x should be 0.0.

        if(r < 1.0 && r >= data[idx+1].x) {
            idx++;
        }

        float prop = (r - data[idx].x)/(data[idx+1].x - data[idx].x);
        float val = data[idx].z + (data[idx+1].y - data[idx].z)*prop;

        table[i] = (NCOLORS-1)*val;
    }

}

void ofxColorMap::setMapFromName(string name)
{
    map<string, ColorMap>::const_iterator it;
    it = namedMaps.find(name);
    if(it == namedMaps.end()) {
        cout << "Could not find color map " << name << "." << endl;
        return;
    }

    setMap(it->second);

}

void ofxColorMap::setMapFromIndex(int n)
{
	if (n == mapIndex_)
		return;

	auto saved_n = n;
	for(const auto& pair: namedMaps)
	{
		if (n == 0)
		{
			mapIndex_ = saved_n;
			setMap(pair.second);
			return;
		}

		n--;
	}

	mapIndex_ = saved_n;
	setMap(namedMaps.begin()->second);
}

void ofxColorMap::setMap(const ColorMap& m)
{
    lookup.clear();
    assert(m.size() == 3); // RGB, 3 channels.
    for(size_t i = 0; i < m.size(); i++) {
        const ColorMapChannel& c = m[i];
        LookupTable t;
        makeLookupTable1D(c,t,NCOLORS);
        assert(t.size() == NCOLORS);
        lookup.push_back(t);
    }
    assert(lookup.size() == 3);
    assert(lookup[0].size() == NCOLORS);
}

void ofxColorMap::apply(ofxCvColorImage& img1, ofxCvColorImage& img2)
{
    assert(img1.getWidth() == img2.getWidth());
    assert(img1.getHeight() == img2.getHeight());

    ofPixels& p1 = img1.getPixels();
    ofPixels& p2 = img2.getPixels();

    for(size_t i = 0; i < img1.getWidth()*img1.getHeight(); i++) {
        ofColor c = p1.getColor(i*3);
		c = use(c);
		p2.setColor(i*3, c);
        //p2[3*i+0] = c.r;
        //p2[3*i+1] = c.g;
        //p2[3*i+2] = c.b;
    }

}

void ofxColorMap::apply(ofImage& img1, ofImage& img2)
{
    assert(img1.getWidth() == img2.getWidth());
    assert(img1.getHeight() == img2.getHeight());

    ofPixels& p1 = img1.getPixels();
    ofPixels& p2 = img2.getPixels();

    for(size_t i = 0; i < img1.getWidth()*img1.getHeight(); i++) {
        ofColor c = use(p1[i]);

        p2[3*i+0] = c.r;
        p2[3*i+1] = c.g;
        p2[3*i+2] = c.b;
    }

    img2.update();

}

ofColor ofxColorMap::use(ofColor c)
{
    float value = (c.r/3.0+c.g/3.0+c.b/3.0);
    return use((unsigned char)value);
}

ofColor ofxColorMap::use(unsigned char value)
{
    ofColor output;
    output.r = lookup[0][value];
    output.g = lookup[1][value];
    output.b = lookup[2][value];
    return output;
}

