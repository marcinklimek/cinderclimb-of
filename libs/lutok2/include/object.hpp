#ifndef LUTOK2_OBJECT_H
#define LUTOK2_OBJECT_H

#include <forward_list>

namespace lutok2 {
#define LUTOK_PROPERTY(KEY, GETTER_FN, SETTER_FN) properties[(KEY)] = PropertyPair(static_cast<Method>(GETTER_FN), static_cast<Method>(SETTER_FN));
#define	LUTOK_METHOD(KEY, METHOD_FN) methods[(KEY)] = static_cast<Method>(METHOD_FN);

	template <class C>
	class Object : public BaseObject{
	public:
		typedef int (Object<C>::*Method) (State &, C *);
		typedef struct std::pair< Method, Method > PropertyPair;
		typedef std::unordered_map< std::string, Method > MethodMap;
		typedef std::unordered_map< std::string, PropertyPair > PropertyMap;

		typedef struct {
			C * instance;
			bool owned;
		} ObjWrapper;
	public:
		lua_State * luaState;
	protected:
		inline ObjWrapper * getWrapped(const int index){
			State state = State(luaState, false);
			Stack * stack = state.stack;
			ObjWrapper * wrapper = static_cast<ObjWrapper *>(stack->getUserData(index, typeid(C).name()));
			return wrapper;
		}

		inline ObjWrapper * getWrapped(const int index, const std::string & typeName){
			State state = State(luaState, false);
			Stack * stack = state.stack;
			ObjWrapper * wrapper = static_cast<ObjWrapper *>(stack->getUserData(index, typeName));
			return wrapper;
		}

		ObjWrapper * getWrapped(const int index, const std::forward_list<std::string> & typeNames){
			State state = State(luaState, false);
			Stack * stack = state.stack;
			ObjWrapper * wrapper = nullptr;

			for (std::forward_list<std::string>::const_iterator iter = typeNames.begin(); iter != typeNames.end(); iter++){
				wrapper = static_cast<ObjWrapper *>(stack->getUserData(index, *iter));
				if (wrapper != nullptr){
					return wrapper;
				}
			}
			
			return nullptr;
		}
	private:
		int index(State & state, C * object){
			Stack * stack = state.stack;
			if (stack->is<LUA_TNUMBER>(1)){
				return operator_getArray(state, object);
			}else if (stack->is<LUA_TSTRING>(1)){
				const std::string key = stack->to<const std::string>(1);
				stack->remove(1);
				//traverse property and method maps
				{
					typename PropertyMap::iterator propertyIterator = properties.find(key);
					if (propertyIterator != properties.end()){
						PropertyPair & pair = propertyIterator->second;
						return (this->*(pair.first))(state, object);
					}
					else{
						typename MethodMap::iterator methodIterator = methods.find(key);
						if (methodIterator != methods.end()){
							Method & method = methodIterator->second;
							stack->push<Function>([=, &state](State & state) -> int {
								return (this->*(method))(state, object);
							});
							return 1;
						}else{
							return 0;
						}
					}
				}
			}
			return 0;
		}
		int newindex(State & state, C * object){
			Stack * stack = state.stack;
			if (stack->is<LUA_TNUMBER>(1)){
				operator_setArray(state, object);
				return 0;
			}else if (stack->is<LUA_TSTRING>(1)){
				const std::string key = stack->to<const std::string>(1);
				stack->remove(1);
				//traverse property map
				{
					typename PropertyMap::iterator propertyIterator = properties.find(key);
					if (propertyIterator != properties.end()){
						PropertyPair & pair = propertyIterator->second;
						return (this->*(pair.second))(state, object);
					}
					else{
						return 0;
					}
				}
			}
			return 0;
		}
	public:
		Object(Object & object){
			this->state = object.state;
			this->methods = object.methods;
			this->properties = object.properties;
		}
		explicit Object(State * state){
			this->luaState = state->state;
		}
		explicit Object(lua_State * state){
			this->luaState = state;
		}
		virtual ~Object(){

		}

		MethodMap methods;
		PropertyMap properties;

