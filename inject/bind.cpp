#include "bind.h"
#include "GameSide.h"
#include <luabind\luabind.hpp>
#include <luabind\iterator_policy.hpp>
#include <boost\shared_ptr.hpp>

namespace ka_ai_duka{
    

    //単に7要素の配列を受け取るだけでいい気もする
    void SendKeys1P(KeyState key_state)
    {
        //TODO: まともな実装
        monitor->SetKeyState(Side_1P, key_state);
    }

    void SendKeys2P(KeyState key_state)
    {
        monitor->SetKeyState(Side_2P, key_state);
    }

    void BindToLua(::lua_State* ls, PlayerSide player_side)
    {
        luabind::open(ls);
        luabind::module(ls)[
            luabind::def("sendKeys", (player_side == Side_1P) ? SendKeys1P : SendKeys2P),
            luabind::class_<managed_types::Player>("Player")
            .property("x", &managed_types::Player::X)
            .property("y", &managed_types::Player::Y)
            .property("life", &managed_types::Player::Life)
            .property("cartAttackLevel", &managed_types::Player::CardAttackLevel)
            .property("bossAttackLevel", &managed_types::Player::BossAttackLevel)
            .property("currentCharge", &managed_types::Player::CurrentCharge)
            .property("currentChargeMax", &managed_types::Player::CurrentChargeMax)
            .property("chargeSpeed", &managed_types::Player::ChargeSpeed)
            .property("spellPoint", &managed_types::Player::SpellPoint)
            .property("combo", &managed_types::Player::Combo)
            .property("character", &managed_types::Player::Character)
            //TODO: 速度, 当たり判定
            ,
            luabind::class_<managed_types::Enemy, boost::shared_ptr<managed_types::Enemy> >("Enemy")
            .property("x", &managed_types::Enemy::X)
            .property("y", &managed_types::Enemy::Y)
            .property("vx", &managed_types::Enemy::Vx)
            .property("vy", &managed_types::Enemy::Vy)
            .property("enabled", &managed_types::Enemy::Enabled)
            .def("isSpirit", &managed_types::Enemy::IsSpirit)
            .def("isActivatedSpirit", &managed_types::Enemy::IsActivatedSpirit)
            .def("isBoss", &managed_types::Enemy::IsBoss)
            .def("isLily", &managed_types::Enemy::IsLily)
            .def("isPseudoEnemy", &managed_types::Enemy::IsPseudoEnemy)
            //TODO: 当たり判定
            ,
            luabind::class_<managed_types::Bullet, boost::shared_ptr<managed_types::Bullet> >("Bullet")
            .property("x", &managed_types::Bullet::X)
            .property("y", &managed_types::Bullet::Y)
            .property("vx", &managed_types::Bullet::Vx)
            .property("vy", &managed_types::Bullet::Vy)
            .property("enabled", &managed_types::Bullet::Enabled)
            //TODO: 当たり判定
            ,
            //TODO: Ex attack
            //TODO: std::vectorをラップするか?
            luabind::class_<managed_types::GameSide>("GameSide")
            .property("player", &managed_types::GameSide::Player)
            .property("enemies", &managed_types::GameSide::Enemies, luabind::return_stl_iterator)
            .property("bullets", &managed_types::GameSide::Bullets, luabind::return_stl_iterator)
        ];
    }

    void ExportVariables(lua_State* ls, TH9Monitor& monitor, PlayerSide player_side)
    {
        auto game_sides = luabind::newtable(ls);
        game_sides[1] = luabind::object(ls, monitor.GetGameSide(Side_1P));
        game_sides[2] = luabind::object(ls, monitor.GetGameSide(Side_2P));
        luabind::globals(ls)["game_sides"] = game_sides;
        luabind::globals(ls)["player_side"] = player_side + 1;
    }

}