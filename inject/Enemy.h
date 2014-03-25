#pragma once
#include "th09types.h"

namespace ka_ai_duka{
    namespace managed_types{
        class Enemy
        {
        private:
            raw_types::Enemy &enemy;
        public:
            Enemy(raw_types::Enemy &enemy)
                : enemy(enemy)
            {}
            virtual ~Enemy(void)
            {
            }
            float X() const
            {
                return enemy.position.x;
            }
            float Y() const
            {
                return enemy.position.y;
            }
            float Vx() const
            {
                float vx = enemy.velocity.x;
                if(IsActivatedSpirit()){
                    vx = enemy.velocity2.x;
                }
                if(enemy.status & 0x8000){//reversed
                    vx *= -1;
                }
                if(IsBoss()){
                    //TODO: �����₳��␳
                }
                return vx;
            }
            float Vy() const
            {
                float vy = enemy.velocity.y;
                if(IsActivatedSpirit()){
                    vy = enemy.velocity2.y;
                }
                if(IsBoss()){
                    //TODO: �����₳��␳
                }
                return vy;
            }
            bool Enabled() const
            {
                return IsEnabled(enemy);
            }
            static bool IsEnabled(raw_types::Enemy &enemy)
            {
                return (enemy.status & 0x101) == 1;
            }
            bool IsSpirit() const
            {
                return (enemy.status & 0x00400000) != 0;
            }
            bool IsActivatedSpirit() const
            {
                return IsSpirit() && (enemy.status2 & 0x1000);
            }
            bool IsBoss() const
            {
                return (enemy.status & 0x80000000) != 0;
            }
            bool IsLily() const
            {
                return (enemy.status & 0x00002400) != 0;
            }
            //�e��ݒu���邾���œ����蔻��������Ȃ����̂Ȃ�
            bool IsPseudoEnemy() const
            {
                return (enemy.status & 0x00000010) != 0;
            }
            //TODO: �����蔻��
        };
    }
}