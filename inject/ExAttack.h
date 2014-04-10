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
            virtual bool Hittable(void) const
            {
                return ex_attack.feature->is_local > 0;
            }
            static PlayerSide GetPlayerSide(raw_types::ExAttack &ex_attack)
            {
                return ex_attack.is_2P ? Side_2P : Side_1P;
            }
            virtual void Update(void) = 0;
            virtual boost::shared_ptr<managed_types::HittableObject> HittableObject(void) const = 0;
        };

        class ExAttackCircle : public ExAttack
        {
        protected:
            float radius;
            boost::shared_ptr<HittableCircle> hittable_object;
        public:
            ExAttackCircle(raw_types::ExAttack &ex_attack, unsigned int id, ExAttackType ex_type, float radius)
                : ExAttack(ex_attack, id, ex_type), radius(radius)
            {
                hittable_object = boost::shared_ptr<HittableCircle>(new HittableCircle(X(), Y(), radius));
            }
            virtual void Update(void);
            boost::shared_ptr<managed_types::HittableObject> HittableObject(void) const
            {
                return boost::static_pointer_cast<managed_types::HittableObject>(hittable_object);
            }
        };

        class ExAttackUnhittable : public ExAttack
        {
        public:
            ExAttackUnhittable(raw_types::ExAttack &ex_attack, unsigned int id, ExAttackType ex_type)
                : ExAttack(ex_attack, id, ex_type)
            {
            }
            virtual bool Hittable(void) const
            {
                return false;
            }
            virtual void Update(void)
            {
            }
            boost::shared_ptr<managed_types::HittableObject> HittableObject(void) const
            {
                return boost::shared_ptr<managed_types::HittableObject>(nullptr);
            }
        };

        //キャラごとに特化したクラス
        class ExAttackSakuya : public ExAttackCircle
        {
        private:
            unsigned int index;
        public:
            ExAttackSakuya(raw_types::ExAttack &ex_attack, unsigned int id, unsigned int index)
                : ExAttackCircle(ex_attack, id, Sakuya, 12.0f), index(index)
            {
            }
            float X() const;//咲夜さんのEXは1つのあれから5個の弾が生まれる
            float Y() const;
            
            virtual bool Hittable(void) const
            {
                return ex_attack.feature->is_local == 0;//最初から(だけ)有効
            }
        };

        class ExAttackReisen : public ExAttackCircle
        {
        public:
            ExAttackReisen(raw_types::ExAttack &ex_attack, unsigned int id)
                : ExAttackCircle(ex_attack, id, Reisen, 1.0f)
            {
            }
            virtual bool Hittable(void) const
            {
                return ex_attack.feature->is_local == 2;
            }
            void Update(void);
        };

        class ExAttackCirno : public ExAttack
        {
        private:
            const float width;
            const float height;
            boost::shared_ptr<HittableRect> hittable_object;
        public:
            ExAttackCirno(raw_types::ExAttack &ex_attack, unsigned int id)
                : ExAttack(ex_attack, id, Cirno), width(6), height(32)
            {
                hittable_object = boost::shared_ptr<HittableRect>(
                    new HittableRect(X(), Y(), width, height));
            }
            void Update(void);
            boost::shared_ptr<managed_types::HittableObject> HittableObject(void) const
            {
                return boost::static_pointer_cast<managed_types::HittableObject>(hittable_object);
            }
        };
        class ExAttackMystia : public ExAttackUnhittable
        {
        public:
            ExAttackMystia(raw_types::ExAttack &ex_attack, unsigned int id, ExAttackType ex_type)
                : ExAttackUnhittable(ex_attack, id, ex_type)
            {
            }
            virtual float Vx() const
            {
                return ex_attack.feature->velocity_mystia.x;
            }
            virtual float Vy() const
            {
                return ex_attack.feature->velocity_mystia.y;
            }
        };
        class ExAttackYuuka : public ExAttackCircle
        {
        public:
            ExAttackYuuka(raw_types::ExAttack &ex_attack, unsigned int id)
                : ExAttackCircle(ex_attack, id, Yuuka, 1.0f)
            {
            }
            void Update(void);
        };

        bool ExAttackFactory(
            raw_types::ExAttack &ex_attack,
            raw_types::ExAttackFuncAddr &ex_func_addr,
            IDGenerator &idgen,
            std::vector<boost::shared_ptr<ExAttack> > &attacks
            );
    }
}
