#pragma once
#include "../vars.h"

void inject_output(bool replace);

#define PrelightVerts   ( (void(__cdecl*)(long, D3DTLVERTEX*, MESH_DATA*)) 0x0047b9f0 )
#define CalcVertsColorSplitMMX ( (void(__cdecl*)(long, D3DTLVERTEX*)) 0x0047b900 )