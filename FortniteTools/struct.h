#ifndef STRUCT_H
#define STRUCT_H

#include "build.h"

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

	FString(wchar_t* Value)
	{
		Max = Count = static_cast<int>(wcslen(Value));

		if (Count)
			Data = const_cast<wchar_t*>(Value);
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

struct UClass
{
    // TODO
};

#if defined (UE32_4_12)
// FIXME(Cyuubi): Probably incorrect.
struct UObject
{
	char		Unk00[0x4];		// 0x0000
	int			ObjectFlags;	// 0x0004
	int			InternalIndex;	// 0x0008
	UClass*		ClassPrivate;	// 0x000C
	char		Unk01[0x4];		// 0x0010
	UObject*	OuterPrivate;	// 0x0014
};
#else
struct UObject
{
	char			Unk00[0x8];		// 0x0000
	unsigned long	ObjectFlags;	// 0x0008
	unsigned long	InternalIndex;	// 0x0010
	UClass*			ClassPrivate;	// 0x0018
	char			Unk01[0x8];		// 0x0020
	UObject*		OuterPrivate;	// 0x0028
};
#endif

struct UConsole
{
    // TODO
};

struct UGameInstance
{
	// TODO
};

#if defined (UE32_4_12)
struct UGameViewportClient
{
    char		Unk00[0x24];		// 0x0000
    UConsole*	ViewportConsole;	// 0x0024
};
#else
struct UGameViewportClient
{
    char		Unk00[0x40];		// 0x0000
    UConsole*	ViewportConsole;	// 0x0040
};
#endif

#if defined (UE32_4_12)
struct UEngine
{
    char                    Unk00[0x90];    // 0x0000
    UClass*                 ConsoleClass;   // 0x0090
    char                    Unk01[0x408];   // 0x0094
    UGameViewportClient*    GameViewport;   // 0x049C
};
#elif defined(UE64_4_23)
struct UEngine
{
    char                    Unk00[0xF8];    // 0x0000
    UClass*                 ConsoleClass;   // 0x00F8
    char                    Unk01[0x650];   // 0x0100
    UGameViewportClient*    GameViewport;   // 0x0750
};
#elif defined(UE64_4_26)
struct UEngine
{
    char                    Unk00[0xF8];    // 0x0000
    UClass*                 ConsoleClass;   // 0x00F8
    char                    Unk01[0x688];   // 0x0100
    UGameViewportClient*    GameViewport;   // 0x0788
};
#endif

#endif // STRUCT_H
