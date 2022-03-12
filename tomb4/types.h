#pragma once
#pragma pack(push, 1)

/*typedefs*/
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

/*Injection macro, originally by Arsunt, modified by ChocolateFan to allow deinjection*/
struct JMP
{
	BYTE opCode;	// must be 0xE9;
	DWORD offset;	// jump offset
};

#define INJECT(from,to,replace) \
do \
{ \
	if (replace) \
		INJECT_JMP(from,to); \
	else \
		INJECT_JMP(to,from); \
} while (false)

#define INJECT_JMP(from,to) \
do \
{ \
	((JMP*)(from))->opCode = 0xE9; \
	((JMP*)(from))->offset = (DWORD)(to) - ((DWORD)(from) + sizeof(JMP)); \
} while (false)


#define	CLRR(clr)	((clr >> 16) & 0xFF)	//shift g and b out of the way and 0xFF
#define	CLRG(clr)	((clr >> 8) & 0xFF)		//shift b out of the way and 0xFF
#define	CLRB(clr)	((clr) & 0xFF)			//and 0xFF
#define RGB_M(clr, m)	(clr = (clr & 0xFF000000) | (((CLRR(clr) * 8 * m) >> 8) << 16) | (((CLRG(clr) * 8 * m) >> 8) << 8) | ((CLRB(clr) * 8 * m) >> 8))


enum camera_type
{
	CHASE_CAMERA,
	FIXED_CAMERA,
	LOOK_CAMERA,
	COMBAT_CAMERA,
	CINEMATIC_CAMERA,
	HEAVY_CAMERA,
};


struct PHD_VECTOR
{
	long x;
	long y;
	long z;
};

struct PHD_3DPOS
{
	long x_pos;
	long y_pos;
	long z_pos;
	short x_rot;
	short y_rot;
	short z_rot;
};

// rename due to TRNG fuckery
struct GAME_VECTORTR4
{
	long x;
	long y;
	long z;
	short room_number;
	short box_number;
};

struct OBJECT_VECTOR
{
	long x;
	long y;
	long z;
	short data;
	short flags;
};

struct PCLIGHT
{
	float x;
	float y;
	float z;
	float r;
	float g;
	float b;
	long shadow;
	float Inner;
	float Outer;
	float InnerAngle;
	float OuterAngle;
	float Cutoff;
	float nx;
	float ny;
	float nz;
	long ix;
	long iy;
	long iz;
	long inx;
	long iny;
	long inz;
	float tr;
	float tg;
	float tb;
	float rs;
	float gs;
	float bs;
	long fcnt;
	uchar Type;
	uchar Active;
	PHD_VECTOR rlp;
	long Range;
};

struct ITEM_LIGHT
{
	long r;
	long g;
	long b;
	long ambient;
	long rs;
	long gs;
	long bs;
	long fcnt;
	PCLIGHT	CurrentLights[21];
	PCLIGHT	PrevLights[21];
	long nCurrentLights;
	long nPrevLights;
	long room_number;
	long RoomChange;
	PHD_VECTOR item_pos;
	void* pCurrentLights;
	void* pPrevLights;
};

struct MESH_DATA
{
	short x;
	short y;
	short z;
	short r;
	short flags;
	short nVerts;
	short nNorms;
	short ngt4;
	short* gt4;
	short ngt3;
	short* gt3;
	long* prelight;
	LPDIRECT3DVERTEXBUFFER SourceVB;
	D3DVECTOR* Normals;
};

struct SPRITESTRUCT
{
	ushort tpage;
	ushort offset;
	ushort width;
	ushort height;
	float x1;	//left
	float y1;	//top
	float x2;	//right
	float y2;	//bottom
};

struct TEXTURESTRUCT
{
	ushort drawtype;
	ushort tpage;
	ushort flag;
	float u1;
	float v1;
	float u2;
	float v2;
	float u3;
	float v3;
	float u4;
	float v4;
};

struct ITEM_INFO
{
	long floor;
	ulong touch_bits;
	ulong mesh_bits;
	short object_number;
	short current_anim_state;
	short goal_anim_state;
	short required_anim_state;
	short anim_number;
	short frame_number;
	short room_number;
	short next_item;
	short next_active;
	short speed;
	short fallspeed;
	short hit_points;
	ushort box_number;
	short timer;
	short flags;
	short shade;
	short trigger_flags;
	short carried_item;
	short after_death;
	ushort fired_weapon;
	short item_flags[4];
	void* data;
	PHD_3DPOS pos;
	ITEM_LIGHT il;
	ulong active : 1;
	ulong status : 2;
	ulong gravity_status : 1;
	ulong hit_status : 1;
	ulong collidable : 1;
	ulong looked_at : 1;
	ulong dynamic_light : 1;
	ulong poisoned : 1;
	ulong ai_bits : 5;
	ulong really_active : 1;
	ulong InDrawRoom : 1;	//I don't think this exists in TR4
	ulong meshswap_meshbits;
	short draw_room;
	short TOSSPAD;
};

