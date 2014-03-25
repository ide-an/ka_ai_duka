#include "GameSide.h"
#include <algorithm>

namespace ka_ai_duka{
    namespace managed_types{
        GameSide::GameSide(raw_types::Board &board)
            : board(board),
            bullet_marks_length(536),
            laser_marks_length(48)
        {
            player = new managed_types::Player(*board.player, board.player_character);
            bullets.reserve(1000);
            for(unsigned int i=0;i<bullet_marks_length;i++){
                bullet_marks[i] = false;
            }
            for(unsigned int i=0;i<laser_marks_length;i++){
                laser_marks[i] = false;
            }
        }

        GameSide::~GameSide(void)
        {
            delete player;
        }
        void GameSide::Update(void)
        {
            bullets.erase(std::remove_if(
                bullets.begin(), bullets.end(), 
                [](boost::shared_ptr<Bullet> elm){
                    return !elm->Enabled();
                }), bullets.end());
            for(unsigned int i=0;i<bullet_marks_length;i++){
                bool is_enabled = NormalBullet::IsEnabled(board.bullet_container->bullets[i]);
                if(bullet_marks[i] && !is_enabled){
                    bullet_marks[i] = false;
                }else if(!bullet_marks[i] && is_enabled){
                    bullet_marks[i] = true;
                    bullets.push_back(boost::shared_ptr<Bullet>(new NormalBullet(board.bullet_container->bullets[i])));
                }
            }
            for(unsigned int i=0;i<laser_marks_length;i++){
                bool is_enabled = Laser::IsEnabled(board.bullet_container->lasers[i]);
                if(laser_marks[i] && !is_enabled){
                    laser_marks[i] = false;
                }else if(!laser_marks[i] && is_enabled){
                    laser_marks[i] = true;
                    bullets.push_back(boost::shared_ptr<Bullet>(new Laser(board.bullet_container->lasers[i])));
                }
            }
        }
    }
}
