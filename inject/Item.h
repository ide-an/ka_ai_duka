#pragma once
#include "th09types.h"
#include <memory>
#include "hitTest.h"
namespace ka_ai_duka{
    namespace managed_types{
        class Item{
        private:
            unsigned int id;
            raw_types::Item &item;
            float &size;
            std::shared_ptr<HittableRect> hittable_object;
        public:
            Item(raw_types::Item &item, unsigned int id, float &size)
                : item(item), id(id), size(size)
            {
                hittable_object = std::shared_ptr<HittableRect>(
                    new HittableRect(
                    item.position.x,
                    item.position.y,
                    size,
                    size
                    ));
            }
            enum ItemType{
                Item_G = 0,
                Item_Bullet,
                Item_Ex,
                Item_Score
            };
            unsigned int Id() const
            {
                return id;
            }
            float X() const
            {
                return item.position.x;
            }
            float Y() const
            {
                return item.position.y;
            }
            float Vx() const
            {
                return item.velocity.x;
            }
            float Vy() const
            {
                return item.velocity.y;
            }
            ItemType Type() const
            {
                return (ItemType) item.type;
            }
            bool Enabled() const
            {
                return IsEnabled(item);
            }
            static bool IsEnabled(raw_types::Item &item)
            {
                return item.enabled != 0;
            }
            std::shared_ptr<managed_types::HittableObject> HittableObject() const
            {
                return std::static_pointer_cast<managed_types::HittableObject>(hittable_object);
            }
            void Update(void)
            {
                hittable_object->SetX(X());
                hittable_object->SetY(Y());
                hittable_object->SetHeight(size);
                hittable_object->SetWidth(size);
            }
        };
    }
}