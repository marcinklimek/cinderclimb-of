#include "Uber.h"

using namespace lutok2;

void initVector3(State * state) {
    
}

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

int LVector3::getZ(State & state, Vector3* object)
{
    state.stack->push<LUA_NUMBER>(object->z);
    return 1;
}


UberObject uberObject;

UberObject::UberObject() 
{
}

int UberObject::getNumJoints()
{
    return 1;
}

int UberObject::getJoint()
{
    return 0;
}



LUberObject::LUberObject(State * state) : Object<UberObject>(state)
{
    LUTOK_PROPERTY("numJoints", &LUberObject::getNumJoints, &LUberObject::nullMethod);
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


int LUberObject::getNumJoints(State & state, UberObject * object) 
{
    state.stack->push<LUA_NUMBER>(object->getNumJoints());
    return 1;
}

int LUberObject::getJoint(State & state, UberObject * object)
{
    LVector3 * interfaceLVector3 = state.getInterface<LVector3>("Vector3");

    auto v = new Vector3();
    v->x = 13;
    v->y = 14;
    v->z = 15;

    interfaceLVector3->push(v, true);
    return 1;
}


int luaopen_UberObject(lua_State* L)
{
    //state->stack->push<Function>([](State & state) -> int {
    //    state.loadString(
    //        "print(\"Hello world from lambda function!\") \
    //        "
    //    );
    //    state.stack->call(0, 0);
    //    return 0;
    //});
    //state->stack->setGlobal("testing");

    int t = lua_gettop(L);
    State * state = new State(L);
    Stack * stack = state->stack;

    state->registerInterface<LVector3>("Vector3");
    state->registerInterface<LUberObject>("UberObj");
    initVector3(state);
    state->stack->setGlobal("UberObj");
    return 0;
}