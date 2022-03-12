#pragma once
#include "../vars.h"

#define ClearObjectLighting	( (void(__cdecl*)()) 0x00477230  )
#define ClearDynamicLighting	( (void(__cdecl*)()) 0x00477900  )
#define InitItemDynamicLighting	( (void(__cdecl*)(ITEM_INFO*)) 0x00477560  )
#define InitObjectLighting	( (void(__cdecl*)(ITEM_INFO*)) 0x00477040  )