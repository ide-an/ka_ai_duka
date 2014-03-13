#include <Windows.h>
#include <cstdio>

#ifdef _DEBUG
FILE* fp;
#endif

void ShowKeyState(FILE* fp, unsigned short keys){
    char* bit2key[] = {
        "Z", "X", "Shift", "Esc", "Å™", "Å´", "Å©", "Å®", "Ctrl", "Q", "S", "P", "Enter", "D", "R", ""
    };
    for(int i=0;i<16;i++){
        if(keys & (1<<i)){
            fprintf(fp, " %s", bit2key[i]);
        }
    }
}

int frame = 0;
void OnFrameUpdate2(void){
    frame++;
    unsigned short* keystate_1P = reinterpret_cast<unsigned short*>(0x4ACE18+0x2C);
    unsigned short* keystate_2P = reinterpret_cast<unsigned short*>(0x4ACE18+0x2C+0x8E);
    unsigned short* keystate_3P = reinterpret_cast<unsigned short*>(0x4ACE18+0x2C+0x8E*2);
#ifdef _DEBUG
    fprintf(fp, "1P:");
    ShowKeyState(fp, *keystate_1P);
    fprintf(fp, "\t2P:");
    ShowKeyState(fp, *keystate_2P);
    fprintf(fp, "\t3P:");
    ShowKeyState(fp, *keystate_3P);
    fprintf(fp, "\n");
    //fprintf(fp, "%08X %08X %08X\n", *keystate_1P, *keystate_2P, *keystate_3P);
    //fprintf(fp, "%04X\n", *keystate_1P);
#endif
    *keystate_1P |= frame%60 < 30 ? 0x40 : 0x80;
    ShowKeyState(fp, *keystate_1P);
    fprintf(fp, "\n");
}

typedef void (*InjectTargetFunc)(void);
int __declspec(naked) OnFrameUpdate(void){
    __asm{
        pushad
        pushfd
    }
    OnFrameUpdate2();
    __asm{
        popfd
        popad
        mov eax, 1
        ret
    }
}

void SetJumpTo(char* code, int from, int to){
    *((int*)code) = to - from;
}

void InjectCode()
{
    /**
    .text:00420290                 retn
    Ç
    .text:00420290                 call OnFrameUpdate ;; return 1
    .text:00420295                 retn
    Ç…èëÇ´ä∑Ç¶ÇÈ
    */
    char* inject_to = reinterpret_cast<char*>(0x00420290);
    char code[] = {
        0xE8, 0, 0, 0, 0, // call OnFrameUpdate
        0xC3              // retn
    };
    SetJumpTo(code+1, (int)(inject_to + 5), (int)OnFrameUpdate);
    DWORD old_protect;
    ::VirtualProtect(inject_to, sizeof(code), PAGE_EXECUTE_READWRITE, &old_protect);
    ::memcpy_s(inject_to, sizeof(code), code, sizeof(code));
}

void Attach(void){
    //check exe version
#ifdef _DEBUG
    fp = fopen("hoge.txt", "wt");
#endif
    InjectCode();
}

void Detach(void){
#ifdef _DEBUG
    if(fp){
        fclose(fp);
    }
#endif
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved){
    switch(ul_reason_for_call){
    case DLL_PROCESS_ATTACH:
        Attach();
        break;
    case DLL_PROCESS_DETACH:
        Detach();
        break;
    }
    return TRUE;
}
