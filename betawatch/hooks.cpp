#include "pch.h"
#include <winsock.h>
#include <iostream>
#include "MinHook.h"
#include "hooks.h"

int(__stdcall* ws2_send)(SOCKET s, const char* buf, int len, int flags) = nullptr;

int __stdcall hk_send(SOCKET s, const char* buf, int len, int flags) {
    // print the data being sent
    std::string data(buf, len);
    std::cout << "sending: " << data << std::endl;

    return ws2_send(s, buf, len, flags);
}


void InitHooks(__int64 gameBase) {
 
    // hook winsocks
    LPVOID* winhook_send_pp = nullptr;
    MH_CreateHookApiEx(L"ws2_32.dll", "send", &hk_send, (LPVOID*) &ws2_send, winhook_send_pp);


    // enable hooks
    MH_EnableHook(MH_ALL_HOOKS);
    printf("hooks enabled\n");
}
