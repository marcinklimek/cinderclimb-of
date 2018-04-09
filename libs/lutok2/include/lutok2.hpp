#include "common.hpp"

#define LUTOK2_NOT_USED(x) ((void)(x))

#ifdef _MSC_VER
# define __thread_local __declspec(thread) 
#else
# define __thread_local __thread
#endif

namespace lutok2 {
	class State;

	typedef int (*cxx_function)(State &);
	typedef std::function<int(State &)> Function;

	typedef std::unordered_map<std::string, cxx_function> Module;
	typedef std::vector<int> StackContent;
	static int cxx_function_wrapper(lua_State *);
	static void storeCurrentState(State *, bool);
	static int free_current_state(lua_State *);

	class BaseObject {
	public:
		virtual void getConstructor(){

		}
	};

};

#include "exceptions.hpp"
#include "stack.hpp"
#include "state.hpp"
#include "stackdebugger.hpp"
#include "object.hpp"

namespace lutok2 {

	static int cxx_function_wrapper(lua_State * L) {
		State state = State(L, false);
		Stack * stack = state.stack;
		const lua_Debug info = state.getInfo("nSlu");
		//StackContent stackContent = stack->getStackContent();

		int upvalues = info.nups;
		if (upvalues >= 1){
			int upvalueIndex = stack->upvalueIndex(1);
			stack->pushValue(upvalueIndex);
			int t = stack->type(-1);
			if (t == LUA_TUSERDATA){
				Function ** originalFunction = reinterpret_cast<Function **>(stack->to<void*>(-1));
				stack->pop(1);
				if (originalFunction != nullptr){
					try{
						return (**originalFunction)(state);
					}
					catch (const std::exception & e){
						state.error("Unhandled exception: %s", e.what());
						return 1;
					}
				}
				else{
					const std::string traceBack = state.traceback();
					state.error("Invalid function pointer!: %s", traceBack.c_str());
					return 1;
				}
			}
			else{
				const std::string traceBack = state.traceback();
				state.error("Stack corrupted!: %s", traceBack.c_str());
				return 1;
			}
		}
		else{
			const std::string traceBack = state.traceback();
			state.error("Closure upvalues corrupted!: %s", traceBack.c_str());
			return 1;
		}
	}

	static int free_current_state(lua_State * L){
		/*
		State * state = State::getCurrentState();
		//free all stored object interfaces
		for (std::unordered_map<std::string, BaseObject*>::iterator iter = state->interfaces.begin(); iter != state->interfaces.end(); iter++){
			delete (iter->second);
		}

		delete state;
		*/
		return 0;
	}
};
