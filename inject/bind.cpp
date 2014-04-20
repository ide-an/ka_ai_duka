#include "bind.h"
#include "GameSide.h"
#include "sandbox.h"
//#include <luabind\luabind.hpp>
//#include <luabind\iterator_policy.hpp>
//#include <luabind\operator.hpp>
#include <boost\shared_ptr.hpp>

namespace ka_ai_duka{
    

    void SendKeys1P(KeyState key_state)
    {
        key_state &= keys::z | keys::x | keys::shift | keys::right | keys::left | keys::up | keys::down;
        monitor->SetKeyState(Side_1P, key_state);
    }

    void SendKeys2P(KeyState key_state)
    {
        key_state &= keys::z | keys::x | keys::shift | keys::right | keys::left | keys::up | keys::down;
        monitor->SetKeyState(Side_2P, key_state);
    }

    //lua wrapperes
    int Lua_SendKey1P(lua_State* ls)
    {
        luaL_argcheck(ls, lua_isnumber(ls, 1), 1, "'int' expected.");
        KeyState a1 = (KeyState) lua_tonumber(ls, 1);
        SendKeys1P(a1);
        return 0;
    }

    int Lua_SendKey2P(lua_State* ls)
    {
        luaL_argcheck(ls, lua_isnumber(ls, 1), 1, "'int' expected.");
        KeyState a1 = (KeyState) lua_tonumber(ls, 1);
        SendKeys2P(a1);
        return 0;
    }

    double GetAsNumber(lua_State* ls, const char* key, int idx)
    {
        lua_pushstring(ls, key);
        lua_gettable(ls, idx); 
        double res = lua_tonumber(ls, -1);
        lua_pop(ls, 1);
        return res;
    }
    
    //pooled hittable objects
    static managed_types::HittableRect args_hittable_rect[2];
    static managed_types::HittableCircle args_hittable_circle[2];
    static managed_types::HittableRotatableRect args_hittable_rotatable_rect[2];

    managed_types::HittableObject* ArgToHittableObject(lua_State* ls, int idx)
    {   
        managed_types::HitType type = (managed_types::HitType) ((int) GetAsNumber(ls, "type", idx));
        switch(type){
        case managed_types::Hit_Rect:
            args_hittable_rect[idx - 1].SetX((float) GetAsNumber(ls, "x", idx));
            args_hittable_rect[idx - 1].SetY((float) GetAsNumber(ls, "y", idx));
            args_hittable_rect[idx - 1].SetWidth((float) GetAsNumber(ls, "width", idx));
            args_hittable_rect[idx - 1].SetHeight((float) GetAsNumber(ls, "height", idx));
            return &args_hittable_rect[idx - 1];
        case managed_types::Hit_Circle:
            args_hittable_circle[idx - 1].SetX((float) GetAsNumber(ls, "x", idx));
            args_hittable_circle[idx - 1].SetY((float) GetAsNumber(ls, "y", idx));
            args_hittable_circle[idx - 1].SetRadius((float) GetAsNumber(ls, "radius", idx));
            return &args_hittable_circle[idx - 1];
        case managed_types::Hit_RotatableRect:
            args_hittable_rotatable_rect[idx - 1].SetX((float) GetAsNumber(ls, "x", idx));
            args_hittable_rotatable_rect[idx - 1].SetY((float) GetAsNumber(ls, "y", idx));
            args_hittable_rotatable_rect[idx - 1].SetWidth((float) GetAsNumber(ls, "width", idx));
            args_hittable_rotatable_rect[idx - 1].SetHeight((float) GetAsNumber(ls, "height", idx));
            args_hittable_rotatable_rect[idx - 1].SetAngle((float) GetAsNumber(ls, "angle", idx));
            return &args_hittable_rotatable_rect[idx - 1];
        default:
            {
                char errmsg[0xff];
                sprintf_s(errmsg, "'hitbody table' expected. This arg has unknown hitbody.type %d", type);
                luaL_argerror(ls, idx, errmsg);
                return nullptr;
            }
        }
    }

