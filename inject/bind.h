#pragma once
#include <lua.hpp>
#include "TH9Monitor.h"

namespace ka_ai_duka{
    void BindToLua(::lua_State* ls, PlayerSide player_side);
    void ExportVariables(::lua_State* ls, TH9Monitor &monitor, PlayerSide player_side);
    void UpdateVariables(::lua_State* ls, TH9Monitor &monitor);
}