struct COLL_INFO
{
	long mid_floor;
	long mid_ceiling;
	long mid_type;
	long front_floor;
	long front_ceiling;
	long front_type;
	long left_floor;
	long left_ceiling;
	long left_type;
	long right_floor;
	long right_ceiling;
	long right_type;
	long left_floor2;
	long left_ceiling2;
	long left_type2;
	long right_floor2;
	long right_ceiling2;
	long right_type2;
	long radius;
	long bad_pos;
	long bad_neg;
	long bad_ceiling;
	PHD_VECTOR shift;
	PHD_VECTOR old;
	short old_anim_state;
	short old_anim_number;
	short old_frame_number;
	short facing;
	short quadrant;
	short coll_type;
	short* trigger;
	char tilt_x;
	char tilt_z;
	char hit_by_baddie;
	char hit_static;
	ushort slopes_are_walls : 2;
	ushort slopes_are_pits : 1;
	ushort lava_is_pit : 1;
	ushort enable_baddie_push : 1;
	ushort enable_spaz : 1;
	ushort hit_ceiling : 1;
};

struct OBJECT_INFO
{
	short nmeshes;
	short mesh_index;
	long bone_index;
	short* frame_base;
	void (*initialise)(short item_number);
	void (*control)(short item_number);
	void (*floor)(ITEM_INFO* item, long x, long y, long z, long* height);
	void (*ceiling)(ITEM_INFO* item, long x, long y, long z, long* height);
	void (*draw_routine)(ITEM_INFO* item);
	void (*collision)(short item_num, ITEM_INFO* laraitem, COLL_INFO* coll);
	short object_mip;
	short anim_index;
	short hit_points;
	short pivot_length;
	short radius;
	short shadow_size;
	ushort bite_offset;
	ushort loaded : 1;
	ushort intelligent : 1;
	ushort non_lot : 1;
	ushort save_position : 1;
	ushort save_hitpoints : 1;
	ushort save_flags : 1;
	ushort save_anim : 1;
	ushort semi_transparent : 1;
	ushort water_creature : 1;
	ushort using_drawanimating_item : 1;
	ushort HitEffect : 2;
	ushort undead : 1;
	ushort save_mesh : 1;
	void (*draw_routine_extra)(ITEM_INFO* item);
	ulong explodable_meshbits;
	ulong padfuck;
};

struct DXPTR
{
	LPDIRECTDRAW4 lpDD;
	LPDIRECT3D3 lpD3D;
	LPDIRECT3DDEVICE3 lpD3DDevice;
	LPDIRECT3DDEVICE3 _lpD3DDevice;
	LPDIRECTDRAWSURFACE4 lpPrimaryBuffer;
	LPDIRECTDRAWSURFACE4 lpBackBuffer;
	LPDIRECTDRAWSURFACE4 lpZBuffer;
	LPDIRECT3DVIEWPORT3 lpViewport;
	LPDIRECTSOUND lpDS;
	ulong dwRenderWidth;
	ulong dwRenderHeight;
	RECT rViewport;
	RECT rScreen;
	long Flags;
	long WindowStyle;
	long CoopLevel;
	IDirectInput2* lpDirectInput;
	IDirectInputDevice2* Keyboard;
	HWND hWnd;
	volatile long InScene;
	volatile long WaitAtBeginScene;
	volatile long DoneBlit;
};

struct DXDISPLAYMODE
{
	long w;
	long h;
	long bpp;
	long RefreshRate;
	long bPalette;
	DDSURFACEDESC2 ddsd;
	uchar rbpp;
	uchar gbpp;
	uchar bbpp;
	uchar rshift;
	uchar gshift;
	uchar bshift;

};

struct DXTEXTUREINFO
{
	DDPIXELFORMAT ddpf;
	ulong bpp;
	long bPalette;
	long bAlpha;
	uchar rbpp;
	uchar gbpp;
	uchar bbpp;
	uchar abpp;
	uchar rshift;
	uchar gshift;
	uchar bshift;
	uchar ashift;
};

struct DXZBUFFERINFO
{
	DDPIXELFORMAT ddpf;
	ulong bpp;
};

struct DXD3DDEVICE
{
	char Name[30];
	char About[80];
	LPGUID lpGuid;
	GUID Guid;
	D3DDEVICEDESC DeviceDesc;
	long bHardware;
	long nDisplayModes;
	DXDISPLAYMODE* DisplayModes;
	long nTextureInfos;
	DXTEXTUREINFO* TextureInfos;
	long nZBufferInfos;
	DXZBUFFERINFO* ZBufferInfos;
};


struct DXDIRECTDRAWINFO
{
	char Name[30];
	char About[80];
	LPGUID lpGuid;
	GUID Guid;
	DDCAPS DDCaps;
	DDDEVICEIDENTIFIER DDIdentifier;
	int	 nDisplayModes;
	DXDISPLAYMODE* DisplayModes;
	int nD3DDevices;
	DXD3DDEVICE* D3DDevices;

};

