#include "Uber.h"

using namespace lutok2;


UVector2* LUVector2::constructor(State & state, bool & managed) 
{
    LUTOK2_NOT_USED(managed);

    return new UVector2();
}

void LUVector2::destructor(State & state, UVector2* object) 
{
    LUTOK2_NOT_USED(state);

    delete object;
}

int LUVector2::getX(State & state, UVector2* object)
{
    state.stack->push<LUA_NUMBER>(object->x);
    return 1;
}

int LUVector2::getY(State & state, UVector2* object)
{
    state.stack->push<LUA_NUMBER>(object->y);
    return 1;
}


UberObject uber_object;

UberObject::UberObject() = default;

int UberObject::update_joints() const
{
	analysis->update_joints();

	return 1;
}

int UberObject::get_num_joints() const
{
	return analysis->get_num_joints();
}

UVector2 UberObject::get_joint(const int idx) const
{
    auto data = analysis->get_joint(idx);
	data = data * ofVec2f(10.24, 7.68);

	return {data.x, data.y};
}

int UberObject::get_num_blobs() const
{
	return  analysis->get_num_blobs();
}

vector<UVector2> UberObject::get_blob(const int idx) const
{
	auto blob = analysis->get_blob(idx);

	vector<UVector2> v;
	for(auto pt : blob)
	{
		pt = pt * ofVec2f(10.24, 7.68);
		v.emplace_back(pt.x, pt.y);
	}

	return v;
}

int UberObject::get_num_bodies() const
{
	return analysis->get_num_bodies();
}

std::vector<UVector2> UberObject::get_joints(const int body_index) const
{
	auto joints = analysis->get_joints(body_index);

	vector<UVector2> v;
	for(auto pt : joints)
	{
		pt = pt * ofVec2f(10.24, 7.68);
		v.emplace_back(pt.x, pt.y);
	}

	return v;
}

LUberObject::LUberObject(State * state) : Object<UberObject>(state)
{
	LUTOK_PROPERTY("numBodies", &LUberObject::get_num_bodies, &LUberObject::nullMethod);
    LUTOK_PROPERTY("numJoints", &LUberObject::get_num_joints, &LUberObject::nullMethod);
    LUTOK_METHOD("joint", &LUberObject::get_joint);

	LUTOK_PROPERTY("numBlobs", &LUberObject::get_num_blobs, &LUberObject::nullMethod);
	LUTOK_METHOD("blob", &LUberObject::get_blob);

	LUTOK_METHOD("insideBlob", &LUberObject::inside_blob);
}

UberObject * LUberObject::constructor(State & state, bool & managed)
{
    managed = true;
    return &uber_object;
}

void LUberObject::destructor(State & state, UberObject * object) 
{
}

int LUberObject::get_num_joints(State & state, UberObject * object) 
{
    state.stack->push<int>(object->get_num_joints());
    return 1;
}

int LUberObject::get_num_bodies(State & state, UberObject * object) 
{
    state.stack->push<int>(object->get_num_joints());
    return 1;
}

int LUberObject::get_joint(State & state, UberObject * object)
{
	auto interfaceLVector3 = state.getInterface<LUVector2>("Vector3");

	auto v = state.stack->typeName();

	if (state.stack->is<LUA_TNUMBER>(1))
	{
		const auto index = static_cast<int>(state.stack->to<int>(1) - 1);
		if (index >= 0 && index < object->get_num_joints())
		{
			const auto joint = object->get_joint(index);
			interfaceLVector3->push(new UVector2(joint), true);

			return 1;
		}
	}

	return 0;
}

int LUberObject::get_joints(State & state, UberObject * object)
{
	auto interfaceLVector3 = state.getInterface<LUVector2>("Vector3");

	if (state.stack->is<LUA_TNUMBER>(1))
	{
		state.stack->newTable();

		const auto body_index = static_cast<int>(state.stack->to<int>(1) - 1);
		if (body_index >= 0 && body_index < object->get_num_bodies())
		{
			const auto joints = object->get_joints(body_index);

			auto idx = 1;
			for(const auto point: joints)
			{
				state.stack->push<int>(idx);
				interfaceLVector3->push(new UVector2(point), true);
				state.stack->rawSet();

				++idx;
			}

			return 1;
		}
	}

	return 0;
}

int LUberObject::get_num_blobs(State& state, UberObject* object)
{
	state.stack->push<int>(object->get_num_blobs());

	return 1;
}

int LUberObject::get_blob(State& state, UberObject* object)
{
	auto interfaceLVector3 = state.getInterface<LUVector2>("Vector3");

	if (state.stack->is<LUA_TNUMBER>(1))
	{
		state.stack->newTable();

		const auto index = static_cast<int>(state.stack->to<int>(1) - 1);
		if (index >= 0 && index < object->get_num_blobs())
		{
			const auto blob = object->get_blob(index);

			auto idx = 1;
			for(const auto point: blob)
			{
				state.stack->push<int>(idx);
				interfaceLVector3->push(new UVector2(point), true);
				state.stack->rawSet();

				++idx;
			}

			return 1;
		}
	}

	return 0;
}

int LUberObject::inside_blob(State& state, UberObject* object)
{
	if (state.stack->is<LUA_TNUMBER>(1) && state.stack->is<LUA_TNUMBER>(2) && state.stack->is<LUA_TNUMBER>(3))
	{

		const auto x = static_cast<LUA_NUMBER>(  state.stack->to<LUA_NUMBER>(1));
		const auto y = static_cast<LUA_NUMBER>(state.stack->to<LUA_NUMBER>(2));
		const auto r = static_cast<LUA_NUMBER>(state.stack->to<LUA_NUMBER>(3));

		state.stack->push<bool>(uber_object.analysis->point_in_blobs( ofPoint(x,y), r ));
		
		return 1;
	}

	return 0;
}

int luaopen_UberObject(lua_State* L)
{
	auto state = new State(L);

    state->registerInterface<LUVector2>("Vector3");
    state->registerInterface<LUberObject>("UberObj");
    state->stack->setGlobal("UberObj");

    return 0;
}
