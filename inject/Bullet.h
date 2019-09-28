#pragma once
#include "th09types.h"
#include <memory>
#include "hitTest.h"
namespace ka_ai_duka{
    namespace managed_types{
        class Bullet
        {
        private:
            unsigned int id;
            raw_types::Vector3D &position;
        public:
            Bullet(raw_types::Vector3D &position, unsigned int id)
                : position(position), id(id)
            {}
            virtual ~Bullet(void)
            {
            }
            unsigned int Id() const
            {
                return id;
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
            virtual bool IsErasable() const = 0;
            virtual void Update() = 0;
            virtual std::shared_ptr<managed_types::HittableObject> HittableObject() const = 0;
        };

        class NormalBullet : public Bullet
        {
        private:
            raw_types::Bullet &bullet;
            std::shared_ptr<HittableRect> hittable_object;
        public:
            NormalBullet(raw_types::Bullet &bullet, unsigned int id)
                : Bullet(bullet.position, id), bullet(bullet)
            {
                hittable_object = std::shared_ptr<HittableRect>(
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
            bool IsErasable() const
            {
                return bullet.bullet_type == 0;
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
            std::shared_ptr<managed_types::HittableObject> HittableObject() const
            {
                return std::static_pointer_cast<managed_types::HittableObject>(hittable_object);
            }
            void Update(void);
        };

        class Laser : public Bullet
        {
        private:
            raw_types::Laser &laser;
            std::shared_ptr<HittableRotatableRect> hittable_object;
        public:
            Laser(raw_types::Laser &laser, unsigned int id)
                : Bullet(laser.position, id), laser(laser)
            {
                hittable_object = std::shared_ptr<HittableRotatableRect>(
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
            bool IsErasable() const
            {
                return false;
            }
            bool Enabled() const
            {
                return IsEnabled(laser);
            }
            static bool IsEnabled(raw_types::Laser &laser)
            {
                return laser.enabled != 0;
            }
            std::shared_ptr<managed_types::HittableObject> HittableObject() const
            {
                return std::static_pointer_cast<managed_types::HittableObject>(hittable_object);
            }
            void Update(void);
        };
    }
}