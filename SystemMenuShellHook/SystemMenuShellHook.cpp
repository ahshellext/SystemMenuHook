#include "stdafx.h"

#define DLLEXPORT extern "C" __declspec(dllexport)

HINSTANCE hMod = NULL;

#pragma data_seg(".Shared")

HWND hwndMain = NULL; // desc
HHOOK hookShell = NULL;
HHOOK hookCbt = NULL;
HHOOK hookGetMsg = NULL;
HHOOK hookCallWndProc = NULL;

#pragma data_seg()
#pragma comment(linker, "/section:.Shared,rws")

typedef void (CALLBACK *HookProc)(int, WPARAM, LPARAM);

static LRESULT CALLBACK ShellHookCb(int, WPARAM, LPARAM);
static LRESULT CALLBACK CbtHookCb(int, WPARAM, LPARAM);
static LRESULT CALLBACK GetMsgHookCb(int, WPARAM, LPARAM);
static LRESULT CALLBACK CallWndProcHookCb(int, WPARAM, LPARAM);

// WH_SHELL

DLLEXPORT bool __stdcall InitShellHook(int threadId, HWND destination) {
    if (hMod == NULL) {
        return false;
    }
        
    if (hwndMain != NULL) {
        UINT msg = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_SHELL_REPLACE");
        if (msg != 0) {
            SendNotifyMessage(hwndMain, msg, 0, 0);
        }
    }
    hwndMain = destination;
    hookShell = SetWindowsHookEx(WH_SHELL, (HOOKPROC) ShellHookCb, hMod, threadId);
    return hookShell != NULL;
}

DLLEXPORT void __stdcall UnInitShellHook() {
    if (hookShell != NULL) {
        UnhookWindowsHookEx(hookShell);
    }
    hookShell = NULL;
}

static LRESULT CALLBACK ShellHookCb(int code, WPARAM wParam, LPARAM lParam) {
    if (code >= 0) {
        UINT msg = 0;
        
        if (code == HSHELL_ACTIVATESHELLWINDOW) {
            msg = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_HSHELL_ACTIVATESHELLWINDOW");
        } else if (code == HSHELL_GETMINRECT) {
            msg = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_HSHELL_GETMINRECT");
        } else if (code == HSHELL_LANGUAGE) {
            msg = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_HSHELL_LANGUAGE");
        } else if (code == HSHELL_REDRAW) {
            msg = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_HSHELL_REDRAW");
        } else if (code == HSHELL_TASKMAN) {
            msg = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_HSHELL_TASKMAN");
        } else if (code == HSHELL_WINDOWACTIVATED) {
            msg = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_HSHELL_WINDOWACTIVATED");
        } else if (code == HSHELL_WINDOWCREATED) {
            msg = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_HSHELL_WINDOWCREATED");
        } else if (code == HSHELL_WINDOWDESTROYED) {
            msg = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_HSHELL_WINDOWDESTROYED");
        }

        if (msg != 0) {
            SendNotifyMessage(hwndMain, msg, wParam, lParam);
        }
    }

    return CallNextHookEx(hookShell, code, wParam, lParam);
}

// WH_CBT

DLLEXPORT bool __stdcall InitCbtHook(int threadId, HWND destination) {
    if (hMod == NULL) {
        return false;
    }
    if (hwndMain != NULL) {
        UINT msg = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_CBT_REPLACE");
        if (msg != 0) {
            SendNotifyMessage(hwndMain, msg, 0, 0);
        }
    }
    hwndMain = destination;
    hookCbt = SetWindowsHookEx(WH_CBT, (HOOKPROC) CbtHookCb, hMod, threadId);
    return hookCbt != NULL;
}

DLLEXPORT void __stdcall UnInitCbtHook() {
    if (hookCbt != NULL) {
        UnhookWindowsHookEx(hookCbt);
    }
    hookCbt = NULL;
}

