#include "Bullet.h"
namespace ka_ai_duka{
    namespace managed_types{
        void NormalBullet::Update(void)
        {
            hittable_object->SetX(X());
            hittable_object->SetY(Y());
            hittable_object->SetWidth(bullet.size.width);
            hittable_object->SetHeight(bullet.size.height);
        }

        void Laser::Update(void)
        {
            hittable_object->SetX(X());
            hittable_object->SetY(Y());
            hittable_object->SetWidth(laser.length_2 - laser.length_1);
            hittable_object->SetHeight(laser.thickness / 2);
            hittable_object->SetAngle(laser.angle);
        }
    }
}