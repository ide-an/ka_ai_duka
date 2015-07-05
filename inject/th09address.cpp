#include "th09address.h"
namespace ka_ai_duka{
    namespace address{
        const struct Address addr_on_frame_update = {
            reinterpret_cast<char*>(0x420290),
            reinterpret_cast<char*>(0x420240)
        };
        const struct Address addr_on_replay_update = {
            reinterpret_cast<char*>(0x4205DA),
            reinterpret_cast<char*>(0x42058A)
        };
        const struct Address addr_on_game_start = {
            reinterpret_cast<char*>(0x41B2C3),
            reinterpret_cast<char*>(0x41B273)
        };
        const struct Address addr_on_game_end = {
            reinterpret_cast<char*>(0x41B9A2),
            reinterpret_cast<char*>(0x41B952)
        };
        const struct Address addr_window_title = {
            reinterpret_cast<char*>(0x4901E4),
            reinterpret_cast<char*>(0x48B1EC)
        };
        struct raw_types::ExAttackFuncAddr ex_attack_func_addr_ver1_5 = {
            0x441100,
            0x441A70,
            0x442750,
            0x442BD0,
            0x4435E0,
            0x443DF0,
            0x445190,
            0x445740,
            0x44BE40,
            0x44C4C0,
            0x446060,
            0x4464A0,
            0x446920,
            0x446EE0,
            0x4471B0,
            0x447890,
            0x4491E0,
            0x44A330,
            0x448490,
            0x44ADE0,
            0x44B500
        };
        struct raw_types::ExAttackFuncAddr ex_attack_func_addr_ver1_0 = {//TODO: ver 1.0対応
            0x43EF70,
            0x43F860,
            0x440540,
            0x4409C0,
            0x4413D0,
            0x441BE0,
            0x442F80,
            0x443530,
            0x449CA0,
            0x44A380,
            0x443E50,
            0x444290,
            0x444880,
            0x444CD0,
            0x444FA0,
            0x445680,
            0x446F50,
            0x4480A0,
            0x446200,
            0x448B40,
            0x4492C0
        };
        struct Th9GlobalVer1_5* const globals_ver1_5 = reinterpret_cast<struct Th9GlobalVer1_5*>(0x4A7D94);
        //TODO: ver1.0用のアドレス
        struct Th9GlobalVer1_0* const globals_ver1_0 = reinterpret_cast<struct Th9GlobalVer1_0*>(0x49ED94);
    }
}
