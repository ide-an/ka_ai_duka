#pragma once
#include "th09types.h"
#include <boost\shared_ptr.hpp>
#include "hitTest.h"
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
            virtual void Update() = 0;
            virtual boost::shared_ptr<managed_types::HittableObject> HittableObject() const = 0;
        };

        class NormalBullet : public Bullet
        {
        private:
            raw_types::Bullet &bullet;
            boost::shared_ptr<HittableRect> hittable_object;
        public:
            NormalBullet(raw_types::Bullet &bullet)
                : Bullet(bullet.position), bullet(bullet)
            {
                hittable_object = boost::shared_ptr<HittableRect>(
                    new HittableRect(
                    bullet.position.x,
                    bullet.position.y,
                    bullet.size.width,
                    bullet.size.height
                    ));
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
                return IsEnabled(bullet);
            }
            static bool IsEnabled(raw_types::Bullet &bullet)
            {
                int status = bullet.status & 0xffff;
                return (status != 0) && (status != 6);
            }
            boost::shared_ptr<managed_types::HittableObject> HittableObject() const
            {
                return boost::static_pointer_cast<managed_types::HittableObject>(hittable_object);
            }
            void Update(void);
        };

        class Laser : public Bullet
        {
        private:
            raw_types::Laser &laser;
            boost::shared_ptr<HittableRotatableRect> hittable_object;
        public:
            Laser(raw_types::Laser &laser)
                : Bullet(laser.position), laser(laser)
            {
                hittable_object = boost::shared_ptr<HittableRotatableRect>(
                    new HittableRotatableRect(
                    laser.position.x,
                    laser.position.y,
                    laser.thickness * 0.5f,
                    laser.length_2 - laser.length_1,
                    laser.angle
                    ));
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
                return IsEnabled(laser);
            }
            static bool IsEnabled(raw_types::Laser &laser)
            {
                return laser.enabled != 0;
            }
            boost::shared_ptr<managed_types::HittableObject> HittableObject() const
            {
                return boost::static_pointer_cast<managed_types::HittableObject>(hittable_object);
            }
            void Update(void);
        };
    }
}