		int nullMethod (State & state, C * object){
			LUTOK2_NOT_USED(state);
			LUTOK2_NOT_USED(object);
			return 0;
		};

		void prepareMetatable(){
			State state = State(luaState, false);
			Stack * stack = state.stack;
			const char * tname = typeid(C).name();
			if (stack->newMetatable(tname)){
				stack->setField("typename", tname);
				stack->setField<Function>("__gc", [this](State & state) -> int {
					luaState = state.state;
					ObjWrapper * wrapped = getWrapped(1);
					if (wrapped->owned){
						destructor(state, wrapped->instance);
					}
					return 0;
				});
				stack->setField<Function>("__typename", [this](State & state) -> int {
					luaState = state.state;
					//C * object = get(1);
					state.stack->push(typeid(C).name());
					return 1;
				});

				stack->setField<Function>("__index", [this](State & state) -> int {
					luaState = state.state;
					C * object = get(1);
					int t = state.stack->type(1);
					state.stack->remove(1);
					return index(state, object);
				});
				stack->setField<Function>("__newindex", [this](State & state) -> int {
					luaState = state.state;
					C * object = get(1);
					state.stack->remove(1);
					return newindex(state, object);
				});

				stack->setField<Function>("__add", [this](State & state) -> int {
					luaState = state.state;
					return operator_add(state, get(1), get(2));
				});
				stack->setField<Function>("__sub", [this](State & state) -> int {
					luaState = state.state;
					return operator_sub(state, get(1), get(2));
				});
				stack->setField<Function>("__mul", [this](State & state) -> int {
					luaState = state.state;
					return operator_mul(state, get(1), get(2));
				});
				stack->setField<Function>("__div", [this](State & state) -> int {
					luaState = state.state;
					return operator_div(state, get(1), get(2));
				});
				stack->setField<Function>("__mod", [this](State & state) -> int {
					luaState = state.state;
					return operator_mod(state, get(1), get(2));
				});
				stack->setField<Function>("__pow", [this](State & state) -> int {
					luaState = state.state;
					return operator_pow(state, get(1), get(2));
				});
				stack->setField<Function>("__unm", [this](State & state) -> int {
					luaState = state.state;
					return operator_unm(state, get(1));
				});
				stack->setField<Function>("__concat", [this](State & state) -> int {
					luaState = state.state;
					return operator_concat(state, get(1), get(2));
				});
				stack->setField<Function>("__len", [this](State & state) -> int {
					luaState = state.state;
					return operator_len(state, get(1));
				});
				stack->setField<Function>("__eq", [this](State & state) -> int {
					luaState = state.state;
					return operator_eq(state, get(1), get(2));
				});
				stack->setField<Function>("__lt", [this](State & state) -> int {
					luaState = state.state;
					return operator_lt(state, get(1), get(2));
				});
				stack->setField<Function>("__le", [this](State & state) -> int {
					luaState = state.state;
					return operator_le(state, get(1), get(2));
				});
				stack->setField<Function>("__call", [this](State & state) -> int {
					luaState = state.state;
					return operator_call(state, get(1));
				});
				stack->setField<Function>("__tostring", [this](State & state) -> int {
					luaState = state.state;
					C * obj = get(1);
					int retvals = operator_tostring(state, obj);
					if (retvals<=0){
						char buffer[128];
						sprintf(buffer, "userdata: 0x%p", static_cast<void*>(obj));
						state.stack->push<const std::string &>(buffer);
						return 1;
					}else{
						return retvals;
					}
				});
			}
		}

		void getConstructor(){
			State state = State(luaState, false);
			Stack * stack = state.stack;
			stack->newTable();
			//metatable
				stack->newTable();
				stack->setField<Function>("__call", [this](State & state) -> int {
					state.stack->remove(1);
					bool managed = true;
					C * obj = constructor(state, managed);
					if (obj != nullptr){
						push(obj, managed);
						return 1;
					}else{
						state.error("Couldn't create object: %s", typeid(C).name());
						return 1;
					}
				});
			stack->setMetatable(-2);
		}

