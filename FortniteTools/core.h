#ifndef CORE_H
#define CORE_H

#include "build.h"

#include <windows.h>
#include <string>

#include "struct.h"

#include <minhook.h>
#if defined(UE32_4_12)
#pragma comment(lib, "libMinHook.x86.lib")
#else
#pragma comment(lib, "libMinHook.x64.lib")
#endif

static UEngine* GEngine;

#if defined(UE32_4_12)
typedef UObject* (__cdecl* fStaticConstructObject_Internal)
#else
typedef UObject* (__fastcall* fStaticConstructObject_Internal)
#endif
(
        void*           Class   // (Missing: UClass)
    ,   UObject*        InOuter
    ,   void*           Name
    ,   int             SetFlags
    ,   unsigned int    InternalSetFlags
    ,   UObject*        Template
    ,   bool            bCopyTransientsFromClassDefaults
    ,   void*           InstanceGraph
#if defined(UE64_4_20) ||defined(UE64_4_22) || defined(UE64_4_23) || defined(UE64_4_26) 
    ,   bool            bAssumeTemplateIsArchetype
#endif
);

static fStaticConstructObject_Internal StaticConstructObject_Internal;

// NOTE(Cyuubi): Dunno what this is actually called, but it works.
#if defined(UE32_4_12)
typedef void (__cdecl* fFree_Internal)
#else
typedef void (__fastcall* fFree_Internal)
#endif
(
    void* Buffer
);

static fFree_Internal Free_Internal;

// NOTE(Cyuubi): Dunno what this is actually called, but it works.
#if defined(UE32_4_12)
typedef FString (__cdecl* fGetObjectName_Internal)
#else
typedef FString (__fastcall* fGetObjectName_Internal)
#endif
(
    UObject* Object
);

static fGetObjectName_Internal GetObjectName_Internal;

static std::wstring GetObjectFirstName(UObject* Object)
{
    std::wstring sName(L"");

    FString objName = GetObjectName_Internal(Object);

    if (objName.IsValid())
    {
        sName = objName.c_str();

        Free_Internal(objName.c_str());
    }

    return sName;
}

static std::wstring GetObjectName(UObject* Object)
{
    std::wstring sName(L"");

    for (auto i = 0; Object; Object = Object->OuterPrivate, ++i)
    {
        FString objName = GetObjectName_Internal(Object);

        if (objName.IsValid())
        {
            sName = objName.c_str() + std::wstring(i > 0 ? L"." : L"") + sName;

            Free_Internal(objName.c_str());
        }
    }

    return sName;
}

static PBYTE pProcessEventAddress;

typedef void* (__fastcall* fProcessEvent)
(
        UObject*    Object
    ,   UObject*    Function
    ,   void*       Params
);

static fProcessEvent ProcessEvent;

// Sigh... Thank you, 32-bit. For nothing, xD.
#if defined(UE32_4_12)
static void* __fastcall ProcessEventHook(UObject* Object, UObject* Function, void* Params)
#else
static void* ProcessEventHook(UObject* Object, UObject* Function, void* Params)
#endif
{
    if (Object && Function)
    {
        std::wstring sObjName = GetObjectFirstName(Object);
        std::wstring sFuncName = GetObjectFirstName(Function);
            
        if (sFuncName.find(L"SendClientHello") != std::string::npos ||
            sFuncName.find(L"SendPacketToServer") != std::string::npos ||
            sFuncName.find(L"SendPacketToClient") != std::string::npos)
        {
            return NULL;
        }

        // TODO(Cyuubi): Re-add ProcessEvent logging, after it's been cleaned up and more refined.
    }

    return ProcessEvent(Object, Function, Params);
}

class Core
{
public:
    static BOOL InitConsole()
    {
        if (GEngine->GameViewport->ViewportConsole)
        {
            return FALSE;
        }

        UObject* pConsole = StaticConstructObject_Internal
        (
                GEngine->ConsoleClass
            ,   GEngine->GameViewport
            ,   nullptr
            ,   0
            ,   0
            ,   nullptr
            ,   false
            ,   nullptr
#if defined(UE64_4_20) ||defined(UE64_4_22) || defined(UE64_4_23) || defined(UE64_4_26)
            ,   false
#endif
        );

        GEngine->GameViewport->ViewportConsole = static_cast<UConsole*>(pConsole);

        return TRUE;
    }

    static BOOL InitProcessEvent()
    {
        if (MH_CreateHook(pProcessEventAddress, ProcessEventHook, reinterpret_cast<PVOID*>(&ProcessEvent)) &&
            MH_EnableHook(pProcessEventAddress))
        {
            return FALSE;
        }

        return TRUE;
    }
};

#endif // CORE_H
