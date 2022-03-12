#pragma once
#include "types.h"

#define VAR_U_(address, type)			(*(type*)(address))
#define ARRAY_(address, type, length)	(*(type(*)length)(address))


#define GlobalAlpha					VAR_U_(0x004ab7b8, long)
#define clip_bottom					VAR_U_(0x0052a21c, float)
#define clip_right					VAR_U_(0x0052a900, float)
#define clip_top					VAR_U_(0x0052a904, float)
#define clip_left					VAR_U_(0x0052a908, float)
#define bWaterEffect				VAR_U_(0x0052a90c, long)
#define objects						ARRAY_(0x0052B720, OBJECT_INFO, [465])
#define meshes						VAR_U_(0x00533950, short**)
#define textinfo					VAR_U_(0x00533990, TEXTURESTRUCT*)
#define spriteinfo					VAR_U_(0x00533994, SPRITESTRUCT*)
#define current_item				VAR_U_(0x005b7378, ITEM_INFO*)
#define GlobalAmbient				VAR_U_(0x005c0344, long)
#define DestVB						VAR_U_(0x007537e8, LPDIRECT3DVERTEXBUFFER)
#define clipflags					VAR_U_(0x00753854, short*)
#define App							VAR_U_(0x00753858, WINAPP)
#define D3DMView					VAR_U_(0x00753af8, D3DMATRIX)
#define f_right						VAR_U_(0x00753bac, float)
#define f_left						VAR_U_(0x00753c20, float)
#define f_top						VAR_U_(0x00753c64, float)
#define f_bottom					VAR_U_(0x00754034, float)
#define camera						VAR_U_(0x007fe700, CAMERA_INFO)