#include "LuaScript.hpp"

#include <lua.hpp>

namespace LuaScript {
    lua_State* state;
}

bool LuaScript::init() {
    state = luaL_newstate();
    if (state == nullptr) {
        return false;
    }
    return true;
}

void LuaScript::quit() {
    lua_close(state);
}