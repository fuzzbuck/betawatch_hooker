#include "pch.h"
#include <iostream>
#include "psapi.h"

void SetupConsole() {
    if (!AllocConsole()) {
        // this might happen if GameClientApp.exe is launched with the --console flag, in which case we should try AttachConsole instead
        AttachConsole(GetCurrentProcessId());
    }

    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "betawatch hooker loaded\n";
}

// courtesy of cere4l
// triggers exception handlers to force decryption of a page
void DecryptPage(__int64 page, __int64 gameBase) {
    DWORD_PTR vehHandler = gameBase + 0x13810d0;
    typedef void(__fastcall* dummyFn)(_EXCEPTION_POINTERS*);
    auto dummyHandler = (dummyFn)vehHandler;
    //PVECTORED_EXCEPTION_HANDLER vehHandler = (PVECTORED_EXCEPTION_HANDLER)&;
    _EXCEPTION_POINTERS exc{};
    _EXCEPTION_RECORD record{};
    exc.ExceptionRecord = &record;
    exc.ExceptionRecord->ExceptionCode = 0xC0000005;
    exc.ExceptionRecord->ExceptionAddress = (PVOID)(gameBase + page);
    exc.ExceptionRecord->ExceptionInformation[1] = gameBase + page;
    dummyHandler(&exc);
}


DWORD WINAPI MainThread(LPVOID lpParam) {
    SetupConsole();

    // base adress of this module
    HMODULE gameModule = GetModuleHandle(NULL);
    __int64 gameBase = (__int64)gameModule;

    printf("gameBase: %llx\n", gameBase);

    // base of winsocks module
    HMODULE ws2Module = GetModuleHandle(L"ws2_32.dll");
    __int64 ws2Base = (__int64)ws2Module;
    printf("ws2Base: %llx\n", ws2Base);


    // get size of the game module
    MODULEINFO modInfo;
    GetModuleInformation(GetCurrentProcess(), gameModule, &modInfo, sizeof(MODULEINFO));
    __int64 gameSize = modInfo.SizeOfImage;

    printf("gameSize: %llx\n", gameSize);
    printf("decrypting pages\n");

    // decrypt pages
    /* call it from 0x1000 to game end (and increase by 0x100 every call) */
    int amt = 0;
    for (size_t i = 0x1000; i < gameSize; i += 0x100) {
		DecryptPage(i, gameBase);
        amt += 1;
	}
    printf("decrypted %d pages\n", amt);

	return 0;
}


