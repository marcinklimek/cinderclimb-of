#include "Uber.h"

using namespace lutok2;


Vector3* LVector3::constructor(State & state, bool & managed) {
    LUTOK2_NOT_USED(managed);
    Vector3* obj = new Vector3();

    return obj;
}

void LVector3::destructor(State & state, Vector3* object) 
{
    LUTOK2_NOT_USED(state);
    delete object;
}

int LVector3::getX(State & state, Vector3* object)
{
    state.stack->push<LUA_NUMBER>(object->x);
    return 1;
}

int LVector3::getY(State & state, Vector3* object)
{
    state.stack->push<LUA_NUMBER>(object->y);
    return 1;
}


UberObject uberObject;

UberObject::UberObject() 
{
}

int UberObject::updateJoints()
{
	analysis->updateJoints();

	return 1;
}

int UberObject::getNumJoints()
{
	return analysis->getNumJoints();
}

Vector3 UberObject::getJoint(int idx)
{
    auto data = analysis->getJoint(idx);

	return Vector3(data.x, data.y);
}



LUberObject::LUberObject(State * state) : Object<UberObject>(state)
{
    LUTOK_PROPERTY("numJoints", &LUberObject::getNumJoints, &LUberObject::nullMethod);
	LUTOK_METHOD("updateJoints", &LUberObject::updateJoints);
    LUTOK_METHOD("joint", &LUberObject::getJoint);
}

UberObject * LUberObject::constructor(State & state, bool & managed)
{
    managed = true;
    return &uberObject;
}

void LUberObject::destructor(State & state, UberObject * object) 
{
}


int LUberObject::updateJoints(State & state, UberObject * object) 
{
	object->updateJoints();
    return 1;
}

int LUberObject::getNumJoints(State & state, UberObject * object) 
{
    state.stack->push<LUA_NUMBER>(object->getNumJoints());
    return 1;
}

int LUberObject::getJoint(State & state, UberObject * object)
{
	auto interfaceLVector3 = state.getInterface<LVector3>("Vector3");

	auto v = state.stack->typeName();

	if (state.stack->is<LUA_TNUMBER>(1))
	{
		const auto index = static_cast<int>(state.stack->to<int>(1) - 1);
		if (index >= 0 && index < object->getNumJoints())
		{
			const auto joint = object->getJoint(index);
			interfaceLVector3->push(new Vector3(joint), true);

			return 1;
		}
	}

	return 0;
}


int luaopen_UberObject(lua_State* L)
{
    State * state = new State(L);

    state->registerInterface<LVector3>("Vector3");
    state->registerInterface<LUberObject>("UberObj");
    state->stack->setGlobal("UberObj");

    return 0;
}