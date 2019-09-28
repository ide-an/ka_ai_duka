#include "GameSide.h"
#include <algorithm>
#include <Windows.h>

namespace ka_ai_duka{
    namespace managed_types{
        GameSide::GameSide(
            raw_types::Board &board, 
            unsigned int &round_win, 
            char &charge_type,
            raw_types::ExAttackFuncAddr &ex_func_addr,
            raw_types::ExAttackContainer* &ex_attack_container,
            PlayerSide player_side
            )
            : board(board),
            round_win(round_win),
            charge_type(charge_type),
            bullet_marks_length(536),
            laser_marks_length(48),
            enemy_marks_length(128),
            item_marks_length(4),
            ex_attack_marks_length(256),
            ex_func_addr(ex_func_addr),
            ex_attack_container(ex_attack_container),
            player_side(player_side)
        {
            player = new managed_types::Player(*board.player, board.player_character);
            bullets.reserve(1000);
            enemies.reserve(128);
            items.reserve(4);
            std::fill(std::begin(bullet_marks), std::end(bullet_marks), false);
            std::fill(std::begin(laser_marks), std::end(laser_marks), false);
            std::fill(std::begin(enemy_marks), std::end(enemy_marks), false);
            std::fill(std::begin(item_marks), std::end(item_marks), false);
            std::fill(std::begin(ex_attack_marks), std::end(ex_attack_marks), false);
        }

        GameSide::~GameSide(void)
        {
            delete player;
        }
        void ResetBullets(Bullets &bullets)
        {
            std::for_each(bullets.begin(), bullets.end(), [](Bullets::value_type elm){
                elm->Update();
            });
        }
        void GameSide::UpdateBullets(IDGenerator &idgen)
        {
            bullets.erase(std::remove_if(
                bullets.begin(), bullets.end(), 
                [](std::shared_ptr<Bullet> elm){
                    return !elm->Enabled();
                }), bullets.end());
            for(unsigned int i=0;i<bullet_marks_length;i++){
                bool is_enabled = NormalBullet::IsEnabled(board.bullet_container->bullets[i]);
                if(bullet_marks[i] && !is_enabled){
                    bullet_marks[i] = false;
                }else if(!bullet_marks[i] && is_enabled){
                    bullet_marks[i] = true;
                    bullets.push_back(std::shared_ptr<Bullet>(new NormalBullet(board.bullet_container->bullets[i], idgen.NewId())));
                }
            }
            for(unsigned int i=0;i<laser_marks_length;i++){
                bool is_enabled = Laser::IsEnabled(board.bullet_container->lasers[i]);
                if(laser_marks[i] && !is_enabled){
                    laser_marks[i] = false;
                }else if(!laser_marks[i] && is_enabled){
                    laser_marks[i] = true;
                    bullets.push_back(std::shared_ptr<Bullet>(new Laser(board.bullet_container->lasers[i], idgen.NewId())));
                }
            }
            ResetBullets(bullets);
            //std::for_each(bullets.begin(), bullets.end(), [](Bullets::value_type elm){
            //    elm->Update();
            //});
        }
        
        void ResetEnemies(Enemies &enemies)
        {
            std::for_each(enemies.begin(), enemies.end(), [](Enemies::value_type elm){
                elm->Update();
            });
        }
        void GameSide::UpdateEnemies(IDGenerator &idgen)
        {
            enemies.erase(std::remove_if(
                enemies.begin(), enemies.end(),
                [](Enemies::value_type elm){
                    return !elm->Enabled();
                }), enemies.end());
            for(unsigned int i=0;i<enemy_marks_length;i++){
                bool is_enabled = Enemy::IsEnabled(board.enemy_container->enemies[i]);
                if(enemy_marks[i] && !is_enabled){
                    enemy_marks[i] = false;
                }else if(!enemy_marks[i] && is_enabled){
                    enemy_marks[i] = true;
                    enemies.push_back(std::shared_ptr<Enemy>(new Enemy(board.enemy_container->enemies[i], idgen.NewId())));
                }
            }
            ResetEnemies(enemies);
            //std::for_each(enemies.begin(), enemies.end(), [](Enemies::value_type elm){
            //    elm->Update();
            //});
        }

        void ResetItems(Items &items)
        {
            std::for_each(items.begin(), items.end(), [](Items::value_type elm){
                elm->Update();
            });
        }
        void GameSide::UpdateItems(IDGenerator &idgen)
        {
            items.erase(std::remove_if(
                items.begin(), items.end(),
                [](Items::value_type elm){
                    return !elm->Enabled();
                }), items.end());
            for(unsigned int i=0;i<item_marks_length;i++){
                bool is_enabled = Item::IsEnabled(board.player->items[i]);
                if(item_marks[i] && !is_enabled){
                    item_marks[i] = false;
                }else if(!item_marks[i] && is_enabled){
                    item_marks[i] = true;
                    items.push_back(std::shared_ptr<Item>(new Item(board.player->items[i], idgen.NewId(), board.player->feature->item_size)));
                }
            }
            ResetItems(items);
            //std::for_each(items.begin(), items.end(), [](Items::value_type elm){
            //    elm->Update();
            //});
        }

        void ResetExAttacks(ExAttacks &ex_attacks)
        {
            std::for_each(ex_attacks.begin(), ex_attacks.end(), [](ExAttacks::value_type elm){
                elm->Update();
            });
        }
        void GameSide::UpdateExAttacks(IDGenerator &idgen)
        {
            ex_attacks.erase(std::remove_if(
                ex_attacks.begin(), ex_attacks.end(),
                [](ExAttacks::value_type elm){
                    return !elm->Enabled();
                }), ex_attacks.end());
            for(unsigned int i=0;i<ex_attack_marks_length;i++){
                bool is_enabled = ExAttack::IsEnabled(ex_attack_container->attacks[i]);
                if(ex_attack_marks[i] && !is_enabled){
                    ex_attack_marks[i] = false;
                }else if(!ex_attack_marks[i] && is_enabled && ExAttack::GetPlayerSide(ex_attack_container->attacks[i]) == player_side){
                    if(ExAttackFactory(ex_attack_container->attacks[i], ex_func_addr, idgen, ex_attacks)){
                        ex_attack_marks[i] = true;
                    }
                }
            }
            ResetExAttacks(ex_attacks);
            //std::for_each(ex_attacks.begin(), ex_attacks.end(), [](ExAttacks::value_type elm){
            //    elm->Update();
            //});
        }

        void GameSide::Update(IDGenerator &idgen)
        {
            UpdatePlayer();
            UpdateBullets(idgen);
            UpdateEnemies(idgen);
            UpdateItems(idgen);
            UpdateExAttacks(idgen);
        }

        void GameSide::Reset()
        {
            UpdatePlayer();
            ResetBullets(bullets);
            ResetEnemies(enemies);
            ResetItems(items);
            ResetExAttacks(ex_attacks);
        }
    }
}
