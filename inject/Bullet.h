#pragma once
#include "th09types.h"
namespace ka_ai_duka{
    namespace managed_types{
        class Bullet
        {
        private:
            raw_types::Vector3D &position;
        public:
            Bullet(raw_types::Vector3D &position)
                : position(position)
            {}
            virtual ~Bullet(void)
            {
            }
            float X() const
            {
                return position.x;
            }
            float Y() const
            {
                return position.y;
            }
            virtual float Vx() const = 0;
            virtual float Vy() const = 0;
            virtual bool Enabled() const = 0;
            //TODO: “–‚½‚è”»’èŽæ“¾
        };

        class NormalBullet : public Bullet
        {
        private:
            raw_types::Bullet &bullet;
        public:
            NormalBullet(raw_types::Bullet &bullet)
                : Bullet(bullet.position), bullet(bullet)
            {
            }
            virtual ~NormalBullet(void)
            {
            }
            float Vx() const
            {
                return bullet.velocity.x;
            }
            float Vy() const
            {
                return bullet.velocity.y;
            }
            bool Enabled() const
            {
                int status = bullet.status & 0xffff;
                return (status != 0) && (status != 6);
            }
        };

        class Laser : public Bullet
        {
        private:
            raw_types::Laser &laser;
        public:
            Laser(raw_types::Laser &laser)
                : Bullet(laser.position), laser(laser)
            {
            }
            virtual ~Laser(void)
            {
            }
            float Vx() const
            {
                return 0.0f;
            }
            float Vy() const
            {
                return 0.0f;
            }
            bool Enabled() const
            {
                return laser.enabled != 0;
            }
        };
    }
}