struct DXDIRECTSOUNDINFO
{
	char Name[30];
	char About[80];
	LPGUID lpGuid;
	GUID Guid;
};

struct DXINFO
{
	long nDDInfo;
	long nDSInfo;
	DXDIRECTDRAWINFO* DDInfo;
	DXDIRECTSOUNDINFO* DSInfo;
	long nDD;
	long nD3D;
	long nDisplayMode;
	long nTexture;
	long nZBuffer;
	long nDS;
	bool bHardware;
};

struct WINAPP
{
	HINSTANCE hInstance;
	HWND hWnd;
	WNDCLASS WindowClass;
	DXINFO DXInfo;
	DXPTR dx;
	HANDLE mutex;
	float fps;
	LPDIRECT3DMATERIAL3 GlobalMaterial;
	D3DMATERIALHANDLE GlobalMaterialHandle;
	HACCEL hAccel;
	bool SetupComplete;
	bool BumpMapping;
	long TextureSize;
	long BumpMapSize;
	bool mmx;
	bool Filtering;
	bool Volumetric;
	bool SoundDisabled;
	long StartFlags;
	volatile bool fmv;
	long Desktopbpp;
	long AutoTarget;
};

struct CAMERA_INFO
{
	GAME_VECTORTR4 pos;
	GAME_VECTORTR4 target;
	camera_type type;
	camera_type old_type;
	long shift;
	long flags;
	long fixed_camera;
	long number_frames;
	long bounce;
	long underwater;
	long target_distance;
	short target_angle;
	short target_elevation;
	short actual_elevation;
	short actual_angle;
	short number;
	short last;
	short timer;
	short speed;
	ITEM_INFO* item;
	ITEM_INFO* last_item;
	OBJECT_VECTOR* fixed;
	long mike_at_lara;
	PHD_VECTOR mike_pos;
};


/* Copy of TRNG stuff to avoid having to include any of the TRNG files 
   and deailing with the problems that come with that */
/*
#define MAX_MYPROGR_ACTIONS 100

typedef struct StrRectXY {
	RECT Rect;
	int XCord;
	int YCord;
}RectXY;

typedef struct StrTwoBytes {
	BYTE AlfaByte;
	BYTE BetaByte;
}DueByteFields;

typedef struct StrPosizione {
	int OrgX;
	int OrgY;
	int OrgZ;
}PosizioneFields;

typedef struct StrDoublePosition {
	StrPosizione From;
	StrPosizione To;
}DoublePositionFields;

typedef struct StrProgressiveAction {
	WORD ActionType;  // AXN_ code

	short ItemIndex;  // Index of item to manage
	WORD Arg1;  // durate of the action: number of frames or 0xffff for endless
	union {
		WORD Arg2;    //  variable for customizable targets
		StrTwoBytes Bytes;
	};
	union {
		int  VetArg[6];  // 6 numbers of int type (32 signed bits)
		float VetArgFloat[6];
		WORD VetArgWord[12];
		short VetArgShort[12];
		BYTE VetArgBytes[24];
		char VetArgSignedBytes[24];
		DWORD VetArgDword[6];
		StrDoublePosition Coords;
		StrRectXY Box;

	};
}ProgressiveActionFields;

typedef struct StrSavegameGlobalData {
	// FOR_YOU:
	// define here your variables that you wish were saved (and then restored) to/from savegame in GLOBAL section 
	//           (only one for all levels)
	// note: the size of this structure should be always even (if you add BYTE variable, remember to compensate that 
	//       with another BYTE vairable or placefolder)


}SavegameGlobalDataFields;

typedef struct StrSavegameLocalData {
	// FOR_YOU:
	// define here your variables that you wish were saved (and then restored) to/from savegame in LOCAL section (one for each different level)
	// note: the size of this structure should be always even (if you add BYTE variable, compensate it with another BYTE vairable or placefolder)

}SavegameLocalDataFields;

typedef struct StrSavegameData {
	StrSavegameGlobalData Global;
	StrSavegameLocalData  Local;
}SavegameDataFields;

typedef struct StrMyData {
	StrSavegameData Save;  // variable that it will be saved and restored to/from savegame
	// FOR_YOU:
	// define here all your global variables, i.e. those variables that will be seen from all procedures and keep 
	// their value between all cycles of the game.
	// after you defined a variabile like:
	// int  Alfa;
	// then you'll be able to access to them using syntax like:
	// MyData.Alfa = 3;
	// if (MyData.Alfa == 5) .
	int TotProgrActions;
	int LastProgrActionIndex; // used in case of overloading

	StrProgressiveAction VetProgrActions[MAX_MYPROGR_ACTIONS];
	StrBaseGenericCustomize BaseCustomizeMine;  // stored all your customize script commands for current level
	StrBaseGenericParameters BaseParametersMine; // store of all your parameters= script commands of current level
	StrBaseAssignSlotMine BaseAssignSlotMine; // stored all AssignSlot= command script commands of current level
}MyDataFields;
*/