		void push(C * instance, const bool manage = false){
			State state = State(luaState, false);
			Stack * stack = state.stack;
			ObjWrapper * wrapper = static_cast<ObjWrapper *>(stack->newUserData(sizeof(ObjWrapper)));
			wrapper->instance = instance;
			wrapper->owned = manage;
			prepareMetatable();
			stack->setMetatable();
		}

		C * get(const int index){
			ObjWrapper * wrapper = getWrapped(index);
			if (wrapper){
				return wrapper->instance;
			}else{
				return nullptr;
			}
		}

		/*
		Constructor/Destructor
		*/

		virtual C * constructor(State & state, bool & managed){
			LUTOK2_NOT_USED(state);
			LUTOK2_NOT_USED(managed);
			return nullptr;
		}

		virtual void destructor(State & state, C * object){
			LUTOK2_NOT_USED(state);
			LUTOK2_NOT_USED(object);
		}

		/*
		Metamethods
		*/

		virtual int operator_add(State & state, C * a, C * b){
			LUTOK2_NOT_USED(state);
			LUTOK2_NOT_USED(a);
			LUTOK2_NOT_USED(b);
			return 0;
		}

		virtual int operator_sub(State & state, C * a, C * b){
			LUTOK2_NOT_USED(state);
			LUTOK2_NOT_USED(a);
			LUTOK2_NOT_USED(b);
			return 0;
		}

		virtual int operator_mul(State & state, C * a, C * b){
			LUTOK2_NOT_USED(state);
			LUTOK2_NOT_USED(a);
			LUTOK2_NOT_USED(b);
			return 0;
		}

		virtual int operator_div(State & state, C * a, C * b){
			LUTOK2_NOT_USED(state);
			LUTOK2_NOT_USED(a);
			LUTOK2_NOT_USED(b);
			return 0;
		}

		virtual int operator_mod(State & state, C * a, C * b){
			LUTOK2_NOT_USED(state);
			LUTOK2_NOT_USED(a);
			LUTOK2_NOT_USED(b);
			return 0;
		}

		virtual int operator_pow(State & state, C * a, C * b){
			LUTOK2_NOT_USED(state);
			LUTOK2_NOT_USED(a);
			LUTOK2_NOT_USED(b);
			return 0;
		}

		virtual int operator_unm(State & state, C * a){
			LUTOK2_NOT_USED(state);
			LUTOK2_NOT_USED(a);
			return 0;
		}

		virtual int operator_concat(State & state, C * a, C * b){
			LUTOK2_NOT_USED(state);
			LUTOK2_NOT_USED(a);
			LUTOK2_NOT_USED(b);
			return 0;
		}

		virtual int operator_len(State & state, C * a){
			LUTOK2_NOT_USED(state);
			LUTOK2_NOT_USED(a);
			return 0;
		}

		virtual int operator_eq(State & state, C * a, C * b){
			LUTOK2_NOT_USED(state);
			LUTOK2_NOT_USED(a);
			LUTOK2_NOT_USED(b);
			return 0;
		}

		virtual int operator_lt(State & state, C * a, C * b){
			LUTOK2_NOT_USED(state);
			LUTOK2_NOT_USED(a);
			LUTOK2_NOT_USED(b);
			return 0;
		}

		virtual int operator_le(State & state, C * a, C * b){
			LUTOK2_NOT_USED(state);
			LUTOK2_NOT_USED(a);
			LUTOK2_NOT_USED(b);
			return 0;
		}

		virtual int operator_call(State & state, C * a){
			LUTOK2_NOT_USED(state);
			LUTOK2_NOT_USED(a);
			return 0;
		}

		virtual int operator_tostring(State & state, C * a){
			LUTOK2_NOT_USED(state);
			LUTOK2_NOT_USED(a);
			return 0;
		}

		/*
		Array access
		*/

		virtual int operator_getArray(State & state, C * a){
			LUTOK2_NOT_USED(state);
			LUTOK2_NOT_USED(a);
			return 0;
		}

		virtual void operator_setArray(State & state, C * a){
			LUTOK2_NOT_USED(state);
			LUTOK2_NOT_USED(a);
		}
	
	};

};

#endif