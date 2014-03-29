#include "bind.h"
#include "GameSide.h"
#include <luabind\luabind.hpp>
#include <luabind\iterator_policy.hpp>
#include <luabind\operator.hpp>
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
             //当たり判定
            luabind::def("hitTest", &managed_types::HitTest),
            luabind::class_<managed_types::HittableObject, boost::shared_ptr<managed_types::HittableObject> >("HittableObject")
            .def(luabind::tostring(luabind::self))
            .enum_("HitType")[
                luabind::value("HIT_RECT", managed_types::Hit_Rect),
                luabind::value("HIT_CIRCLE", managed_types::Hit_Circle),
                luabind::value("HIT_ROTATABLE_RECT", managed_types::Hit_RotatableRect)
            ]
            .property("type", &managed_types::HittableObject::Type)
            .property("x", &managed_types::HittableObject::X)
            .property("y", &managed_types::HittableObject::Y)
            .property("width", &managed_types::HittableObject::Width)
            .property("height", &managed_types::HittableObject::Height)
            .property("radius", &managed_types::HittableObject::Radius)
            .property("angle", &managed_types::HittableObject::Angle)
            ,
            luabind::class_<managed_types::HittableRect,
                            luabind::bases<managed_types::HittableObject>,
                            //luabind doc 8.7節を参照。shared_ptrには基底クラスを指定する
                            boost::shared_ptr<managed_types::HittableObject> >("HittableRect")
            .def(luabind::constructor<float,float,float,float>())
            .property("type", &managed_types::HittableRect::Type)
            .property("x", &managed_types::HittableRect::X)
            .property("y", &managed_types::HittableRect::Y)
            .property("width", &managed_types::HittableRect::Width)
            .property("height", &managed_types::HittableRect::Height)
            .property("radius", &managed_types::HittableRect::Radius)
            .property("angle", &managed_types::HittableRect::Angle)
            ,
            luabind::class_<managed_types::HittableCircle,
                            luabind::bases<managed_types::HittableObject>,
                            boost::shared_ptr<managed_types::HittableObject> >("HittableCircle")
            .def(luabind::constructor<float,float,float>())
            .property("type", &managed_types::HittableRect::Type)
            .property("x", &managed_types::HittableCircle::X)
            .property("y", &managed_types::HittableCircle::Y)
            .property("width", &managed_types::HittableCircle::Width)
            .property("height", &managed_types::HittableCircle::Height)
            .property("radius", &managed_types::HittableCircle::Radius)
            .property("angle", &managed_types::HittableCircle::Angle)
            ,
            luabind::class_<managed_types::HittableRotatableRect,
                            luabind::bases<managed_types::HittableObject>,
                            boost::shared_ptr<managed_types::HittableObject> >("HittableRotatableRect")
            .def(luabind::constructor<float,float,float,float,float>())
            .property("type", &managed_types::HittableRotatableRect::Type)
            .property("x", &managed_types::HittableRotatableRect::X)
            .property("y", &managed_types::HittableRotatableRect::Y)
            .property("width", &managed_types::HittableRotatableRect::Width)
            .property("height", &managed_types::HittableRotatableRect::Height)
            .property("radius", &managed_types::HittableRotatableRect::Radius)
            .property("angle", &managed_types::HittableRotatableRect::Angle)
            ,
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
            .property("hitBody", &managed_types::Enemy::HittableObject)
            .def("isSpirit", &managed_types::Enemy::IsSpirit)
            .def("isActivatedSpirit", &managed_types::Enemy::IsActivatedSpirit)
            .def("isBoss", &managed_types::Enemy::IsBoss)
            .def("isLily", &managed_types::Enemy::IsLily)
            .def("isPseudoEnemy", &managed_types::Enemy::IsPseudoEnemy)
            ,
            luabind::class_<managed_types::Bullet, boost::shared_ptr<managed_types::Bullet> >("Bullet")
            .property("x", &managed_types::Bullet::X)
            .property("y", &managed_types::Bullet::Y)
            .property("vx", &managed_types::Bullet::Vx)
            .property("vy", &managed_types::Bullet::Vy)
            .property("enabled", &managed_types::Bullet::Enabled)
            .property("hitBody", &managed_types::Bullet::HittableObject)
            ,
            //TODO: Ex attack
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