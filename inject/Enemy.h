#pragma once
#include "th09types.h"
#include "hitTest.h"
#include <memory>

namespace ka_ai_duka{
    namespace managed_types{
        class Enemy
        {
        private:
            unsigned int id;
            raw_types::Enemy &enemy;
            std::shared_ptr<HittableRect> hittable_object;
        public:
            Enemy(raw_types::Enemy &enemy, unsigned int id)
                : enemy(enemy), id(id)
            {
                hittable_object = std::shared_ptr<HittableRect>(new HittableRect(
                    enemy.position.x,
                    enemy.position.y,
                    enemy.size.width,
                    enemy.size.height
                    ));
            }
            virtual ~Enemy(void)
            {
            }
            unsigned int Id() const
            {
                return id;
            }
            float X() const
            {
                return enemy.position.x;
            }
            float Y() const
            {
                return enemy.position.y;
            }
            float Vx() const;
            float Vy() const;
            bool Enabled() const
            {
                return IsEnabled(enemy);
            }
            static bool IsEnabled(raw_types::Enemy &enemy)
            {
                return (enemy.status & 0x101) == 1;
            }
            bool IsSpirit() const
            {
                return (enemy.status & 0x00400000) != 0;
            }
            bool IsActivatedSpirit() const
            {
                return IsSpirit() && (enemy.status2 & 0x1000);
            }
            bool IsBoss() const
            {
                return (enemy.status & 0x80000000) != 0;
            }
            bool IsLily() const
            {
                return ((enemy.status & 0x00002400) != 0) && !IsBoss();
            }
            //’e‚ğİ’u‚·‚é‚¾‚¯‚Å“–‚½‚è”»’è‚ğ‚½‚È‚¢‚à‚Ì‚È‚Ç
            bool IsPseudoEnemy() const
            {
                return (enemy.status & 0x00000010) != 0;
            }
            std::shared_ptr<HittableObject> HittableObject() const
            {
                return std::static_pointer_cast<managed_types::HittableObject>(hittable_object);
            }
            void Update(void);
        };
    }
}