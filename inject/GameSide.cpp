#include "GameSide.h"

namespace ka_ai_duka{
    namespace managed_types{
        GameSide::GameSide(raw_types::Board &board)
            : board(board)
        {
            player = new managed_types::Player(*board.player, board.player_character);
            bullets.reserve(1000);
        }

        GameSide::~GameSide(void)
        {
            delete player;
        }
    }
}
