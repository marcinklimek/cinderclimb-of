#include "Uber.h"

using namespace lutok2;

UberObject::UberObject(const std::string & value) 
{
    this->value = value;
}

std::string UberObject::getValue() 
{
    return value;
}

void UberObject::setValue(const std::string & value) 
{
    this->value = value;
}


LUberObject::LUberObject(State * state) : Object<UberObject>(state)
{
    LUTOK_PROPERTY("value", &LUberObject::getValue, &LUberObject::setValue);
    LUTOK_METHOD("method", &LUberObject::method);
}

UberObject * LUberObject::constructor(State & state, bool & managed)
{
    UberObject * obj = nullptr;
    Stack * stack = state.stack;
    if (stack->is<LUA_TSTRING>(1)) {
        const std::string value = stack->to<const std::string>(1);
        obj = new UberObject(value);
    }
    managed = true;
    return obj;
}

void LUberObject::destructor(State & state, UberObject * object) 
{
    delete object;
}

int LUberObject::operator_concat(State & state, UberObject * a, UberObject * b) 
{
    push(new UberObject(a->getValue() + b->getValue()), true);
    return 1;
}

int LUberObject::getValue(State & state, UberObject * object) 
{
    state.stack->push<const std::string &>(object->getValue());
    return 1;
}

int LUberObject::setValue(State & state, UberObject * object) 
{
    const std::string value = state.stack->to<const std::string>(1);
    object->setValue(value);
    return 0;
}

int LUberObject::method(State & state, UberObject * object) 
{
    state.stack->push<const std::string &>("Hello");
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
    state->registerInterface<LUberObject>("UberObj");
    state->stack->setGlobal("UberObj");
    return 0;
}