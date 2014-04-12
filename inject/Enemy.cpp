#include "Enemy.h"
namespace ka_ai_duka{
    namespace managed_types{
        static bool ShouldAdjust(raw_types::Enemy &enemy)
        {
            return (enemy.status & 0x10000) != 0;
        }
        float Enemy::Vx() const
        {
            float vx = enemy.velocity.x;
            if(IsActivatedSpirit()){
                vx = enemy.velocity2.x;
            }
            if(enemy.status & 0x8000){//reversed
                vx *= -1;
            }
            if(IsBoss() && ShouldAdjust(enemy)){
                float new_x = X() + vx;
                if(new_x < enemy.appear_area.ru.x){
                    vx = enemy.appear_area.ru.x - new_x;
                }else if(enemy.appear_area.ld.x < new_x){
                    vx = enemy.appear_area.ld.x - new_x;
                }
            }
            return vx;
        }
        float  Enemy::Vy() const
        {
            float vy = enemy.velocity.y;
            if(IsActivatedSpirit()){
                vy = enemy.velocity2.y;
            }
            if(IsBoss() && ShouldAdjust(enemy)){
                float new_y = Y() + vy;
                if(new_y < enemy.appear_area.ru.y){
                    vy = enemy.appear_area.ru.y - new_y;
                }else if(enemy.appear_area.ld.y < new_y){
                    vy = enemy.appear_area.ld.y - new_y;
                }
            }
            return vy;
        }
        void Enemy::Update(void)
        {
            hittable_object->SetX(X());
            hittable_object->SetY(Y());
            hittable_object->SetWidth(enemy.size.width);
            hittable_object->SetHeight(enemy.size.height);
        }
    }
}