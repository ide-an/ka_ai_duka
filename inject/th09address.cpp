#include "th09address.h"
namespace ka_ai_duka{
    namespace address{
        const struct Address addr_on_frame_update = {
            reinterpret_cast<char*>(0x420290),
            reinterpret_cast<char*>(0x420290)//TODO: ver 1.0対応
        };
        const struct Address addr_on_game_start = {
            reinterpret_cast<char*>(0x41B2C3),
            reinterpret_cast<char*>(0x41B2C3)//TODO: ver 1.0対応
        };
        const struct Address addr_on_game_end = {
            reinterpret_cast<char*>(0x41B9A2),
            reinterpret_cast<char*>(0x41B9A2)//TODO: ver 1.0対応
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
        struct Th9GlobalVer1_5* const globals_ver1_5 = reinterpret_cast<struct Th9GlobalVer1_5*>(0x4A7D94);
        //TODO: ver1.0用のアドレス
        struct Th9GlobalVer1_5* const globals_ver1_0 = reinterpret_cast<struct Th9GlobalVer1_5*>(0x4A7D94);
    }
}
