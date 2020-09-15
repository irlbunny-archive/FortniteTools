#ifndef CORE_H
#define CORE_H

#include "build.h"

#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "struct.h"

static UEngine* GEngine;

#if defined(UE32_4_12)
typedef UObject* (__cdecl* fStaticConstructObject_Internal)
#else
typedef UObject* (__fastcall* fStaticConstructObject_Internal)
#endif
(
        UClass*         Class
    ,   UObject*        InOuter
    ,   void*           Name
#if defined(UE32_4_12)
    ,   int             SetFlags
    ,   unsigned int    InternalSetFlags
#else
    ,   long            SetFlags
    ,   long            InternalSetFlags
#endif
    ,   UObject*        Template
    ,   bool            bCopyTransientsFromClassDefaults
    ,   void*           InstanceGraph
#if defined(UE64_4_23) || defined(UE64_4_26)
    ,   bool            bAssumeTemplateIsArchetype
#endif
);

static fStaticConstructObject_Internal StaticConstructObject_Internal;

// NOTE(Cyuubi): Dunno what this is actually called, but it works.
typedef void* (_fastcall* fFree_Internal)
(
    void* Buffer
);

static fFree_Internal Free_Internal;

// NOTE(Cyuubi): Dunno what this is actually called, but it works.
typedef FString(_fastcall* fGetObjectName_Internal)
(
    UObject* Object
);

static fGetObjectName_Internal GetObjectName_Internal;

static std::wstring GetObjectFirstName(UObject* Object)
{
    wchar_t* buffer = GetObjectName_Internal(Object).c_str();
    if (!buffer)
        return L"";

    std::wstring name(buffer);

    Free_Internal(buffer);

    return name;
}

std::wstring GetObjectName(UObject* Object) {
    std::wstring name(L"");

    for (auto i = 0; Object; Object = Object->OuterPrivate, ++i) {
        wchar_t* buffer = GetObjectName_Internal(Object).c_str();
        if (!buffer)
            break;

        name = buffer + std::wstring(i > 0 ? L"." : L"") + name;

        Free_Internal(buffer);
    }

    return name;
}

typedef void* (_fastcall* fProcessEvent)
(
        UObject*    Object
    ,   UObject*    Function
    ,   void*       Params
);

static fProcessEvent ProcessEvent;

static std::vector<std::wstring> cache;
static std::ofstream stream;

static bool bStreamInitialized;

static void* ProcessEventHook(UObject* Object, UObject* Function, void* Params)
{
        
    if (Object && Function)
    {
        std::wstring sObjectName = GetObjectFirstName(Object);
        std::wstring sFunctionName = GetObjectFirstName(Function);
            
        if (sFunctionName.find(L"SendClientHello") != std::string::npos)
            return NULL;
        if (sFunctionName.find(L"SendPacketToServer") != std::string::npos)
            return NULL;
        if (sFunctionName.find(L"SendPacketToClient") != std::string::npos)
            return NULL;

        std::wstring sValue = L"Object = " + sObjectName + L", Function = " + sFunctionName;

        if (std::find(cache.begin(), cache.end(), sValue) == cache.end())
        {
            if (!bStreamInitialized)
            {
                stream = std::ofstream("ProcessEvent-Log.txt");

                bStreamInitialized = true;
            }

            if (stream.is_open())
            {
                stream << std::string(sValue.begin(), sValue.end()) << "\n";
                stream.flush();
            }

            cache.push_back(sValue);
        }
    }

    return ProcessEvent(Object, Function, Params);
}

class Core
{
public:
    static BOOL InitConsole()
    {
        if (GEngine->GameViewport->ViewportConsole)
            return FALSE;

        auto pConsole = reinterpret_cast<UConsole*>(StaticConstructObject_Internal
        (
                GEngine->ConsoleClass
            ,   reinterpret_cast<UObject*>(GEngine->GameViewport)
            ,   nullptr
            ,   0
            ,   0
            ,   nullptr
            ,   false
            ,   nullptr
#if defined(UE64_4_23) || defined(UE64_4_26)
            ,   false
#endif
        ));

        GEngine->GameViewport->ViewportConsole = pConsole;

        return TRUE;
    }

    static BOOL InitProcessEvent()
    {
        return TRUE; // TODO
    }
};

#endif // CORE_H
