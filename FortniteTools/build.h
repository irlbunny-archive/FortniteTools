#ifndef BUILD_H
#define BUILD_H

//#define UE32_4_12 // Use UE 4.12 [32-bit] (Alpha 0.6.5) structs.
//#define UE64_4_20 // Use UE 4.20 [64-bit] (Season 3/4) structs.
#define UE64_4_21 // Use UE 4.21 [64-bit] (Season 5) structs.
//#define UE64_4_22 // Use UE 4.22 [64-bit] (Season 7/8-ish) structs.
//#define UE64_4_23 // Use UE 4.23 [64-bit] (Season 8-ish/9/10) structs.
//#define UE64_4_26 // Use UE 4.26 [64-bit] (Season 13/14) structs.

// Version:
#ifdef UE32_4_12
#define UE_VERSION "4.12 [32-bit]"
#elif defined(UE64_4_20)
#define UE_VERSION "4.20 [64-bit]"
#elif defined(UE64_4_21)
#define UE_VERSION "4.21 [64-bit]"
#elif defined(UE64_4_22)
#define UE_VERSION "4.22 [64-bit]"
#elif defined(UE64_4_23)
#define UE_VERSION "4.23 [64-bit]"
#elif defined(UE64_4_26)
#define UE_VERSION "4.26 [64-bit]"
#else
#define UE_VERSION "Unknown"
#endif

// Options:
//#define PROCESSEVENT_HOOK // Use ProcessEvent hook.

#endif // BUILD_H
