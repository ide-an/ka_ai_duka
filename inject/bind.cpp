#include "bind.h"
#include "GameSide.h"
#include "sandbox.h"
#include <luabind\luabind.hpp>
#include <luabind\iterator_policy.hpp>
#include <luabind\operator.hpp>
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

    void BindToLua(::lua_State* ls, PlayerSide player_side)
    {
        luabind::open(ls);
        luabind::module(ls)[
            luabind::def("sendKeys", (player_side == Side_1P) ? SendKeys1P : SendKeys2P),
             //�����蔻��
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
                            //luabind doc 8.7�߂��Q�ƁBshared_ptr�ɂ͊��N���X���w�肷��
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
            .property("speedFast", &managed_types::Player::SpeedFast)
            .property("speedSlow", &managed_types::Player::SpeedSlow)
            .property("hitBodyRect", &managed_types::Player::HittableObjectRect)
            .property("hitBodyCircle", &managed_types::Player::HittableObjectCircle)
            .property("hitBodyForItem", &managed_types::Player::HittableObjectForItem)
            ,
            luabind::class_<managed_types::Enemy, boost::shared_ptr<managed_types::Enemy> >("Enemy")
            .property("id", &managed_types::Enemy::Id)
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
            .property("id", &managed_types::Bullet::Id)
            .property("x", &managed_types::Bullet::X)
            .property("y", &managed_types::Bullet::Y)
            .property("vx", &managed_types::Bullet::Vx)
            .property("vy", &managed_types::Bullet::Vy)
            .property("enabled", &managed_types::Bullet::Enabled)
            .property("hitBody", &managed_types::Bullet::HittableObject)
            ,
            luabind::class_<managed_types::Item, boost::shared_ptr<managed_types::Item> >("Item")
            .enum_("ItemType")[
                luabind::value("ITEM_G", managed_types::Item::Item_G),
                luabind::value("ITEM_BULLET", managed_types::Item::Item_Bullet),
                luabind::value("ITEM_EX", managed_types::Item::Item_Ex),
                luabind::value("ITEM_SCORE", managed_types::Item::Item_Score)
            ]
            .property("id", &managed_types::Item::Id)
            .property("x", &managed_types::Item::X)
            .property("y", &managed_types::Item::Y)
            .property("vx", &managed_types::Item::Vx)
            .property("vy", &managed_types::Item::Vy)
            .property("type", &managed_types::Item::Type)
            .property("enabled", &managed_types::Item::Enabled)
            .property("hitBody", &managed_types::Item::HittableObject)
            ,
            luabind::class_<managed_types::ExAttack>("ExAttack")
            .enum_("ExAttackType")[
                luabind::value("ITEM_Reimu", managed_types::Reimu),
                luabind::value("ITEM_Marisa", managed_types::Marisa),
                luabind::value("ITEM_Sakuya", managed_types::Sakuya),
                luabind::value("ITEM_Youmu", managed_types::Youmu),
                luabind::value("ITEM_Reisen", managed_types::Reisen),
                luabind::value("ITEM_Cirno", managed_types::Cirno),
                luabind::value("ITEM_Lyrica", managed_types::Lyrica),
                luabind::value("ITEM_Merlin", managed_types::Merlin),
                luabind::value("ITEM_Lunasa", managed_types::Lunasa),
                luabind::value("ITEM_Mystia_Ex", managed_types::Mystia_Ex),
                luabind::value("ITEM_Mystia_Charge2", managed_types::Mystia_Charge2),
                luabind::value("ITEM_Mystia_Charge3", managed_types::Mystia_Charge3),
                luabind::value("ITEM_Mystia_Boss1", managed_types::Mystia_Boss1),
                luabind::value("ITEM_Mystia_Boss2", managed_types::Mystia_Boss2),
                luabind::value("ITEM_Tewi", managed_types::Tewi),
                luabind::value("ITEM_Aya", managed_types::Aya),
                luabind::value("ITEM_Medicine", managed_types::Medicine),
                luabind::value("ITEM_Yuuka", managed_types::Yuuka),
                luabind::value("ITEM_Komachi", managed_types::Komachi),
                luabind::value("ITEM_Eiki", managed_types::Eiki)
            ]
            .property("id", &managed_types::ExAttack::Id)
            .property("x", &managed_types::ExAttack::X)
            .property("y", &managed_types::ExAttack::Y)
            .property("vx", &managed_types::ExAttack::Vx)
            .property("vy", &managed_types::ExAttack::Vy)
            .property("type", &managed_types::ExAttack::Type)
            .property("enabled", &managed_types::ExAttack::Enabled)
            .property("hitBody", &managed_types::ExAttack::HittableObject)
            .property("hittable", &managed_types::ExAttack::Hittable)
            ,
            luabind::class_<managed_types::GameSide>("GameSide")
            .property("player", &managed_types::GameSide::Player)
            .property("enemies", &managed_types::GameSide::Enemies, luabind::return_stl_iterator)
            .property("bullets", &managed_types::GameSide::Bullets, luabind::return_stl_iterator)
            .property("items", &managed_types::GameSide::Items, luabind::return_stl_iterator)
            .property("exAttacks", &managed_types::GameSide::ExAttacks, luabind::return_stl_iterator)
            .property("round_win", &managed_types::GameSide::RoundWin)
            .property("score", &managed_types::GameSide::Score)
        ];
    }

    void ExportVariables(lua_State* ls, TH9Monitor& monitor, PlayerSide player_side, const std::string &script_dir)
    {
        auto game_sides = luabind::newtable(ls);
        game_sides[1] = luabind::object(ls, monitor.GetGameSide(Side_1P));
        game_sides[2] = luabind::object(ls, monitor.GetGameSide(Side_2P));
        luabind::globals(ls)["game_sides"] = game_sides;
        luabind::globals(ls)["player_side"] = player_side + 1;
        luabind::globals(ls)["round"] = monitor.GetRound();
        luabind::globals(ls)["difficulty"] = monitor.GetDifficulty();
        luabind::globals(ls)[sandbox::varname_script_dir] = script_dir.c_str();
    }

    void UpdateVariables(lua_State* ls, TH9Monitor& monitor)
    {
        luabind::globals(ls)[sandbox::env_name]["round"] = monitor.GetRound();
    }
}
