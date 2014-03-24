#pragma once
#include "th09types.h"
#include "Player.h"
#include "Bullet.h"
#include <vector>
#include <boost\shared_ptr.hpp>

namespace ka_ai_duka{
    namespace managed_types{
        class GameSide
        {
        private:
            raw_types::Board &board;
            Player* player;
            std::vector<boost::shared_ptr<Bullet*> > bullets;
        public:
            GameSide(raw_types::Board &board);
            virtual ~GameSide(void);
            void Update(void)
            {
                //TODO: implement
                //不要になった敵を管理下から外すなどの処置をする
            }
            Player* Player(void)
            {
                return player;
            }
        };
    }
}
