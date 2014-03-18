#include "th09address.h"
namespace ka_ai_duka{
    namespace raw_address{
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
        struct Th9GlobalVer1_5* const globals_ver1_5 = reinterpret_cast<struct Th9GlobalVer1_5*>(0x4A7D94);
        //TODO: ver1.0用のアドレス
        struct Th9GlobalVer1_5* const globals_ver1_0 = reinterpret_cast<struct Th9GlobalVer1_5*>(0x4A7D94);
    }
}