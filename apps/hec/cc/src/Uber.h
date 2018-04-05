#pragma once

#include "lutok2/lutok2.hpp"

using namespace lutok2;

class UberObject 
{
private:
	std::string value;
public:
	
	UberObject(const std::string & value);

	std::string getValue();
	
	void setValue(const std::string & value);
};

class LUberObject : public Object<UberObject> 
{
public:
	explicit LUberObject(State * state);
	
	UberObject * constructor(State & state, bool & managed);
	
	void destructor(State & state, UberObject * object);
	int operator_concat(State & state, UberObject * a, UberObject * b);

	int getValue(State & state, UberObject * object);
	int setValue(State & state, UberObject * object);
	int method(State & state, UberObject * object);

};

int luaopen_UberObject(lua_State * L);