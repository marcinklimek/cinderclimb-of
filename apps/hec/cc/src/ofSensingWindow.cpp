#include "ofSensingWindow.h"

ofSensingWindow::ofSensingWindow(): is_changed_(false)
{
    reset();
}

void ofSensingWindow::reset()
{
    points[0].x = 0;
    points[0].y = 0;
    points[1].x = 1;
    points[1].y = 0;
    points[2].x = 1;
    points[2].y = 1;
    points[3].x = 0;
    points[3].y = 1;

    for (auto& point : points)
    {
        point.is_over = false;
        point.is_dragged = false;
        point.radius = 0.018;
    }
}

void ofSensingWindow::update_homography()
{
    sensing_trans.distortedCorners[0] = points[0]();
    sensing_trans.distortedCorners[1] = points[1]();
    sensing_trans.distortedCorners[2] = points[2]();
    sensing_trans.distortedCorners[3] = points[3]();

    sensing_trans.update();
}


void ofSensingWindow::draw(float w, float h)
{
    ofSetColor(0, 128, 128, 128);

	ofBeginShape();

		for (auto& point : points)
			ofVertex(point.x * w, point.y * h);

	ofEndShape(true);
	
	ofSetColor(255, 255, 255, 128);

	for (auto& point : points)
    {
		if (point.is_over) 
            ofFill();
		else 
            ofNoFill();

		ofDrawCircle(point.x * w, point.y * h, 8);
	}
}


//------------- -------------------------------------------------
void ofSensingWindow::mouseMoved(float x, float y)
{
	for (auto& point : points)
	{
	    const float diffx = x - point.x;
	    const float diffy = y - point.y;
	    const float dist = sqrt(diffx*diffx + diffy*diffy);

	    point.is_over = dist <= point.radius;
	}
}

//--------------------------------------------------------------
void ofSensingWindow::mouseDragged(float x, float y, int button)
{
	for (auto& point : points)
	{
		if (point.is_dragged)
        {
            point.x = x;
            point.y = y;

            is_changed_ = true;
		}
	}

    if (is_changed_)
        update_homography();
}

//--------------------------------------------------------------
void ofSensingWindow::mousePressed(float x, float y, int button)
{
	for (auto& point : points)
	{
		const float diffx = x - point.x;
		const float diffy = y - point.y;
		const float dist = sqrt(diffx*diffx + diffy*diffy);

        point.is_dragged = dist <= point.radius;
	}
}

//--------------------------------------------------------------
void ofSensingWindow::mouseReleased(float x, float y, int button)
{
	for (auto& point : points)
	{
	    point.is_dragged = false;	
	}
}

void ofSensingWindow::save(ofxXmlSettings& xml)
{
    string tag{};
    auto counter = 0;
	for (auto& point : points)
	{
        tag = "SensingWindow-x-" + to_string(counter);
	    xml.setValue( tag, point.x );

        tag = "SensingWindow-y-" + to_string(counter);
        xml.setValue( tag, point.y );

        counter++;
	}
}

void ofSensingWindow::load(ofxXmlSettings& xml)
{
    string tag{};
    auto counter = 0;
    auto zero_counter = 0;

	for (auto& point : points)
	{
        tag = "SensingWindow-x-" + to_string(counter);
	    point.x = xml.getValue( tag, 0.0f );

        tag = "SensingWindow-y-" + to_string(counter);
        point.y = xml.getValue( tag, 0.0f );

        if ( (point.x < 0.00001) && (point.y < 0.00001) )
        {
            zero_counter++;
        }

        counter++;
	}

    auto equal_counter = 0;
    for (auto& pointA : points)
        for (auto& pointB : points)
        {
            if (pointA == pointB)
            {
                equal_counter++;
                break;
            }
        }

    if (zero_counter > 1 || equal_counter > 0)
    {
        reset();
    }

    update_homography();
}
