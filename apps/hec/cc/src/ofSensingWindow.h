#pragma once
#include <array>
#include "ofPoint.h"
#include "ofxXmlSettings/src/ofxXmlSettings.h"

class DraggableVertex
{

public:
	float 	x;
	float 	y;
	bool 	is_dragged;
	bool 	is_over;
	float 	radius;

    ofPoint operator () () const
    {
        return {x, y, 0.0f};
    }

    bool operator == (const DraggableVertex& other) const
    {
        if (this == &other)
            return false;

        return (x == other.x) && (y == other.y);
    }
};

class ofSensingWindow
{
    bool is_changed_;

public:

    std::array<DraggableVertex, 4> points{};

    bool is_changed()
    {
        if( is_changed_ )
        {
            is_changed_ = false;
            return true;
        }

        return false;
    }

    ofSensingWindow();

    void reset();

    void draw(float w, float h);
    void mouseMoved(float x, float y);
    void mouseDragged(float x, float y, int button);
    void mousePressed(float x, float y, int button);
    void mouseReleased(float x, float y, int button);

    void save(ofxXmlSettings& xml);
    void load(ofxXmlSettings& xml);
};
