/*
*	Code adapted from:
*	https://github.com/Trxyebeep/TOMB4
*/

#include "../framework.h"
#include "../vars.h"

#include "../game/function_table.h"
#include "../game/OBJECTS.H"

#include "3dmath.h"
#include "d3dmatrix.h"
#include "drawroom.h"
#include "dxshell.h"
#include "lighting.h"
#include "output.h"

// Have to include this for the customize crap...
#include "../../bass.h"
#include "../../Tomb_NextGeneration.h"
#include "../../constants_mine.h"
#include "../../structures.h"
#include "../../structures_mine.h"

#include "../../custenvmap.h"

//extern bool SendToLog(char* szFormat, ...);

SPRITESTRUCT* CustEnvMap(short* objptr)
{
	SPRITESTRUCT* envmap = &spriteinfo[objects[DefaultSpriteSlot].mesh_index + DefaultSpriteID];

	// Partial copy from Get() function to avoid GET monstrosity
	for (int i = 0; i < MyData.BaseCustomizeMine.TotCustomize; i++) 
	{
		if (MyData.BaseCustomizeMine.pVetCustomize[i].CustValue == CUST_ENVMAP)
		{
			StrGenericCustomize* cust = &MyData.BaseCustomizeMine.pVetCustomize[i];
			//SendToLog("ID %d, Slot %d, EnvMapType %d, SpriteSlot %d, SpriteID %d, OcbCode %d, RoomNumber %d, MeshArray %d", cust->pVetArg[EnvMapID], cust->pVetArg[ObjectSlot], cust->pVetArg[EnvMapType], cust->pVetArg[SpriteSlot], cust->pVetArg[SpriteID], cust->pVetArg[OcbCode], cust->pVetArg[RoomNumber], cust->pVetArg[MeshArray]);

			if ((cust->pVetArg[ObjectSlot] == current_item->object_number) &&
				(cust->pVetArg[OcbCode] == current_item->trigger_flags || cust->pVetArg[OcbCode] == -1) &&
				(cust->pVetArg[RoomNumber] == current_item->room_number || cust->pVetArg[RoomNumber] == -1))
			{
				if (cust->pVetArg[EnvMapType] == TENVMAP_SLOT)
				{
					int spriteslot = cust->pVetArg[SpriteSlot];
					int spriteID = cust->pVetArg[SpriteID];

					// Hmm... this doesn't seem to work
					// This should allow TENVMAP_MESH to overwrite TENVMAP_SLOT when possible, but TENVMAP_SLOT can still overwrite mesh settings when placed lower in the script. Why?
					envmap = &spriteinfo[objects[spriteslot].mesh_index + spriteID];
				}
				else if (cust->pVetArg[EnvMapType] == TENVMAP_MESH)
				{
					int nummeshes = (cust->NArguments - MeshArray);

					for (int j = 0; j < nummeshes; j++)
					{
						int spriteslot = cust->pVetArg[SpriteSlot];
						int spriteID = cust->pVetArg[SpriteID];
						int objectslot = cust->pVetArg[ObjectSlot];
						int currentmesh = cust->pVetArg[MeshArray + j];

						if (objptr == meshes[objects[objectslot].mesh_index + (currentmesh * 2)])
						{
							return &spriteinfo[objects[spriteslot].mesh_index + spriteID];
						}
					}
				}				
			}
		}
	}
	
	// no customize command, do default sprite
	return envmap;
}

const float f1 = 2.0F;
const float f2 = 256.0F;

static float w, h;

__declspec(naked) void phd_PutPolygons1()
{
	__asm
	{
		push ebx
		push esi
		push ebp
		call CustEnvMap
		add esp, 4h
		mov ebx, eax
		movzx eax, WORD PTR [ebx + 4h]
		shr eax, 8h
		mov DWORD PTR [w], eax
		fild DWORD PTR [w]
		fdiv DWORD PTR [f1]
		fdiv DWORD PTR [f2]
		fstp DWORD PTR [w]
		movzx eax, WORD PTR [ebx + 6h]
		shr eax, 8h
		mov DWORD PTR [h], eax
		fild DWORD PTR [h]
		fdiv DWORD PTR [f1]
		fdiv DWORD PTR [f2]
		fstp DWORD PTR [h]
		mov edx, 47BB8Ch
		jmp edx
	}
}