    int Lua_HitTest(lua_State* ls)
    {
        luaL_argcheck(ls, lua_istable(ls, 1), 1, "'hitbody table' expected.");
        luaL_argcheck(ls, lua_istable(ls, 2), 2, "'hitbody table' expected.");
        
        managed_types::HittableObject* arg1_hittable = ArgToHittableObject(ls, 1);
        managed_types::HittableObject* arg2_hittable = ArgToHittableObject(ls, 2);
        assert(arg1_hittable != nullptr);
        assert(arg2_hittable != nullptr);
        bool res = managed_types::HitTest(arg1_hittable, arg2_hittable);
        lua_pushboolean(ls, res ? 1 : 0);
        return 1;
    }

    void BindToLua(::lua_State* ls, PlayerSide player_side)
    {
        lua_pushcfunction(ls, player_side == Side_1P ? Lua_SendKey1P : Lua_SendKey2P);
        lua_setglobal(ls, "sendKeys");
        lua_pushcfunction(ls, Lua_HitTest);
        lua_setglobal(ls, "hitTest");
    }
    
    //lua utilities
    void SetNewTable(lua_State* ls, const char* key, int index=-3)
    {
        ::lua_pushstring(ls, key);
        ::lua_newtable(ls);
        ::lua_settable(ls, index);
    }
    void SetNumber(lua_State* ls, const char* key, ::lua_Number value, int index=-3)
    {
        ::lua_pushstring(ls, key);
        ::lua_pushnumber(ls, value);
        ::lua_settable(ls, index);
    }
    void SetBool(lua_State* ls, const char* key, bool value, int index=-3)
    {
        ::lua_pushstring(ls, key);
        ::lua_pushboolean(ls, value ? 1 : 0);
        ::lua_settable(ls, index);
    }
    void SetNil(lua_State* ls, int key, int index=-3)
    {
        ::lua_pushnumber(ls, key);
        ::lua_pushnil(ls);
        ::lua_settable(ls, index);
    }
    
    void GetFromTopTable(lua_State* ls, const char* key)
    {
        ::lua_pushstring(ls, key);
        ::lua_gettable(ls, -2);
    }
    void GetFromTopTable(lua_State* ls, int key)
    {
        ::lua_pushnumber(ls, key);
        ::lua_gettable(ls, -2);
    }

    //追加したテーブルか、現存するテーブルがトップに残る
    void AddTableIfNotFound(lua_State* ls, const char* key, int narr, int nrec)
    {
        GetFromTopTable(ls, key);
        if(!lua_istable(ls, -1)){
            lua_pop(ls, 1);
            ::lua_createtable(ls, narr, nrec);
            ::lua_pushstring(ls, key);
            ::lua_pushvalue(ls, -2);
            lua_settable(ls, -4);
        }
    }
    void AddTableIfNotFound(lua_State* ls, int key, int narr, int nrec)
    {
        GetFromTopTable(ls, key);
        if(!lua_istable(ls, -1)){
            lua_pop(ls, 1);
            ::lua_createtable(ls, narr, nrec);
            ::lua_pushnumber(ls, key);
            ::lua_pushvalue(ls, -2);
            lua_settable(ls, -4);
        }
    }

    void SetHitBodyFields(lua_State* ls, managed_types::HittableObject& hit_body)
    {
        SetNumber(ls, "type", hit_body.Type());
        SetNumber(ls, "x", hit_body.X());
        SetNumber(ls, "y", hit_body.Y());
        SetNumber(ls, "width", hit_body.Width());
        SetNumber(ls, "height", hit_body.Height());
        SetNumber(ls, "radius", hit_body.Radius());
        SetNumber(ls, "angle", hit_body.Angle());
    }

    void SetPlayerFields(lua_State* ls, managed_types::Player& player)
    {
        SetNumber(ls, "x", player.X());
        SetNumber(ls, "y", player.Y());
        SetNumber(ls, "life", player.Life());
        SetNumber(ls, "cardAttackLevel", player.CardAttackLevel());
        SetNumber(ls, "bossAttackLevel", player.BossAttackLevel());
        SetNumber(ls, "currentCharge", player.CurrentCharge());
        SetNumber(ls, "currentChargeMax", player.CurrentChargeMax());
        SetNumber(ls, "chargeSpeed", player.ChargeSpeed());
        SetNumber(ls, "spellPoint", player.SpellPoint());
        SetNumber(ls, "combo", player.Combo());
        SetNumber(ls, "character", player.Character());
        SetNumber(ls, "speedFast", player.SpeedFast());
        SetNumber(ls, "speedSlow", player.SpeedSlow());

        AddTableIfNotFound(ls, "hitBodyRect", 0, 7);
        SetHitBodyFields(ls, *player.HittableObjectRect());
        lua_pop(ls, 1);

        AddTableIfNotFound(ls, "hitBodyCircle", 0, 7);
        SetHitBodyFields(ls, *player.HittableObjectCircle());
        lua_pop(ls, 1);

        AddTableIfNotFound(ls, "hitBodyForItem", 0, 7);
        SetHitBodyFields(ls, *player.HittableObjectForItem());
        lua_pop(ls, 1);
    }

