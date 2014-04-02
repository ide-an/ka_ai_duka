#pragma once
#include "th09types.h"
#include <boost\shared_ptr.hpp>
#include "hitTest.h"
#include "player_side.h"
#include "idgen.h"
#include <vector>
namespace ka_ai_duka {
    namespace managed_types{
        enum ExAttackType{
            Reimu = 0,
            Marisa,
            Sakuya,// Sakuya2 is ignored
            Youmu,
            Reisen,
            Cirno,
            Lyrica,
            Merlin,
            Lunasa,
            Mystia_Ex,
            Mystia_Charge2,
            Mystia_Charge3,
            Mystia_Boss1,
            Mystia_Boss2,
            Tewi,
            Aya,
            Medicine,
            Yuuka,
            Komachi,
            Eiki
        };
        class ExAttack{
        private:
            unsigned int id;
            ExAttackType ex_type;
        protected:
            raw_types::ExAttack &ex_attack;
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
            virtual float X() const;
            virtual float Y() const;
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
            //TODO: 当たり判定が有効かどうかのメソッド
            static PlayerSide GetPlayerSide(raw_types::ExAttack &ex_attack)
            {
                return ex_attack.is_2P ? Side_2P : Side_1P;
            }
            virtual void Update(void) = 0;
            virtual boost::shared_ptr<managed_types::HittableObject> HittableObject(void) const = 0;
        };
        //TODO: 円形のEXアタック一般を表すクラスを用意した方がシンプル?
        class ExAttackReimu : public ExAttack
        {
        private:
            const float radius;
            boost::shared_ptr<HittableCircle> hittable_object;
        public:
            ExAttackReimu(raw_types::ExAttack &ex_attack, unsigned int id)
                : ExAttack(ex_attack, id, Reimu), radius(18.0f)
            {
                hittable_object = boost::shared_ptr<HittableCircle>(new HittableCircle(X(), Y(), radius));
            }
            void Update(void);
            boost::shared_ptr<managed_types::HittableObject> HittableObject(void) const
            {
                return boost::static_pointer_cast<managed_types::HittableObject>(hittable_object);
            }
        };
        class ExAttackMarisa : public ExAttack
        {
        private:
            boost::shared_ptr<HittableCircle> hittable_object;
        public:
            ExAttackMarisa(raw_types::ExAttack &ex_attack, unsigned int id)
                : ExAttack(ex_attack, id, Marisa)
            {
                hittable_object = boost::shared_ptr<HittableCircle>(new HittableCircle(X(), Y(), 0));
            }
            void Update(void);
            boost::shared_ptr<managed_types::HittableObject> HittableObject(void) const
            {
                return boost::static_pointer_cast<managed_types::HittableObject>(hittable_object);
            }
        };
        class ExAttackSakuya : public ExAttack
        {
        private:
            const float radius;
            boost::shared_ptr<HittableCircle> hittable_object;
            unsigned int index;
        public:
            ExAttackSakuya(raw_types::ExAttack &ex_attack, unsigned int id, unsigned int index)
                : ExAttack(ex_attack, id, Sakuya), index(index), radius(12.0f)
            {
                hittable_object = boost::shared_ptr<HittableCircle>(new HittableCircle(X(), Y(), radius));
            }
            float X() const;//咲夜さんのEXは1つのあれから5個の弾が生まれる
            float Y() const;
            void Update(void);
            boost::shared_ptr<managed_types::HittableObject> HittableObject(void) const
            {
                return boost::static_pointer_cast<managed_types::HittableObject>(hittable_object);
            }
        };
        class ExAttackYoumu : public ExAttack
        {
        private:
            const float radius;
            boost::shared_ptr<HittableCircle> hittable_object;
        public:
            ExAttackYoumu(raw_types::ExAttack &ex_attack, unsigned int id)
                : ExAttack(ex_attack, id, Youmu), radius(14.0f)
            {
                hittable_object = boost::shared_ptr<HittableCircle>(new HittableCircle(X(), Y(), radius));
            }
            void Update(void);
            boost::shared_ptr<managed_types::HittableObject> HittableObject(void) const
            {
                return boost::static_pointer_cast<managed_types::HittableObject>(hittable_object);
            }
        };
        //TODO: derived classes
        bool ExAttackFactory(
            raw_types::ExAttack &ex_attack,
            raw_types::ExAttackFuncAddr &ex_func_addr,
            IDGenerator &idgen,
            std::vector<boost::shared_ptr<ExAttack> > &attacks
            );
    }
}