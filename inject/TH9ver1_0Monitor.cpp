#include "TH9Monitor.h"
#include "th09address.h"

namespace ka_ai_duka{
    TH9ver1_0Monitor::TH9ver1_0Monitor(void) :
        TH9Monitor(
            address::globals_ver1_0->board,
            address::globals_ver1_0->key_states,
            address::globals_ver1_0->ex_attack_container,
            address::globals_ver1_0->round,
            address::globals_ver1_0->round_win,
            address::globals_ver1_0->difficulty
            )
    {
    }
}