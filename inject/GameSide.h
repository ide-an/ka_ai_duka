#pragma once
#include "th09types.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Item.h"
#include "ExAttack.h"
#include "player_side.h"
#include "idgen.h"
#include <vector>
#include <boost\shared_ptr.hpp>

namespace ka_ai_duka{
    namespace managed_types{
        typedef std::vector<boost::shared_ptr<Bullet> > Bullets;
        typedef std::vector<boost::shared_ptr<Enemy> > Enemies;
        typedef std::vector<boost::shared_ptr<Item> > Items;
        typedef std::vector<boost::shared_ptr<ExAttack> > ExAttacks;

        class GameSide
        {
        private:
            raw_types::Board &board;
            unsigned int &round_win;
            struct raw_types::ExAttackFuncAddr &ex_func_addr;
            raw_types::ExAttackContainer* &ex_attack_container;
            PlayerSide player_side;
            Player* player;
            Bullets bullets;
            bool bullet_marks[536];//TODO: raw_types‚Ì•û‚Å’è”‚Æ‚µ‚Ä“f‚¢‚Ä‚¨‚­‚×‚«?
            //bool bullet_marks[sizeof(raw_types::BulletContainer.bullets)/sizeof(raw_types::Bullet)];
            bool laser_marks[48];
            //bool laser_marks[sizeof(raw_types::BulletContainer.lasers)/sizeof(raw_types::Laser)];
            const unsigned int bullet_marks_length;
            const unsigned int laser_marks_length;
            Enemies enemies;
            bool enemy_marks[128];//TODO: const‚Å“¾‚½‚¢
            const unsigned int enemy_marks_length;
            Items items;
            bool item_marks[4];
            const unsigned int item_marks_length;
            ExAttacks ex_attacks;
            bool ex_attack_marks[256];
            const unsigned int ex_attack_marks_length;
            void UpdateBullets(IDGenerator &idgen);
            void UpdateEnemies(IDGenerator &idgen);
            void UpdateItems(IDGenerator &idgen);
            void UpdateExAttacks(IDGenerator &idgen);
        public:
            GameSide(
                raw_types::Board &board,
                unsigned int &round_win,
                raw_types::ExAttackFuncAddr &ex_func_addr,
                raw_types::ExAttackContainer* &ex_attack_container,
                PlayerSide player_side
                );
            virtual ~GameSide(void);
            void Update(IDGenerator &idgen);
            Player* Player(void)
            {
                return player;
            }
            const Bullets& Bullets(void)
            {
                return bullets;
            }
            const Enemies& Enemies(void)
            {
                return enemies;
            }
            const Items& Items(void)
            {
                return items;
            }
            const managed_types::ExAttacks& ExAttacks(void)
            {
                return ex_attacks;
            }
            unsigned int RoundWin(void) const
            {
                return round_win;
            }
            unsigned int Score(void) const
            {
                return board.score->score;
            }
        };
    }
}