    void SetEnemyFields(lua_State* ls, const managed_types::Enemy& enemy)
    {
        SetNumber(ls, "id", enemy.Id());
        SetNumber(ls, "x", enemy.X());
        SetNumber(ls, "y", enemy.Y());
        SetNumber(ls, "vx", enemy.Vx());
        SetNumber(ls, "vy", enemy.Vy());
        SetBool(ls, "enabled", enemy.Enabled());
        SetBool(ls, "isSpirit", enemy.IsSpirit());
        SetBool(ls, "isActivatedSpirit", enemy.IsActivatedSpirit());
        SetBool(ls, "isBoss", enemy.IsBoss());
        SetBool(ls, "isLily", enemy.IsLily());
        SetBool(ls, "isPseudoEnemy", enemy.IsPseudoEnemy());

        AddTableIfNotFound(ls,"hitBody", 0, 7);
        SetHitBodyFields(ls, *enemy.HittableObject());
        lua_pop(ls, 1);
    }

    void SetBulletFields(lua_State* ls, const managed_types::Bullet& bullet)
    {
        SetNumber(ls, "id", bullet.Id());
        SetNumber(ls, "x", bullet.X());
        SetNumber(ls, "y", bullet.Y());
        SetNumber(ls, "vx", bullet.Vx());
        SetNumber(ls, "vy", bullet.Vy());
        SetBool(ls, "enabled", bullet.Enabled());

        AddTableIfNotFound(ls,"hitBody", 0, 7);
        SetHitBodyFields(ls, *bullet.HittableObject());
        lua_pop(ls, 1);
    }

    void SetItemFields(lua_State* ls, const managed_types::Item& item)
    {
        SetNumber(ls, "id", item.Id());
        SetNumber(ls, "x", item.X());
        SetNumber(ls, "y", item.Y());
        SetNumber(ls, "vx", item.Vx());
        SetNumber(ls, "vy", item.Vy());
        SetNumber(ls, "type", item.Type());
        SetBool(ls, "enabled", item.Enabled());

        AddTableIfNotFound(ls,"hitBody", 0, 7);
        SetHitBodyFields(ls, *item.HittableObject());
        lua_pop(ls, 1);
    }

    void SetExAttackFields(lua_State* ls, const managed_types::ExAttack& ex_attack)
    {
        SetNumber(ls, "id", ex_attack.Id());
        SetNumber(ls, "x", ex_attack.X());
        SetNumber(ls, "y", ex_attack.Y());
        SetNumber(ls, "vx", ex_attack.Vx());
        SetNumber(ls, "vy", ex_attack.Vy());
        SetNumber(ls, "type", ex_attack.Type());
        SetBool(ls, "enabled", ex_attack.Enabled());
        SetBool(ls, "hittable", ex_attack.Hittable());
        if(ex_attack.Hittable()){
            AddTableIfNotFound(ls,"hitBody", 0, 7);
            SetHitBodyFields(ls, *ex_attack.HittableObject());
            lua_pop(ls, 1);
        }
    }

    void SetGameSideFields(lua_State* ls,  managed_types::GameSide& game_side)
    {
        SetNewTable(ls, "player");
        SetNewTable(ls, "enemies");
        SetNewTable(ls, "bullets");
        SetNewTable(ls, "items");
        SetNewTable(ls, "exAttacks");
        SetNumber(ls, "round_win", game_side.RoundWin());
        SetNumber(ls, "score", game_side.Score());
    }

    void UpdatePlayer(lua_State* ls, managed_types::Player& player)
    {
        AddTableIfNotFound(ls, "player", 0, 16);
        SetPlayerFields(ls, player);
        lua_pop(ls, 1);
    }

