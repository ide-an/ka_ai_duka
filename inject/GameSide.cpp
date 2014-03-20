#include "GameSide.h"

namespace ka_ai_duka{
    namespace managed_types{
        GameSide::GameSide(raw_types::Board &board)
        {
            player = new managed_types::Player(*board.player, board.player_character);
        }

        GameSide::~GameSide(void)
        {
            delete player;
        }
    }
}