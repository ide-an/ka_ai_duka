#include "AIManager.h"

namespace ka_ai_duka{
    AIManager::AIManager(void)
    {
    }

    AIManager::~AIManager(void)
    {
    }

    void AIManager::OnGameStart(TH9Monitor &monitor)
    {
        frame = 0;
    }

    void AIManager::OnFrameUpdate(TH9Monitor &monitor)
    {
        static KeyState keystate = keys::right | keys::up;
        auto game_side_1p = monitor.GetGameSide(Side_1P);
        auto player = game_side_1p->Player();
        if(player->X() < -130.0 && (keystate & keys::left)){
            keystate &= ~keys::left;
            keystate |= keys::right;
        }
        if(player->X() > 130.0 && (keystate & keys::right)){
            keystate &= ~keys::right;
            keystate |= keys::left;
        }
        if(player->Y() < 20.0 && (keystate & keys::up)){
            keystate &= ~keys::up;
            keystate |= keys::down;
        }
        if(player->Y() > 380.0 && (keystate & keys::down)){
            keystate &= ~keys::down;
            keystate |= keys::up;
        }
        monitor.SetKeyState(Side_1P, keystate);
        
        frame++;
    }

    void AIManager::OnGameEnd(TH9Monitor &monitor)
    {
    }
}
