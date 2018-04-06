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
    float z;

    Vector3(): x(0), y(0), z(0) {}
};

class LVector3 : public Object<Vector3> {
public:
    explicit LVector3(State * state) : Object<Vector3>(state) {
        LUTOK_PROPERTY("x", &LVector3::getX, &LVector3::nullMethod);
        LUTOK_PROPERTY("y", &LVector3::getY, &LVector3::nullMethod);
        LUTOK_PROPERTY("z", &LVector3::getZ, &LVector3::nullMethod);
    }

    Vector3* constructor(State & state, bool & managed);
    void destructor(State & state, Vector3 * object);

    int getX(State & state, Vector3* object);
    int getY(State & state, Vector3* object);
    int getZ(State & state, Vector3* object);

};

void initVector3(State *);


class UberObject
{
private:
    std::string value;
public:
    
    UberObject();

    int getNumJoints();
    int getJoint();

    std::shared_ptr<AnalysisThread> analysis;
};

extern UberObject uberObject;


class LUberObject : public Object<UberObject> 
{
public:
    explicit LUberObject(State * state);
    
    UberObject * constructor(State & state, bool & managed);
    void destructor(State & state, UberObject * object);

    int getNumJoints(State & state, UberObject * object);
    int getJoint(State & state, UberObject * object);
};

int luaopen_UberObject(lua_State * L);