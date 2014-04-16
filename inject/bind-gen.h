#pragma once
#include "TH9Monitor.h"
#include <lua.hpp>
namespace ka_ai_duka{
    void lua_bind_all(::lua_State* ls, PlayerSide player_side);
}
