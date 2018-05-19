#pragma once
#include <memory>
#include <lutok2/lutok2.hpp>

#include "Analysis.h"

using namespace lutok2;

class Vector3
{
public:
    float x;
    float y;

    Vector3(): x(0), y(0) {}
	Vector3(float _x, float _y): x(_x), y(_y) {}
};

class LVector3 : public Object<Vector3> {
public:
    explicit LVector3(State * state) : Object<Vector3>(state) {
        LUTOK_PROPERTY("x", &LVector3::getX, &LVector3::nullMethod);
        LUTOK_PROPERTY("y", &LVector3::getY, &LVector3::nullMethod);
    }

    Vector3* constructor(State & state, bool & managed);
    void destructor(State & state, Vector3 * object);

    int getX(State & state, Vector3* object);
    int getY(State & state, Vector3* object);
};

class UberObject
{
private:
    std::string value;
public:
    
    UberObject();
	int updateJoints();

	int getNumJoints();
	Vector3 getJoint(int idx);

	int getNumBlobs();
	vector<Vector3> getBlob(int idx);

    std::shared_ptr<AnalysisThread> analysis;
};

extern UberObject uberObject;


class LUberObject : public Object<UberObject> 
{
public:
    explicit LUberObject(State * state);
    
    UberObject * constructor(State & state, bool & managed);
    void destructor(State & state, UberObject * object);

	int updateJoints(State& state, UberObject* object);
	int getNumJoints(State & state, UberObject * object);
    int getJoint(State & state, UberObject * object);

	int getNumBlobs(State& state, UberObject* object);
	int getBlob(State& state, UberObject* object);
};

int luaopen_UberObject(lua_State * L);