static LRESULT CALLBACK CbtHookCb(int code, WPARAM wParam, LPARAM lParam) {
    if (code >= 0) {
        UINT msg = 0;

        if (code == HCBT_ACTIVATE) {
            msg = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_HCBT_ACTIVATE");
        } else if (code == HCBT_CREATEWND) {
            msg = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_HCBT_CREATEWND");
        } else if (code == HCBT_DESTROYWND) {
            msg = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_HCBT_DESTROYWND");
        } else if (code == HCBT_MINMAX) {
            msg = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_HCBT_MINMAX");
        } else if (code == HCBT_MOVESIZE) {
            msg = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_HCBT_MOVESIZE");
        } else if (code == HCBT_SETFOCUS) {
            msg = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_HCBT_SETFOCUS");
        } else if (code == HCBT_SYSCOMMAND) {
            msg = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_HCBT_SYSCOMMAND");
        }

        if (msg != 0) {
            SendNotifyMessage(hwndMain, msg, wParam, lParam);
        }
    }

    return CallNextHookEx(hookCbt, code, wParam, lParam);
}

// WH_GETMESSAGE

DLLEXPORT bool __stdcall InitGetMsgHook(int threadId, HWND destination) {
    if (hMod == NULL) {
        return false;
    }
    if (hwndMain != NULL) {
        UINT msg = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_GETMSG_REPLACE");
        if (msg != 0) {
            SendNotifyMessage(hwndMain, msg, 0, 0);
        }
    }
    hwndMain = destination;
    hookGetMsg = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC) GetMsgHookCb, hMod, threadId);
    return hookGetMsg != NULL;
}

DLLEXPORT void __stdcall UnInitGetMsgHook() {
    if (hookGetMsg != NULL) {
        UnhookWindowsHookEx(hookGetMsg);
    }
    hookGetMsg = NULL;
}

static LRESULT CALLBACK GetMsgHookCb(int code, WPARAM wParam, LPARAM lParam) {
    if (code >= 0) {
        UINT msg = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_GETMSG");
        UINT msg2 = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_GETMSG_PARAMS");

        auto pMsg = (MSG *) lParam;
        if (msg != 0 && pMsg->message != msg && pMsg->message != msg2 && wParam == PM_REMOVE) {
            if (pMsg->message == WM_SYSCOMMAND) {
                SendNotifyMessage(hwndMain, msg, (WPARAM) pMsg->hwnd, pMsg->message);
                SendNotifyMessage(hwndMain, msg2, pMsg->wParam, pMsg->lParam);
            }
        }
    }
    return CallNextHookEx(hookGetMsg, code, wParam, lParam);
}

// WH_CALLWNDPROC

DLLEXPORT bool __stdcall InitCallWndProcHook(int threadId, HWND destination) {
    if (hMod == NULL) {
        return false;
    }
    if (hwndMain != NULL) {
        UINT msg = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_CALLWNDPROC_REPLACE");
        if (msg != 0) {
            SendNotifyMessage(hwndMain, msg, 0, 0);
        }
    }
    hwndMain = destination;
    hookCallWndProc = SetWindowsHookEx(WH_CALLWNDPROC, (HOOKPROC) CallWndProcHookCb, hMod, threadId);
    return hookCallWndProc != NULL;
}

DLLEXPORT void __stdcall UnInitCallWndProcHook() {
    if (hookCallWndProc != NULL) {
        UnhookWindowsHookEx(hookCallWndProc);
    }
    hookCallWndProc = NULL;
}

static LRESULT CALLBACK CallWndProcHookCb(int code, WPARAM wParam, LPARAM lParam) {
    if (code >= 0) {
        UINT msg = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_CALLWNDPROC");
        UINT msg2 = RegisterWindowMessage(L"AH_SYSTEM_MENU_HOOK_CALLWNDPROC_PARAMS");

        auto pCwpStruct = (CWPSTRUCT *) lParam;
        if (msg != 0 && pCwpStruct->message != msg && pCwpStruct->message != msg2 && wParam == PM_REMOVE) {
            SendNotifyMessage(hwndMain, msg, (WPARAM) pCwpStruct->hwnd, pCwpStruct->message);
            SendNotifyMessage(hwndMain, msg2, pCwpStruct->wParam, pCwpStruct->lParam);
        }
    }
    return CallNextHookEx(hookCallWndProc, code, wParam, lParam);
}
