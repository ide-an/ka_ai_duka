#include <Windows.h>
#include "resource.h"
#include "../common/common.h"

bool IsChecked(HWND hwnd, INT_PTR a)
{
    return SendMessage( GetDlgItem(hwnd, a), BM_GETCHECK, 0, 0 ) == 1;
}
void SetChecked(HWND hwnd, bool checked, INT_PTR a)
{
    SendMessage( GetDlgItem(hwnd, a), BM_SETCHECK, checked ? BST_CHECKED : BST_UNCHECKED, 0 );
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
void ChangeScriptPath(HWND hwnd, const std::string &path, INT_PTR a)
{
    SetDlgItemTextA(hwnd, a, path.c_str());
}
void GetScriptPath(HWND hwnd, std::string &path, INT_PTR a)
{
    char s[0x800];
    GetDlgItemTextA(hwnd, a, s, sizeof(s));
    path = s;
}

bool OpenFileDialog(HWND hwnd, const std::string &path, std::string &new_path)
{
    char s[0x800];
    ZeroMemory(s, sizeof(s));
    std::copy(path.begin(), path.end(), s);
    OPENFILENAME op;
    memset(&op, 0, sizeof(op));
    op.lStructSize = sizeof(op);
    op.hwndOwner = hwnd;
    op.lpstrFilter = "lua(*.lua)\0 *.lua\0\0";
    op.lpstrFile = s;
    op.nMaxFile = sizeof(s);
    op.lpstrTitle = "ファイルを開く";
    op.Flags = OFN_FILEMUSTEXIST;
    op.lpstrDefExt = "lua";
    if(GetOpenFileNameA(&op)){
        new_path = s;
        return true;
    }else{
        return false;
    }
}

void OnSelectFileClick(HWND hwnd, INT_PTR edit_box)
{
    std::string old_path;
    GetScriptPath(hwnd, old_path, edit_box);
    std::string new_path;
    if(OpenFileDialog(hwnd, old_path, new_path)){
        ChangeScriptPath(hwnd, new_path, edit_box);
    }
}

void InitUI(HWND hwnd, ka_ai_duka::common::Config &conf)
{
    SetChecked(hwnd, conf.Enable1P(), ID_ENABLED_1P);
    SetChecked(hwnd, conf.Enable2P(), ID_ENABLED_2P);
    ChangeEnabled1P(hwnd, conf.Enable1P());
    ChangeEnabled2P(hwnd, conf.Enable2P());
    ChangeScriptPath(hwnd, conf.ScriptPath1P(), ID_EDIT_SCRIPTPATH_1P);
    ChangeScriptPath(hwnd, conf.ScriptPath2P(), ID_EDIT_SCRIPTPATH_2P);
}

void SetConfigFromUI(HWND hwnd, ka_ai_duka::common::Config &conf)
{
    conf.SetEnable1P(IsChecked(hwnd, ID_ENABLED_1P));
    conf.SetEnable2P(IsChecked(hwnd, ID_ENABLED_2P));
    std::string path_1p;
    GetScriptPath(hwnd, path_1p, ID_EDIT_SCRIPTPATH_1P);
    conf.SetScriptPath1P(path_1p);
    std::string path_2p;
    GetScriptPath(hwnd, path_2p, ID_EDIT_SCRIPTPATH_2P);
    conf.SetScriptPath2P(path_2p);
    //TODO: th09.exe path
}

static ka_ai_duka::common::Config conf;

BOOL CALLBACK DialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_INITDIALOG:
            InitUI(hWnd, conf);
            return TRUE;
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
                    SetConfigFromUI(hWnd, conf);
                    EndDialog(hWnd, IDOK);
                    return TRUE;
                case IDCANCEL:
                    EndDialog(hWnd, IDCANCEL);
                    return TRUE;
                case ID_SELECTFILE_1P:
                    OnSelectFileClick(hWnd, ID_EDIT_SCRIPTPATH_1P);
                    break;
                case ID_SELECTFILE_2P:
                    OnSelectFileClick(hWnd, ID_EDIT_SCRIPTPATH_2P);
                    break;
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
    std::string ini_path;
    ka_ai_duka::common::Config::IniFilePath(NULL, ini_path);
    conf.Load(ini_path);//TODO: error handling
    auto res = DialogBoxA(hInstance, "IDD_DIALOG1", NULL, DialogProc);
    if(res ==-1){
        auto err = GetLastError();
    }else if(res == IDOK){
        conf.Save(ini_path);
        //TODO:  run ka_ai_duka.exe
    }
    return 0;
}