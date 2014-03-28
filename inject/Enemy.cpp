#include "Enemy.h"
namespace ka_ai_duka{
    namespace managed_types{
        void Enemy::Update(void)
        {
            hittable_object->SetX(X());
            hittable_object->SetY(Y());
            //TODO: “–‚½‚è”»’èXV
        }
    }
}