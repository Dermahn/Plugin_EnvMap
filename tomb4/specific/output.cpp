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

void phd_PutPolygons(short* objptr, long clip)
{
	MESH_DATA* mesh;
	SPRITESTRUCT* envmap_sprite;
	D3DTLVERTEX* v;
	TEXTURESTRUCT* pTex;
	D3DVECTOR normals[4];
	TEXTURESTRUCT envmap_texture;
	short* quad;
	short* tri;
	long clrbak[4];
	long spcbak[4];
	long num;
	ushort drawbak;
	bool envmap;
	float w, h; // new variables

	SetD3DViewMatrix();
	mesh = (MESH_DATA*)objptr;


	if (objptr)
	{
		// We don't need this. It gets overwritten anyway.
		/*
		if (objptr == meshes[objects[LARA_DOUBLE].mesh_index] || objptr == meshes[objects[LARA_DOUBLE].mesh_index + 2])
			envmap_sprite = &spriteinfo[objects[SKY_GRAPHICS].mesh_index];
		else
			envmap_sprite = &spriteinfo[objects[DEFAULT_SPRITES].mesh_index + 11];
		*/

		envmap_sprite = CustEnvMap(objptr);

		// Calculate sprite size to allow for arbitrary sprite sizes. No longer limited to 64x64
		w = float(envmap_sprite->width >> 8);
		w /= 2.0F;                                //half the size
		w /= 256.0F;                            //by the tpage size
		h = float(envmap_sprite->height >> 8);
		h /= 2.0F;
		h /= 256.0F;

		if (GlobalAmbient)
		{
			ClearObjectLighting();
			ClearDynamicLighting();
			App.dx.lpD3DDevice->SetLightState(D3DLIGHTSTATE_AMBIENT, GlobalAmbient);
			GlobalAmbient = 0;
		}
		else if (mesh->prelight)
		{
			ClearObjectLighting();
			InitItemDynamicLighting(current_item);
		}
		else
			InitObjectLighting(current_item);

		if (mesh->nVerts)
			DXAttempt(DestVB->ProcessVertices(D3DVOP_LIGHT | D3DVOP_TRANSFORM, 0, mesh->nVerts, mesh->SourceVB, 0, App.dx._lpD3DDevice, 0));

		DestVB->Lock(DDLOCK_READONLY, (void**)&v, 0);
		clip_left = f_left;
		clip_top = f_top;
		clip_right = f_right;
		clip_bottom = f_bottom;

		if (mesh->nVerts)
		{
			bWaterEffect = camera.underwater;
			ProjectVerts(mesh->nVerts, v, clipflags);

			if (mesh->prelight)
				PrelightVerts(mesh->nVerts, v, mesh);
			else
				CalcVertsColorSplitMMX(mesh->nVerts, v);
		}

		quad = mesh->gt4;

		for (int i = 0; i < mesh->ngt4; i++, quad += 6)
		{
			pTex = &textinfo[quad[4] & 0x7FFF];
			envmap = 0;
			drawbak = pTex->drawtype;

			if (quad[5] & 1)
				pTex->drawtype = 2;

			if (quad[5] & 2)
			{
				envmap = 1;
				num = (quad[5] >> 2) & 0x1F;
				normals[0] = mesh->Normals[quad[0]];
				normals[1] = mesh->Normals[quad[1]];
				normals[2] = mesh->Normals[quad[2]];
				normals[3] = mesh->Normals[quad[3]];
				D3DTransform(&normals[0], &D3DMView);
				D3DTransform(&normals[1], &D3DMView);
				D3DTransform(&normals[2], &D3DMView);
				D3DTransform(&normals[3], &D3DMView);

				for (int i = 0; i < 4; i++)
				{
					normals[i].x *= w;
					normals[i].y *= h;
					normals[i].z *= w;
				}

				envmap_texture.drawtype = 2;
				envmap_texture.flag = pTex->flag;
				envmap_texture.tpage = envmap_sprite->tpage;
				envmap_texture.u1 = normals[0].x + envmap_sprite->x1 + w;
				envmap_texture.v1 = normals[0].y + envmap_sprite->y1 + h;
				envmap_texture.u2 = normals[1].x + envmap_sprite->x1 + w;
				envmap_texture.v2 = normals[1].y + envmap_sprite->y1 + h;
				envmap_texture.u3 = normals[2].x + envmap_sprite->x1 + w;
				envmap_texture.v3 = normals[2].y + envmap_sprite->y1 + h;
				envmap_texture.u4 = normals[3].x + envmap_sprite->x1 + w;
				envmap_texture.v4 = normals[3].y + envmap_sprite->y1 + h;
			}

			if (GlobalAlpha == 0xFF000000)
			{
				if (!pTex->drawtype)
					AddQuadZBuffer(v, quad[0], quad[1], quad[2], quad[3], pTex, 0);
				else if (pTex->drawtype <= 2)
					AddQuadSorted(v, quad[0], quad[1], quad[2], quad[3], pTex, 0);

				if (envmap)
				{
					clrbak[0] = v[quad[0]].color;
					clrbak[1] = v[quad[1]].color;
					clrbak[2] = v[quad[2]].color;
					clrbak[3] = v[quad[3]].color;
					spcbak[0] = v[quad[0]].specular;
					spcbak[1] = v[quad[1]].specular;
					spcbak[2] = v[quad[2]].specular;
					spcbak[3] = v[quad[3]].specular;
					RGB_M(v[quad[0]].color, num);
					RGB_M(v[quad[1]].color, num);
					RGB_M(v[quad[2]].color, num);
					RGB_M(v[quad[3]].color, num);
					RGB_M(v[quad[0]].specular, num);
					RGB_M(v[quad[1]].specular, num);
					RGB_M(v[quad[2]].specular, num);
					RGB_M(v[quad[3]].specular, num);
					AddQuadSorted(v, quad[0], quad[1], quad[2], quad[3], &envmap_texture, 0);
					v[quad[0]].color = clrbak[0];
					v[quad[1]].color = clrbak[1];
					v[quad[2]].color = clrbak[2];
					v[quad[3]].color = clrbak[3];
					v[quad[0]].specular = spcbak[0];
					v[quad[1]].specular = spcbak[1];
					v[quad[2]].specular = spcbak[2];
					v[quad[3]].specular = spcbak[3];
				}
			}
			else
			{
				pTex->drawtype = 7;
				AddQuadSorted(v, quad[0], quad[1], quad[2], quad[3], pTex, 0);
			}

			pTex->drawtype = drawbak;
		}

		tri = mesh->gt3;

		for (int i = 0; i < mesh->ngt3; i++, tri += 5)
		{
			pTex = &textinfo[tri[3] & 0x7FFF];
			envmap = 0;
			drawbak = pTex->drawtype;

			if (tri[4] & 1)
				pTex->drawtype = 2;

			if (tri[4] & 2)
			{
				envmap = 1;
				num = (tri[4] >> 2) & 0x1F;
				normals[0] = mesh->Normals[tri[0]];
				normals[1] = mesh->Normals[tri[1]];
				normals[2] = mesh->Normals[tri[2]];
				D3DTransform(&normals[0], &D3DMView);
				D3DTransform(&normals[1], &D3DMView);
				D3DTransform(&normals[2], &D3DMView);

				for (int i = 0; i < 3; i++)
				{
					normals[i].x *= w;
					normals[i].y *= h;
					normals[i].z *= w;
				}

				envmap_texture.drawtype = 2;
				envmap_texture.flag = pTex->flag;
				envmap_texture.tpage = envmap_sprite->tpage;
				envmap_texture.u1 = normals[0].x + envmap_sprite->x1 + w;
				envmap_texture.v1 = normals[0].y + envmap_sprite->y1 + h;
				envmap_texture.u2 = normals[1].x + envmap_sprite->x1 + w;
				envmap_texture.v2 = normals[1].y + envmap_sprite->y1 + h;
				envmap_texture.u3 = normals[2].x + envmap_sprite->x1 + w;
				envmap_texture.v3 = normals[2].y + envmap_sprite->y1 + h;
			}

			if (GlobalAlpha == 0xFF000000)
			{
				if (!pTex->drawtype)
					AddTriZBuffer(v, tri[0], tri[1], tri[2], pTex, 0);
				else if (pTex->drawtype <= 2)
					AddTriSorted(v, tri[0], tri[1], tri[2], pTex, 0);

				if (envmap)
				{
					clrbak[0] = v[tri[0]].color;
					clrbak[1] = v[tri[1]].color;
					clrbak[2] = v[tri[2]].color;
					spcbak[0] = v[tri[0]].specular;
					spcbak[1] = v[tri[1]].specular;
					spcbak[2] = v[tri[2]].specular;
					RGB_M(v[tri[0]].color, num);
					RGB_M(v[tri[1]].color, num);
					RGB_M(v[tri[2]].color, num);
					RGB_M(v[tri[0]].specular, num);
					RGB_M(v[tri[1]].specular, num);
					RGB_M(v[tri[2]].specular, num);
					AddTriSorted(v, tri[0], tri[1], tri[2], &envmap_texture, 0);
					v[tri[0]].color = clrbak[0];
					v[tri[1]].color = clrbak[1];
					v[tri[2]].color = clrbak[2];
					v[tri[0]].specular = spcbak[0];
					v[tri[1]].specular = spcbak[1];
					v[tri[2]].specular = spcbak[2];
				}
			}
			else
			{
				pTex->drawtype = 7;
				AddTriSorted(v, tri[0], tri[1], tri[2], pTex, 0);
			}

			pTex->drawtype = drawbak;
		}

		DestVB->Unlock();
	}
}

void inject_output(bool replace)
{
	INJECT(0x0047bb30, phd_PutPolygons, replace);
}