    void UpdateEnemies(lua_State* ls, const managed_types::Enemies& enemies)
    {
        int len = enemies.size();
        AddTableIfNotFound(ls, "enemies", len, 0);
        int idx = 1;
        for(auto it=enemies.begin();it!=enemies.end();++it){
            AddTableIfNotFound(ls, idx, 0, 12);
            SetEnemyFields(ls, *it->get());
            lua_pop(ls, 1);
            idx++;
        }
        int real_len = ::lua_objlen(ls, -1);
        for(int i=real_len;i>=idx;i--){
            SetNil(ls, i);
        }
        lua_pop(ls, 1);
    }

    void UpdateBullets(lua_State* ls, const managed_types::Bullets& bullets)
    {
        int len = bullets.size();
        AddTableIfNotFound(ls, "bullets", len, 0);
        int idx = 1;
        for(auto it=bullets.begin();it!=bullets.end();++it){
            AddTableIfNotFound(ls, idx, 0, 7);
            SetBulletFields(ls, *it->get());
            lua_pop(ls, 1);
            idx++;
        }
        int real_len = ::lua_objlen(ls, -1);
        for(int i=real_len;i>=idx;i--){
            SetNil(ls, i);
        }
        lua_pop(ls, 1);
    }

    void UpdateItems(lua_State* ls, const managed_types::Items& items)
    {
        int len = items.size();
        AddTableIfNotFound(ls, "items", len, 0);
        int idx = 1;
        for(auto it=items.begin();it!=items.end();++it){
            AddTableIfNotFound(ls, idx, 0, 8);
            SetItemFields(ls, *it->get());
            lua_pop(ls, 1);
            idx++;
        }
        int real_len = ::lua_objlen(ls, -1);
        for(int i=real_len;i>=idx;i--){
            SetNil(ls, i);
        }
        lua_pop(ls, 1);
    }

    void UpdateExAttacks(lua_State* ls, const managed_types::ExAttacks& ex_attacks)
    {
        int len = ex_attacks.size();
        AddTableIfNotFound(ls, "exAttacks", len, 0);
        int idx = 1;
        for(auto it=ex_attacks.begin();it!=ex_attacks.end();++it){
            AddTableIfNotFound(ls, idx, 0, 9);
            SetExAttackFields(ls, *it->get());
            lua_pop(ls, 1);
            idx++;
        }
        int real_len = ::lua_objlen(ls, -1);
        for(int i=real_len;i>=idx;i--){
            SetNil(ls, i);
        }
        lua_pop(ls, 1);
    }

    void UpdateGameSide(lua_State* ls,  managed_types::GameSide& game_side, PlayerSide side)
    {
        int idx = side == Side_1P ? 1 : 2;
        AddTableIfNotFound(ls, idx, 0, 7);
        SetGameSideFields(ls, game_side);
        UpdatePlayer(ls, *game_side.Player());
        UpdateEnemies(ls, game_side.Enemies());
        UpdateBullets(ls, game_side.Bullets());
        UpdateItems(ls, game_side.Items());
        UpdateExAttacks(ls, game_side.ExAttacks());
        lua_pop(ls, 1);
    }

    void UpdateBothGameSides(lua_State* ls, TH9Monitor& monitor)
    {
        //retrieve game_sides[side]
        lua_getglobal(ls, "game_sides");
        if(!lua_istable(ls, -1)){
            lua_pop(ls, 1);
            ::lua_createtable(ls, 2, 0);
            ::lua_pushvalue(ls, -1);
            lua_setglobal(ls, "game_sides");
        }
        for(int i=0;i<2;i++){
            PlayerSide side = i==0 ? Side_1P : Side_2P;
            UpdateGameSide(ls, *monitor.GetGameSide(side), side);
        }
        lua_pop(ls, 1);
    }

