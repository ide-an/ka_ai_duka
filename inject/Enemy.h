#pragma once
#include "th09types.h"
#include "hitTest.h"
#include <boost\shared_ptr.hpp>

namespace ka_ai_duka{
    namespace managed_types{
        class Enemy
        {
        private:
            raw_types::Enemy &enemy;
            boost::shared_ptr<HittableRect> hittable_object;
        public:
            Enemy(raw_types::Enemy &enemy)
                : enemy(enemy)
            {
                hittable_object = boost::shared_ptr<HittableRect>(new HittableRect(
                    enemy.position.x,
                    enemy.position.y,
                    enemy.size.width,
                    enemy.size.height
                    ));
            }
            virtual ~Enemy(void)
            {
            }
            float X() const
            {
                return enemy.position.x;
            }
            float Y() const
            {
                return enemy.position.y;
            }
            //TODO: x,VyÇÃíËã`ÇÕ.cppÇ…éùÇ¡ÇƒÇ¢Ç¡ÇΩÇŸÇ§Ç™ÇÊÇ≥Ç∞
            float Vx() const
            {
                float vx = enemy.velocity.x;
                if(IsActivatedSpirit()){
                    vx = enemy.velocity2.x;
                }
                if(enemy.status & 0x8000){//reversed
                    vx *= -1;
                }
                if(IsBoss()){
                    //TODO: Ç≥Ç≠Ç‚Ç≥ÇÒï‚ê≥
                }
                return vx;
            }
            float Vy() const
            {
                float vy = enemy.velocity.y;
                if(IsActivatedSpirit()){
                    vy = enemy.velocity2.y;
                }
                if(IsBoss()){
                    //TODO: Ç≥Ç≠Ç‚Ç≥ÇÒï‚ê≥
                }
                return vy;
            }
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
                return (enemy.status & 0x00002400) != 0;
            }
            //íeÇê›íuÇ∑ÇÈÇæÇØÇ≈ìñÇΩÇËîªíËÇéùÇΩÇ»Ç¢Ç‡ÇÃÇ»Ç«
            bool IsPseudoEnemy() const
            {
                return (enemy.status & 0x00000010) != 0;
            }
            boost::shared_ptr<HittableObject> GetHittableObject() const
            {

                return boost::static_pointer_cast<HittableObject>(hittable_object);
            }
            void Update(void);/
        };
    }
}