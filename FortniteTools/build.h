#ifndef BUILD_H
#define BUILD_H

#define UE32_4_12 // Use UE 4.12 [32-bit] (Alpha) structs.
//#define UE64_4_23 // Use UE 4.23 [64-bit] (Season 10) structs.
//#define UE64_4_26 // Use UE 4.26 [64-bit] (Season 13/14) structs.

// Version:
#if defined(UE32_4_12)
#define UE_VERSION "4.12 [32-bit]"
#elif defined(UE64_4_23)
#define UE_VERSION "4.23 [64-bit]"
#elif defined(UE64_4_26)
#define UE_VERSION "4.26 [64-bit]"
#else
#define UE_VERSION "Unknown"
#endif

// Options:
//#define S14_OFFSETS // Use Season 14 offsets for UE 4.26 [64-bit].
//#define PROCESSEVENT_HOOK // Use ProcessEvent hook.

#endif // BUILD_H
