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
    char s2[0x800];
    GetCurrentDirectory(sizeof(s2), s2);
    ZeroMemory(s, sizeof(s));
    std::copy(path.begin(), path.end(), s);
    OPENFILENAME op;
    memset(&op, 0, sizeof(op));
    op.lStructSize = sizeof(op);
    op.hwndOwner = hwnd;
    op.lpstrFilter = "lua(*.lua)\0 *.lua\0\0";
    op.lpstrFile = s;
    op.lpstrInitialDir = s2;
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

bool OpenFileDialogExe(HWND hwnd, const std::string &path, std::string &new_path)
{
    char s[0x800];
    ZeroMemory(s, sizeof(s));
    std::copy(path.begin(), path.end(), s);
    OPENFILENAME op;
    memset(&op, 0, sizeof(op));
    op.lStructSize = sizeof(op);
    op.hwndOwner = hwnd;
    op.lpstrFilter = "exe(*.exe)\0 *.exe\0\0";
    op.lpstrFile = s;
    op.nMaxFile = sizeof(s);
    op.lpstrTitle = "ファイルを開く";
    op.Flags = OFN_FILEMUSTEXIST;
    op.lpstrDefExt = "exe";
    if(GetOpenFileNameA(&op)){
        new_path = s;
        return true;
    }else{
        return false;
    }
}

void OnSelectFileClick(HWND hwnd, INT_PTR edit_box, bool is_exe=false)
{
    std::string old_path;
    GetScriptPath(hwnd, old_path, edit_box);
    std::string new_path;
    bool res;
    if(is_exe){
        res = OpenFileDialogExe(hwnd, old_path, new_path);
    }else{
        res = OpenFileDialog(hwnd, old_path, new_path);
    }
    if(res){
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
    ChangeScriptPath(hwnd, conf.Th09ExePath(), ID_EDIT_EXEPATH);
    SetChecked(hwnd, conf.EnableSnapshot(), IDC_ENABLE_SNAPTSHOT);
    SetChecked(hwnd, conf.EnableRunWhileReplay(), IDC_DRY_RUN);
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
    std::string exe_path;
    GetScriptPath(hwnd, exe_path, ID_EDIT_EXEPATH);
    conf.SetTh09ExePath(exe_path);
    conf.SetEnableSnapshot(IsChecked(hwnd, IDC_ENABLE_SNAPTSHOT));
    conf.SetEnableRunWhileReplay(IsChecked(hwnd, IDC_DRY_RUN));
}

bool RunExe(void)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ::ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ::ZeroMemory(&pi, sizeof(pi));
    if(!::CreateProcess("ka_ai_duka.exe", NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi)){
        return false;
    }
    ::CloseHandle(pi.hThread);
    ::CloseHandle(pi.hProcess);
    return true;
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
                case ID_SELECTFILE_EXE:
                    OnSelectFileClick(hWnd, ID_EDIT_EXEPATH, true);
                    break;
                case IDC_ENABLE_SNAPTSHOT:
                    // Nothing to do
                    break;
                case IDC_DRY_RUN:
                    // Nothing to do
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
    try{
        ka_ai_duka::common::Config::IniFilePath(NULL, ini_path);
    }catch(std::exception &e){
        //iniファイルのパスがそもそも長過ぎるケースは諦める
        ::MessageBoxA(NULL, e.what(), "エラー", MB_OK);
        return 1;
    }
    try{
        conf.Load(ini_path);
    }catch (...) {
        //iniファイルがうまく読めなければデフォルト設定とする
        conf.SetEnable1P(false);
        conf.SetEnable2P(false);
        conf.SetScriptPath1P("");
        conf.SetScriptPath2P("");
        conf.SetTh09ExePath("");
        conf.SetEnableSnapshot(false);
        conf.SetEnableRunWhileReplay(false);
    }
    auto res = DialogBoxA(hInstance, "IDD_DIALOG1", NULL, DialogProc);
    if(res ==-1){
        auto err = GetLastError();
    }else if(res == IDOK){
        conf.Save(ini_path);
        //if(!RunExe()){
        //    ::MessageBoxA(NULL, "ka_ai_duka.exeの起動に失敗しました。", "エラー", MB_OK);
        //    return 1;
        //}
    }
    return 0;
}