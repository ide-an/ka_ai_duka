#include "AIManager.h"
#include <cstdio>
namespace ka_ai_duka{
    AIManager::AIManager(void)
    {
    }

    AIManager::~AIManager(void)
    {
    }
    FILE* fp;
    int count = 0;
    void AIManager::OnGameStart(TH9Monitor &monitor)
    {
        char filename[0xff];
        frame = 0;
        count++;
        ::sprintf(filename, "C:/Users/ide/Desktop/hoge-%d.txt", count);
        fp = fopen(filename, "wt");
    }

    void AIManager::OnFrameUpdate(TH9Monitor &monitor)
    {
        static KeyState keystate = 0;//keys::right | keys::up;
        auto game_side_1p = monitor.GetGameSide(Side_1P);
        auto game_side_2p = monitor.GetGameSide(Side_2P);
        //auto player = game_side_1p->Player();
        //if(player->X() < -130.0 && (keystate & keys::left)){
        //    keystate &= ~keys::left;
        //    keystate |= keys::right;
        //}
        //if(player->X() > 130.0 && (keystate & keys::right)){
        //    keystate &= ~keys::right;
        //    keystate |= keys::left;   
        //}
        //if(player->Y() < 20.0 && (keystate & keys::up)){
        //    keystate &= ~keys::up;
        //    keystate |= keys::down;
        //}
        //if(player->Y() > 380.0 && (keystate & keys::down)){
        //    keystate &= ~keys::down;
        //    keystate |= keys::up;
        //}
        //TODO: debug only
        char text[0xff];
        ::sprintf(text, "%d : %d" , game_side_1p->Bullets().size(), game_side_2p->Bullets().size());
        monitor.SetWindowTitle(text);
        //fprintf(fp, "bullet: %d : %d\n", 
        //    game_side_1p->Bullets().size(),
        //    game_side_2p->Bullets().size()
        //    );
        
        monitor.SetKeyState(Side_1P, keystate);
        
        frame++;
    }

    void AIManager::OnGameEnd(TH9Monitor &monitor)
    {
        fclose(fp);
    }
}
