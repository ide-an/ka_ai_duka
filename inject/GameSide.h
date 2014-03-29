#pragma once
#include "th09types.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include <vector>
#include <boost\shared_ptr.hpp>

namespace ka_ai_duka{
    namespace managed_types{
        typedef std::vector<boost::shared_ptr<Bullet> > Bullets;
        typedef std::vector<boost::shared_ptr<Enemy> > Enemies;
        class GameSide
        {
        private:
            raw_types::Board &board;
            unsigned int &round_win;
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
            void UpdateBullets(void);
            void UpdateEnemies(void);
        public:
            GameSide(raw_types::Board &board, unsigned int &round_win);
            virtual ~GameSide(void);
            void Update(void);
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
