#pragma once
#include "th09types.h"

namespace ka_ai_duka{
    namespace managed_types{
        class Item{
        private:
            unsigned int id;
            raw_types::Item &item;
            //TODO: “–‚½‚è”»’è
        public:
            Item(raw_types::Item &item, unsigned int id)
                : item(item), id(id)
            {
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
            void Update(void)
            {
                //TODO: implement
            }
        };
    }
}