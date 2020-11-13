#ifndef UTIL_H
#define UTIL_H

#include <windows.h>
#include <inttypes.h>
#include <psapi.h>
#include <stdio.h>

#include <minhook.h>
#if defined(UE32_4_12)
#pragma comment(lib, "libMinHook.x86.lib")
#else
#pragma comment(lib, "libMinHook.x64.lib")
#endif

#include "core.h"

class Util
{
private:
    static BOOL MaskCompare(PVOID pBuffer, LPCSTR lpPattern, LPCSTR lpMask)
    {
        for (auto value = static_cast<PBYTE>(pBuffer); *lpMask; ++lpPattern, ++lpMask, ++value)
        {
            if (*lpMask == 'x' && *reinterpret_cast<LPCBYTE>(lpPattern) != *value)
            {
                return false;
            }
        }

        return true;
    }

public:
    static VOID InitConsole()
    {
        AllocConsole();

        FILE* pFile;
        freopen_s(&pFile, "CONOUT$", "w", stdout);
    }

    // TODO(Cyuubi): These patterns will need to be updated depending on the version.
    static VOID InitCore()
    {
#if defined(UE32_4_12)
        auto pGEngineAddress = BaseAddress() + 0x2E67DD8; // FIXME(Cyuubi): Hardcoded address for 0.6.5.

        auto pStaticConstructObject_InternalOffset = FindPattern
        (
            "\xE8\x00\x00\x00\x00\x0F\xB6\x53\x04",
            "x????xxxx"
        );

        auto pStaticConstructObject_InternalAddress = pStaticConstructObject_InternalOffset + 5 + *reinterpret_cast<int32_t*>(pStaticConstructObject_InternalOffset + 1);

#ifdef PROCESSEVENT_HOOK
        auto pFree_InternalOffset = FindPattern
        (
            "\xE8\x00\x00\x00\x00\xFF\x76\xB0",
            "x????xxx"
        );

        auto pFree_InternalAddress = pFree_InternalOffset + 5 + *reinterpret_cast<int32_t*>(pFree_InternalOffset + 1);

        auto pGetObjectName_InternalAddress = FindPattern
        (
            "\x8B\x4C\x24\x08\x83\xEC\x08\x56\x8B\x74\x24\x10\x85\xC9\x75\x4E",
            "xxxxxxxxxxxxxxxx"
        );

        auto pProcessEventAddress = FindPattern
        (
            "\x55\x8B\xEC\x81\xEC\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\xFC\x56\x8B\xF1\x8B\x4D\x0C\x57\x8B\x7D\x08\x89\x75\x8C",
            "xxxxx????x????xxxxxxxxxxxxxxxxxx"
        );
#endif // PROCESSEVENT_HOOK
#elif defined(UE64_4_23)
        auto pGEngineOffset = FindPattern
        (
            "\x48\x8B\xD3\x00\x00\x00\x00\x00\x48\x8B\x4C\x24\x40\x48\x89\x05\x00\x00\x00\x00\x48\x85\xC9",
            "xxx?????xxxxxxxx????xxx"
        );

        auto pGEngineAddress = pGEngineOffset + 20 + *reinterpret_cast<int32_t*>(pGEngineOffset + 16);

        auto pStaticConstructObject_InternalOffset = FindPattern
        (
            "\xE8\x00\x00\x00\x00\x44\x8D\x4D\x01",
            "x????xxxx"
        );

        auto pStaticConstructObject_InternalAddress = pStaticConstructObject_InternalOffset + 5 + *reinterpret_cast<int32_t*>(pStaticConstructObject_InternalOffset + 1);

#ifdef PROCESSEVENT_HOOK
        auto pFree_InternalAddress = FindPattern
        (
            "\x48\x85\xC9\x74\x2E\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x8B\x0D\x00\x00\x00\x00\x48\x85\xC9\x75\x0C",
            "xxxxxxxxxxxxxxxx????xxxxx"
        );

        auto pGetObjectName_InternalAddress = FindPattern
        (
            "\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x85\xD2\x75\x30\x33\xC0\x48\x89\x01\x48\xC7\x41\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\x0B\x48\x8D\x15\x00\x00\x00\x00\x41\xB8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\xC3\x48\x83\xC4\x20\x5B\xC3\x48\x8B\x42\x18",
            "xxxxxxxxxxxxxxxxxxxxxx?????x????xxxxxx????xx????x????xxxxxxxxxxxxx"
        );

        auto pProcessEventAddress = FindPattern
        (
            "\x40\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8D\x6C\x24\x00\x48\x89\x9D\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC5\x48\x89\x85\x00\x00\x00\x00\x8B\x41\x0C\x45\x33\xF6\x3B\x05\x00\x00\x00\x00\x4D\x8B\xF8\x48\x8B\xF2\x4C\x8B\xE1\x41\xB8\x00\x00\x00\x00\x7D\x2A",
            "xxxxxxxxxxxxxxx????xxxx?xxx????xxx????xxxxxx????xxxxxxxx????xxxxxxxxxxx????xx"
        );
#endif // PROCESSEVENT_HOOK
#elif defined(UE64_4_26)
#ifndef S14_OFFSETS
        auto pGEngineOffset = FindPattern
        (
            "\x48\x8B\xD3\x00\x00\x00\x00\x00\x48\x8B\x4C\x24\x40\x48\x89\x05\x00\x00\x00\x00\x48\x85\xC9",
            "xxx?????xxxxxxxx????xxx"
        );

        auto pGEngineAddress = pGEngineOffset + 20 + *reinterpret_cast<int32_t*>(pGEngineOffset + 16);
#else
        auto pGEngineOffset = FindPattern
        (
            "\x48\x89\x74\x24\x20\xE8\x00\x00\x00\x00\x48\x8B\x4C\x24\x40\x48\x89\x05",
            "xxxxxx????xxxxxxxx"
        );

        auto pGEngineAddress = pGEngineOffset + 22 + *reinterpret_cast<int32_t*>(pGEngineOffset + 18);
#endif // S14_OFFSETS

        auto pStaticConstructObject_InternalAddress = FindPattern
        (
            "\x4C\x89\x44\x24\x18\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4",
            "xxxxxxxxxxxxxxxxxxxxx????xxx????xxx????xxx"
        );

#ifdef PROCESSEVENT_HOOK
        auto pFree_InternalAddress = FindPattern
        (
            "\x48\x85\xC9\x74\x2E\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x8B\x0D\x00\x00\x00\x00\x48\x85\xC9\x75\x0C",
            "xxxxxxxxxxxxxxxx????xxxxx"
        );

        auto pGetObjectName_InternalAddress = FindPattern
        (
            "\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x85\xD2\x75\x45\x33\xC0\x48\x89\x01\x48\x89\x41\x08\x8D\x50\x05\xE8\x00\x00\x00\x00\x8B\x53\x08\x8D\x42\x05\x89\x43\x08\x3B\x43\x0C\x7E\x08\x48\x8B\xCB\xE8\x00\x00\x00\x00\x48\x8B\x0B\x48\x8D\x15\x00\x00\x00\x00\x41\xB8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\xC3\x48\x83\xC4\x20\x5B\xC3\x48\x8B\x42\x18",
            "xxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxx????xxxxxx????xx????x????xxxxxxxxxxxxx"
        );

        auto pProcessEventAddress = FindPattern
        (
            "\x40\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8D\x6C\x24\x00\x48\x89\x9D\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC5\x48\x89\x85\x00\x00\x00\x00\x8B\x41\x0C\x45\x33\xF6\x3B\x05\x00\x00\x00\x00\x4D\x8B\xF8\x48\x8B\xF2\x4C\x8B\xE1\x41\xB8\x00\x00\x00\x00\x7D\x2A",
            "xxxxxxxxxxxxxxx????xxxx?xxx????xxx????xxxxxx????xxxxxxxx????xxxxxxxxxxx????xx"
        );
#endif // PROCESSEVENT_HOOK
#endif

        GEngine = *reinterpret_cast<UEngine**>(pGEngineAddress);

        if (!pStaticConstructObject_InternalAddress)
        {
            MessageBox(0, L"Invalid StaticConstructObject_Internal address, exiting...", L"Error", MB_ICONERROR);
            ExitProcess(EXIT_FAILURE);
        }

        StaticConstructObject_Internal = reinterpret_cast<fStaticConstructObject_Internal>(pStaticConstructObject_InternalAddress);

#ifdef PROCESSEVENT_HOOK
        if (!pFree_InternalAddress)
        {
            MessageBox(0, L"Invalid Free_Internal address, exiting...", L"Error", MB_ICONERROR);
            ExitProcess(EXIT_FAILURE);
        }

        Free_Internal = reinterpret_cast<fFree_Internal>(pFree_InternalAddress);

        if (!pGetObjectName_InternalAddress)
        {
            MessageBox(0, L"Invalid GetObjectName_Internal address, exiting...", L"Error", MB_ICONERROR);
            ExitProcess(EXIT_FAILURE);
        }

        GetObjectName_Internal = reinterpret_cast<fGetObjectName_Internal>(pGetObjectName_InternalAddress);

        if (!pProcessEventAddress)
        {
            MessageBox(0, L"Invalid ProcessEvent address, exiting...", L"Error", MB_ICONERROR);
            ExitProcess(EXIT_FAILURE);
        }

        // FIXME(Cyuubi): This is temporary, it should be moved into InitProcessEvent.
        MH_Initialize();

        MH_CreateHook(pProcessEventAddress, ProcessEventHook, reinterpret_cast<PVOID*>(&ProcessEvent));
        MH_EnableHook(pProcessEventAddress);
#endif // PROCESSEVENT_HOOK
    }

    static uintptr_t BaseAddress()
    {
        return reinterpret_cast<uintptr_t>(GetModuleHandle(0));
    }

    static PBYTE FindPattern(PVOID pBase, DWORD dwSize, LPCSTR lpPattern, LPCSTR lpMask)
    {
        dwSize -= static_cast<DWORD>(strlen(lpMask));

        for (auto i = 0UL; i < dwSize; ++i)
        {
            auto pAddress = static_cast<PBYTE>(pBase) + i;

            if (MaskCompare(pAddress, lpPattern, lpMask))
            {
                return pAddress;
            }
        }

        return NULL;
    }

    static PBYTE FindPattern(LPCSTR lpPattern, LPCSTR lpMask)
    {
        MODULEINFO info{};
        GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &info, sizeof(info));

        return FindPattern(info.lpBaseOfDll, info.SizeOfImage, lpPattern, lpMask);
    }
};

#endif // UTIL_H
