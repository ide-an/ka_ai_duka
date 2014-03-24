#include "th09types.h"
#pragma once
namespace ka_ai_duka{
    namespace managed_types{
        class Player
        {
        private:
            raw_types::Player &player;
            raw_types::PlayerCharacter &character;
        public:
            Player(raw_types::Player &player, raw_types::PlayerCharacter &character)
                : player(player), character(character)
            {}
            ~Player(void)
            {
            }
            float X() const
            {
                return player.position.x;
            }
            float Y() const
            {
                return player.position.y;
            }
            int Life() const
            {
                return player.life;
            }
            int CardAttackLevel() const
            {
                return player.card_attack_level;
            }
            int BossAttackLevel() const
            {
                return player.boss_card_attack_level;
            }
            float CurrentCharge() const
            {
                return player.charge_current;
            }
            float CurrentChargeMax() const
            {
                return player.charge_max;
            }
            float ChargeSpeed() const
            {
                return player.feature->charge_speed;
            }
            int Combo() const
            {
                return player.combo;
            }
            int SpellPoint() const
            {
                return player.spell_point;
            }
            raw_types::PlayerCharacter Character() const
            {
                return character;
            }
            //TODO: “–‚½‚è”»’è
            //TODO: ‘¬“x
        };
    }
}