    void ExportEnums(lua_State* ls, TH9Monitor& monitor)
    {
        ::lua_createtable(ls, 0, 20);
        ::lua_pushvalue(ls, -1);
        lua_setglobal(ls, "ExAttackType");
        SetNumber(ls, "Reimu", managed_types::Reimu);
        SetNumber(ls, "Marisa", managed_types::Marisa);
        SetNumber(ls, "Sakuya", managed_types::Sakuya);
        SetNumber(ls, "Youmu", managed_types::Youmu);
        SetNumber(ls, "Reisen", managed_types::Reisen);
        SetNumber(ls, "Cirno", managed_types::Cirno);
        SetNumber(ls, "Lyrica", managed_types::Lyrica);
        SetNumber(ls, "Merlin", managed_types::Merlin);
        SetNumber(ls, "Lunasa", managed_types::Lunasa);
        SetNumber(ls, "Mystia_Ex", managed_types::Mystia_Ex);
        SetNumber(ls, "Mystia_Charge2", managed_types::Mystia_Charge2);
        SetNumber(ls, "Mystia_Charge3", managed_types::Mystia_Charge3);
        SetNumber(ls, "Mystia_Boss1", managed_types::Mystia_Boss1);
        SetNumber(ls, "Mystia_Boss2", managed_types::Mystia_Boss2);
        SetNumber(ls, "Tewi", managed_types::Tewi);
        SetNumber(ls, "Aya", managed_types::Aya);
        SetNumber(ls, "Medicine", managed_types::Medicine);
        SetNumber(ls, "Yuuka", managed_types::Yuuka);
        SetNumber(ls, "Komachi", managed_types::Komachi);
        SetNumber(ls, "Eiki", managed_types::Eiki);
        lua_pop(ls, 1);

        ::lua_createtable(ls, 0, 4);
        ::lua_pushvalue(ls, -1);
        lua_setglobal(ls, "ItemType");
        SetNumber(ls, "G", managed_types::Item::Item_G);
        SetNumber(ls, "Bullet", managed_types::Item::Item_Bullet);
        SetNumber(ls, "Ex", managed_types::Item::Item_Ex);
        SetNumber(ls, "Score", managed_types::Item::Item_Score);
        lua_pop(ls, 1);

        ::lua_createtable(ls, 0, 3);
        ::lua_pushvalue(ls, -1);
        lua_setglobal(ls, "HitType");
        SetNumber(ls, "Rect", managed_types::Hit_Rect);
        SetNumber(ls, "Circle", managed_types::Hit_Circle);
        SetNumber(ls, "RotatableRect", managed_types::Hit_RotatableRect);
        lua_pop(ls, 1);

        ::lua_createtable(ls, 0, 16);
        ::lua_pushvalue(ls, -1);
        lua_setglobal(ls, "CharacterType");
        SetNumber(ls, "Reimu", raw_types::Reimu);
        SetNumber(ls, "Marisa", raw_types::Marisa);
        SetNumber(ls, "Sakuya", raw_types::Sakuya);
        SetNumber(ls, "Youmu", raw_types::Youmu);
        SetNumber(ls, "Reisen", raw_types::Reisen);
        SetNumber(ls, "Cirno", raw_types::Cirno);
        SetNumber(ls, "Lyrica", raw_types::Lyrica);
        SetNumber(ls, "Mystia", raw_types::Mystia);
        SetNumber(ls, "Tewi", raw_types::Tewi);
        SetNumber(ls, "Yuuka", raw_types::Yuuka);
        SetNumber(ls, "Aya", raw_types::Aya);
        SetNumber(ls, "Medicine", raw_types::Medicine);
        SetNumber(ls, "Komachi", raw_types::Komachi);
        SetNumber(ls, "Eiki", raw_types::Eiki);
        SetNumber(ls, "Merlin", raw_types::Merlin);
        SetNumber(ls, "Lunasa", raw_types::Lunasa);
        lua_pop(ls, 1);
    }

    void ExportVariables(lua_State* ls, TH9Monitor& monitor, PlayerSide player_side, const std::string &script_dir)
    {
        //毎回更新する変数についてはどうせupdateするときに存在しない変数を作るので
        //ここでもUpdateVariablesを使って変数を生成する
        UpdateVariables(ls, monitor);
        //以降更新しない変数
        lua_pushnumber(ls, player_side + 1);
        lua_setglobal(ls, "player_side");
        lua_pushnumber(ls, monitor.GetDifficulty());
        lua_setglobal(ls, "difficulty");
        lua_pushstring(ls, script_dir.c_str());
        lua_setglobal(ls, sandbox::varname_script_dir);
        ExportEnums(ls, monitor);
    }

    void UpdateVariables(lua_State* ls, TH9Monitor& monitor)
    {
        UpdateBothGameSides(ls, monitor);
        lua_pushnumber(ls, monitor.GetRound());
        lua_setglobal(ls, "round");
    }
}
