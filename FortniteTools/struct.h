#ifndef STRUCT_H
#define STRUCT_H

#include "build.h"

struct UObject
{
public:
    void**      VTable;         // 0x0000
    int         ObjectFlags;    // 64 = 0x0008 | 32 = 0x0004
    int         InternalIndex;  // 64 = 0x000C | 32 = 0x0008
    void*       ClassPrivate;   // 64 = 0x0010 | 32 = 0x000C (Missing: UClass)
    char        Unk00[0x8];     // 64 = 0x0018 | 32 = 0x0010
    UObject*    OuterPrivate;   // 64 = 0x0020 | 32 = 0x0018
};

struct UConsole : public UObject
{
    // TODO
};

struct UScriptViewportClient : public UObject
{
public:
#ifdef UE32_4_12
    char    Unk00[0x4]; // 0x001C
#else
    char    Unk00[0x10];    // 0x0028
#endif
};


struct UGameViewportClient : public UScriptViewportClient
{
public:
#ifdef UE32_4_12
    char        Unk01[0x4];         // 0x0020
    UConsole*   ViewportConsole;    // 0x0024
#else
    char        Unk01[0x8];         // 0x0030
    UConsole*   ViewportConsole;    // 0x0040
#endif
};


struct UEngine : public UObject
{
public:
#ifdef UE32_4_12
    char                    Unk01[0x7C];    // 0x001C
    void*                   ConsoleClass;   // 0x0090 (Missing: UClass)
    char                    Unk02[0x398];   // 0x0094
    UGameViewportClient*    GameViewport;   // 0x042C
#elif defined(UE64_4_20)
    char                    Unk01[0xD0];    // 0x0028
    void*                   ConsoleClass;   // 0x00F8 (Missing: UClass)
    char                    Unk02[0x620];   // 0x0100
    UGameViewportClient*    GameViewport;   // 0x0720
#elif defined(UE64_4_21)
    char                    Unk01[0xD0];    // 0x0028
    void*                   ConsoleClass;   // 0x00F8 (Missing: UClass)
    char                    Unk02[0x621];   // 0x0100
    UGameViewportClient*    GameViewport;   // 0x0720
#elif defined(UE64_4_22)
    char                    Unk01[0xD0];    // 0x0028
    void*                   ConsoleClass;   // 0x00F8 (Missing: UClass)
    char                    Unk02[0x648];   // 0x0100
    UGameViewportClient*    GameViewport;   // 0x0748
#elif defined(UE64_4_23)
    char                    Unk01[0xD0];    // 0x0028
    void*                   ConsoleClass;   // 0x00F8 (Missing: UClass)
    char                    Unk02[0x650];   // 0x0100
    UGameViewportClient*    GameViewport;   // 0x0750
#elif defined(UE64_4_26)
    char                    Unk01[0xD0];    // 0x0028
    void*                   ConsoleClass;   // 0x00F8 (Missing: UClass)
    char                    Unk02[0x688];   // 0x0100
    UGameViewportClient*    GameViewport;   // 0x0788
#endif
};

template<class T>
struct TArray
{
    friend struct FString;

public:
    inline TArray()
    {
        Data = nullptr;

        Count = Max = 0;
    };

    inline int Num() const
    {
        return Count;
    };

    inline T& operator[](int i)
    {
        return Data[i];
    };

    inline bool IsValid(int i)
    {
        return i < Num();
    }

private:
    T* Data;

    int Count;
    int Max;
};

struct FString : private TArray<wchar_t>
{
    FString()
    {
        Data = nullptr;

        Max = Count = 0;
    }

    FString(const wchar_t* Value)
    {
        Max = Count = *Value ? std::wcslen(Value) + 1 : 0;

        if (Count)
        {
            Data = const_cast<wchar_t*>(Value);
        }
    };

    inline bool IsValid()
    {
        return Data != nullptr;
    }

    inline wchar_t* c_str()
    {
        return Data;
    }
};

#endif // STRUCT_H
