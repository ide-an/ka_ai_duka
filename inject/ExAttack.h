#pragma once
#include "th09types.h"
#include <boost\shared_ptr.hpp>
#include "hitTest.h"
namespace ka_ai_duka {
    namespace managed_types{
        enum ExAttackType{
            Reimu = 0,
            Marisa
            //TODO: more types
        };
        class ExAttack{
        private:
            unsigned int id;
            raw_types::ExAttack &ex_attack;
            ExAttackType ex_type;
        public:
            ExAttack(raw_types::ExAttack &ex_attack, unsigned int id, ExAttackType ex_type)
                : ex_attack(ex_attack), id(id), ex_type(ex_type)
            {
            }
            virtual ~ExAttack(void)
            {
            }
            unsigned int Id() const
            {
                return id;
            }
            virtual ExAttackType Type() const
            {
                return ex_type;
            }
            virtual float X() const = 0;//TODO: we should know player_side if ex_attack.feature->is_local is false.
            virtual float Y() const = 0;
            virtual float Vx() const
            {
                return ex_attack.feature->velocity.x;
            }
            virtual float Vy() const
            {
                return ex_attack.feature->velocity.y;
            }
            bool Enabled() const
            {
                return IsEnabled(ex_attack);
            }
            static bool IsEnabled(raw_types::ExAttack &ex_attack)
            {
                return ex_attack.enabled != 0;
            }
            virtual void Update(void) = 0;
            virtual boost::shared_ptr<managed_types::HittableObject> HittableObject(void) const = 0;
        };
        //TODO: derived classes
    }
}