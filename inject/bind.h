#pragma once
#include <lua.hpp>
#include "TH9Monitor.h"
#include <string>

namespace ka_ai_duka{
    void BindToLua(::lua_State* ls, PlayerSide player_side);
    void ExportVariables(::lua_State* ls, TH9Monitor &monitor, PlayerSide player_side, const std::string &script_dir);
    void UpdateVariables(::lua_State* ls, TH9Monitor &monitor);
}