__declspec(naked) void phd_PutPolygons2()
{
	__asm
	{
		add esp, 20h
		fld DWORD PTR [esp + 5Ch]
		fmul DWORD PTR [w]
		fstp DWORD PTR [esp + 5Ch]
		fld DWORD PTR [esp + 60h]
		fmul DWORD PTR [h]
		fstp DWORD PTR [esp + 60h]
		fld DWORD PTR [esp + 64h]
		fmul DWORD PTR [w]
		fstp DWORD PTR [esp + 64h]
		fld DWORD PTR [esp + 1Ch]
		fmul DWORD PTR [w]
		fstp DWORD PTR [esp + 1Ch]
		fld DWORD PTR [esp + 20h]
		fmul DWORD PTR [h]
		fstp DWORD PTR [esp + 20h]
		fld DWORD PTR [esp + 24h]
		fmul DWORD PTR [w]
		fstp DWORD PTR [esp + 24h]
		fld DWORD PTR [esp + 34h]
		fmul DWORD PTR [w]
		fstp DWORD PTR [esp + 34h]
		fld DWORD PTR [esp + 38h]
		fmul DWORD PTR [h]
		fstp DWORD PTR [esp + 38h]
		fld DWORD PTR [esp + 3Ch]
		fmul DWORD PTR [w]
		fstp DWORD PTR [esp + 3Ch]
		fld DWORD PTR [esp + 28h]
		fmul DWORD PTR [w]
		fstp DWORD PTR [esp + 28h]
		fld DWORD PTR [esp + 2Ch]
		fmul DWORD PTR [h]
		fstp DWORD PTR [esp + 2Ch]
		fld DWORD PTR [esp + 30h]
		fmul DWORD PTR [w]
		fstp DWORD PTR [esp + 30h]
		mov WORD PTR [esp + 78h], 2h
		mov ax, WORD PTR [ebx]
		mov WORD PTR [esp + 7Ah], ax
		mov ax, WORD PTR [edi + 4h]
		mov WORD PTR [esp + 7Ch], ax
		fld DWORD PTR [esp + 5Ch]
		fadd DWORD PTR [ebx + 8h]
		fadd DWORD PTR [w]
		fstp DWORD PTR [esp + 7Eh]
		fld DWORD PTR [esp + 60h]
		fadd DWORD PTR [ebx + 0Ch]
		fadd DWORD PTR [h]
		fstp DWORD PTR [esp + 82h]
		fld DWORD PTR [esp + 1Ch]
		fadd DWORD PTR [ebx + 8h]
		fadd DWORD PTR [w]
		fstp DWORD PTR [esp + 86h]
		fld DWORD PTR [esp + 20h]
		fadd DWORD PTR [ebx + 0Ch]
		fadd DWORD PTR [h]
		fstp DWORD PTR [esp + 8Ah]
		fld DWORD PTR [esp + 34h]
		fadd DWORD PTR [ebx + 8h]
		fadd DWORD PTR [w]
		fstp DWORD PTR [esp + 8Eh]
		fld DWORD PTR [esp + 38h]
		fadd DWORD PTR [ebx + 0Ch]
		fadd DWORD PTR [h]
		fstp DWORD PTR [esp + 92h]
		fld DWORD PTR [esp + 28h]
		fadd DWORD PTR [ebx + 8h]
		fadd DWORD PTR [w]
		fstp DWORD PTR [esp + 96h]
		fld DWORD PTR [esp + 2Ch]
		fadd DWORD PTR [ebx + 0Ch]
		fadd DWORD PTR [h]
		fstp DWORD PTR [esp + 9Ah]
		mov edx, 47BF60h
		jmp edx
	}
}

__declspec(naked) void phd_PutPolygons3()
{
	__asm
	{
		add esp, 18h
		mov BYTE PTR [esp + 17h], 1h
		fld DWORD PTR [esp + 28h]
		fmul DWORD PTR [w]
		fstp DWORD PTR [esp + 28h]
		fld DWORD PTR [esp + 2Ch]
		fmul DWORD PTR [h]
		fstp DWORD PTR [esp + 2Ch]
		fld DWORD PTR [esp + 30h]
		fmul DWORD PTR [w]
		fstp DWORD PTR [esp + 30h]
		fld DWORD PTR [esp + 34h]
		fmul DWORD PTR [w]
		fstp DWORD PTR [esp + 34h]
		fld DWORD PTR [esp + 38h]
		fmul DWORD PTR [h]
		fstp DWORD PTR [esp + 38h]
		fld DWORD PTR [esp + 3Ch]
		fmul DWORD PTR [w]
		fstp DWORD PTR [esp + 3Ch]
		fld DWORD PTR [esp + 1Ch]
		fmul DWORD PTR [w]
		fstp DWORD PTR [esp + 1Ch]
		fld DWORD PTR [esp + 20h]
		fmul DWORD PTR [h]
		fstp DWORD PTR [esp + 20h]
		fld DWORD PTR [esp + 24h]
		fmul DWORD PTR [w]
		fstp DWORD PTR [esp + 24h]
		mov WORD PTR [esp + 78h], 2h
		mov ax, WORD PTR [ebx]
		mov WORD PTR [esp + 7Ah], ax
		mov ax, WORD PTR [edi + 4h]
		mov WORD PTR [esp + 7Ch], ax
		fld DWORD PTR [esp + 28h]
		fadd DWORD PTR [ebx + 8h]
		fadd DWORD PTR [w]
		fstp DWORD PTR [esp + 7Eh]
		fld DWORD PTR [esp + 2Ch]
		fadd DWORD PTR [ebx + 0Ch]
		fadd DWORD PTR [h]
		fstp DWORD PTR [esp + 82h]
		fld DWORD PTR [esp + 34h]
		fadd DWORD PTR [ebx + 8h]
		fadd DWORD PTR [w]
		fstp DWORD PTR [esp + 86h]
		fld DWORD PTR [esp + 38h]
		fadd DWORD PTR [ebx + 0Ch]
		fadd DWORD PTR [h]
		fstp DWORD PTR [esp + 8Ah]
		fld DWORD PTR [esp + 1Ch]
		fadd DWORD PTR [ebx + 8h]
		fadd DWORD PTR [w]
		fstp DWORD PTR [esp + 8Eh]
		fld DWORD PTR [esp + 20h]
		fadd DWORD PTR [ebx + 0Ch]
		fadd DWORD PTR [h]
		fstp DWORD PTR [esp + 92h]
		mov edx, 47C5D0h
		jmp edx
	}
}

void inject_output(bool replace)
{
	INJECT(0x0047BB4E, phd_PutPolygons1, replace);
	INJECT(0x0047BDCE, phd_PutPolygons2, replace);
	INJECT(0x0047C49B, phd_PutPolygons3, replace);
}
