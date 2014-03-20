#pragma once
#include "th09types.h"
#include "Player.h"

namespace ka_ai_duka{
    namespace managed_types{
        class GameSide
        {
        private:
            Player* player;
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