#pragma once
#include <memory>
#include <lutok2/lutok2.hpp>

#include "Analysis.h"

using namespace lutok2;

class UVector2
{
public:
    float x;
    float y;

    UVector2(): x(0), y(0) {}
	UVector2(float _x, float _y): x(_x), y(_y) {}
};

class LUVector2 : public Object<UVector2> {
public:
    explicit LUVector2(State * state) : Object<UVector2>(state) {
        LUTOK_PROPERTY("x", &LUVector2::getX, &LUVector2::nullMethod);
        LUTOK_PROPERTY("y", &LUVector2::getY, &LUVector2::nullMethod);
    }

    UVector2* constructor(State & state, bool & managed) override;
    void destructor(State & state, UVector2 * object) override;

    int getX(State & state, UVector2* object);
    int getY(State & state, UVector2* object);
};

class UberObject
{
private:
    std::string value;
public:
    
    UberObject();
	int update_joints() const;

	int get_num_joints() const;
	UVector2 get_joint(int idx) const;

	int get_num_blobs() const;
	vector<UVector2> get_blob(int idx) const;
	int get_num_bodies() const;
	std::vector<UVector2> get_joints(const int body_index) const;

	std::shared_ptr<AnalysisThread> analysis;
};

extern UberObject uber_object;


class LUberObject : public Object<UberObject> 
{
public:
    explicit LUberObject(State * state);
    
    UberObject * constructor(State & state, bool & managed) override;
    void destructor(State & state, UberObject * object) override;

	int get_num_joints(State & state, UberObject * object);
	int get_num_bodies(State& state, UberObject* object);
	int get_joint(State & state, UberObject * object);
	int get_joints(State& state, UberObject* object);

	int get_num_blobs(State& state, UberObject* object);
	int get_blob(State& state, UberObject* object);
	int inside_blob(State& state, UberObject* object);
	int get_blob_min_max(State& state, UberObject* object);
};

int luaopen_UberObject(lua_State * L);