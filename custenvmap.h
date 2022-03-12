#pragma once

// Plugin_EnvMap.cpp
extern int DefaultSpriteSlot;
extern int DefaultSpriteID;
extern StrMyData MyData;


enum cust_envmap_commands {
	CUST_ENVMAP = 1,
	CUST_ENVMAP_GLOBAL
};

enum cust_envmap {
	EnvMapID, // Useless but NG Center requires it
	ObjectSlot,
	EnvMapType,
	SpriteSlot,
	SpriteID,
	OcbCode,
	RoomNumber,
	MeshArray // Should always be last
};

enum cust_envmap_type {
	TENVMAP_SLOT,
	TENVMAP_MESH
};

enum cust_envmap_global {
	GlobalSpriteSlot,
	GlobalSpriteID
};