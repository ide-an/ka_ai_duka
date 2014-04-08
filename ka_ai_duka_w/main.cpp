#include <Windows.h>
#include "resource.h"

bool IsChecked(HWND hwnd, INT_PTR a)
{
    return SendMessage( GetDlgItem(hwnd, a), BM_GETCHECK, 0, 0 ) == 1;
}
void ChangeEnabled(HWND hwnd, bool enabled, INT_PTR a)
{
    EnableWindow(GetDlgItem(hwnd, a), enabled ? TRUE : FALSE);
}
void ChangeEnabled1P(HWND hwnd, bool enabled)
{
    ChangeEnabled(hwnd, enabled, ID_EDIT_SCRIPTPATH_1P);
    ChangeEnabled(hwnd, enabled, ID_SELECTFILE_1P);
}
void ChangeEnabled2P(HWND hwnd, bool enabled)
{
    ChangeEnabled(hwnd, enabled, ID_EDIT_SCRIPTPATH_2P);
    ChangeEnabled(hwnd, enabled, ID_SELECTFILE_2P);
}

BOOL CALLBACK DialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_INITDIALOG:
            //TODO: init dialog via initial config
            break;
        case WM_CLOSE:
            EndDialog(hWnd, IDCANCEL);
            return TRUE;
        case WM_COMMAND:
            {
                UINT wmId = LOWORD(wParam);
                switch(wmId){
                case ID_ENABLED_1P:
                    if(IsChecked(hWnd, ID_ENABLED_1P)){
                        ChangeEnabled1P(hWnd, true);
                    }else{
                        ChangeEnabled1P(hWnd, false);
                    }
                    break;
                case ID_ENABLED_2P:
                    if(IsChecked(hWnd, ID_ENABLED_2P)){
                        ChangeEnabled2P(hWnd, true);
                    }else{
                        ChangeEnabled2P(hWnd, false);
                    }
                    break;
                case IDOK:
                    //TODO: update config
                    EndDialog(hWnd, IDOK);
                    return TRUE;
                case IDCANCEL:
                    EndDialog(hWnd, IDCANCEL);
                    return TRUE;
                case ID_SELECTFILE_1P:
                    break;//TODO: show file dialog
                case ID_SELECTFILE_2P:
                    break; //TODO: show file dialog
                }
                break;
            }
    }
    return FALSE;
}

int APIENTRY WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    LPSTR lpCmdLine, 
    int nCmdShow
)
{
    auto res = DialogBoxA(hInstance, "IDD_DIALOG1", NULL, DialogProc);
    if(res ==-1){
        auto err = GetLastError();
    }
    //TODO: if res is ok, save config and run ka_ai_duka.exe
    return 0;
}