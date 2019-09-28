#include "th09types.h"
#include <memory>
#include "hitTest.h"
#pragma once
namespace ka_ai_duka{
    namespace managed_types{
        class Player
        {
        private:
            raw_types::Player &player;
            raw_types::PlayerCharacter &character;
            std::shared_ptr<HittableRect> hittable_rect;
            std::shared_ptr<HittableCircle> hittable_circle;
            std::shared_ptr<HittableRect> hittable_for_item;
        public:
            Player(raw_types::Player &player, raw_types::PlayerCharacter &character)
                : player(player), character(character)
            {
                hittable_rect = std::shared_ptr<HittableRect>(
                    new HittableRect(
                    player.position.x,
                    player.position.y,
                    player.rect_size1.x * 2,
                    player.rect_size1.y * 2));
                hittable_circle = std::shared_ptr<HittableCircle>(
                    new HittableCircle(
                    player.position.x,
                    player.position.y,
                    player.feature->radius));
                hittable_for_item = std::shared_ptr<HittableRect>(
                    new HittableRect(
                    player.position.x,
                    player.position.y,
                    player.rect_size3.x * 2,
                    player.rect_size3.y * 2));
            }
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
            bool IsNativeAI(void) const
            {
                return player.is_ai != 0;
            }
            float SpeedFast(void) const
            {
                return player.feature->speed_fast;
            }
            float SpeedSlow(void) const
            {
                return player.feature->speed_slow;
            }
            std::shared_ptr<HittableRect> HittableObjectRect() const
            {
                return hittable_rect;
            }
            std::shared_ptr<HittableCircle> HittableObjectCircle() const
            {
                return hittable_circle;
            }
            std::shared_ptr<HittableRect> HittableObjectForItem() const
            {
                return hittable_for_item;
            }
            void Update(void)
            {
                //矩形
                hittable_rect->SetX(X());
                hittable_rect->SetY(Y());
                hittable_rect->SetWidth(player.rect_size1.x * 2);
                hittable_rect->SetHeight(player.rect_size1.y * 2);
                //円
                hittable_circle->SetX(X());
                hittable_circle->SetY(Y());
                hittable_circle->SetRadius(player.feature->radius);
                //対アイテム
                hittable_for_item->SetX(X());
                hittable_for_item->SetY(Y());
                hittable_for_item->SetWidth(player.rect_size3.x * 2);
                hittable_for_item->SetHeight(player.rect_size3.y * 2);
            }
        };
    }
}