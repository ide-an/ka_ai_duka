#include "Enemy.h"
namespace ka_ai_duka{
    namespace managed_types{
        float Enemy::Vx() const
        {
            float vx = enemy.velocity.x;
            if(IsActivatedSpirit()){
                vx = enemy.velocity2.x;
            }
            if(enemy.status & 0x8000){//reversed
                vx *= -1;
            }
            if(IsBoss()){
                //TODO: ‚³‚­‚â‚³‚ñ•â³
            }
            return vx;
        }
        float  Enemy::Vy() const
        {
            float vy = enemy.velocity.y;
            if(IsActivatedSpirit()){
                vy = enemy.velocity2.y;
            }
            if(IsBoss()){
                //TODO: ‚³‚­‚â‚³‚ñ•â³
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