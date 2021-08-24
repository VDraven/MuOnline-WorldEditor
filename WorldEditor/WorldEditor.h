#pragma once

#ifndef WORLD_EDITOR_H
#define WORLD_EDITOR_H

#include "WorldEditorOffset.h"

class WorldEditor
{
public:
	WorldEditor() { };
	virtual ~WorldEditor() {};

	static WorldEditor* Instance()
	{
		static WorldEditor instance;
		return &instance;
	}

	void Init();

private:
	void SetHooks();
	void Refresh();
	void LoadPluginConfig();
	void LoadExtData();
	void LoadExtTile();
	void LoadExtObject();
	void SetFreeMode();

	void InitUI();
	void UI();
	void UI_SaveWorld();
	void UI_EditObject();
	void UI_EditFlag_1();
	void UI_EditFlag_2();
	void UI_EditFlag_2_Local();
	void UI_EditFlag_2_All();
	void UI_EditFlag_3();
	void UI_EditFlag_4();
	void UI_EditFlag_5();
	void UI_EditFlag_6();
	void UI_EditFlag_7();

	void InitEdit();
	void Edit();
	void EditObjects();
	void EditCamera();

	void SaveWorld();
	void SaveTerrainLight(const char*, const char*);
	void SaveTerrainHeight(const char*, const char*);
	void SaveTerrainHeightNew(const char*, const char*);
	void SaveTerrainMapping(const char*, const char*, int);
	void SaveTerrainAttribute(const char*, const char*, int);
	void SaveObjects(const char*, const char*, int);

	LRESULT WndProcHandler(HWND, UINT, WPARAM, LPARAM);

	friend LRESULT CALLBACK HookWndProc(HWND, UINT, WPARAM, LPARAM);
	friend void HookRenderMuEditor();
	friend void HookRenderTerrainTile(float, float, int, int, float, int, bool);
	friend void HookOpenObj();

	OBJECT* CollisionDetectObjects();
	bool CollisionDetectLineToMesh(BMD*, vec3_t, vec3_t, bool, int, int);
	void AddTerrainHeight(float, float, float, int, float*);
	void SetTerrainHeight(float, float, float, int, float*);
	void SetTerrainLight(float xf, float yf, vec3_t Light, int Range, vec3_t* Buffer);
	void SetTerrainWall(float xf, float yf, int Range, bool set);
	bool RenderTerrainTile(float xf, float yf, int xi, int yi, float lodf, int lodi, bool Flag);
	void RenderPreview3D(int, float, float, float, float, int v = 0); 

private:
	//============================
	struct
	{
		bool Active;
		std::string DirSave;
	} PluginConfig;

	struct 
	{
		std::string DirTile;
		std::map<std::string, GLuint> Tiles;
		
		std::string DirObject;
		std::map<std::string, DWORD> Objects;
	} ExternalData;

	struct
	{
		bool Active;
		float Zoom;
		float Angle[3];
		float PosX;
		float PosY;
		float Width;
		float Height;
		int CenterView;
	} Frame3D;

	struct
	{
		int nWorld;
		int nWorldID;

		ATT_TYPE SaveTerrainAttributeType;
		OZB_TYPE SaveTerrainHeightType;
		OBJ_TYPE SaveObjectsType;
	} SaveWorldConfig;

	bool FreeMode;
	bool BrushRing;
	bool SelectRing;
	bool SelectLock;
	bool HeightLock;
	bool GridLock;
	bool FlatLock;
	WORD MonsterKey;
	int BrushSize;
	int SelectMonsterType;
	int SelectTileType;
	int SelectWallType;
	int SelectObjectType;
	float FlatHeight;
	float PickObjectHeight;
	float PickObjectScale;
	vec3_t PickObjectAngle;
	vec3_t HeroPos;
	vec3_t HeroPosOld;
	vec3_t SelectLightColor;
	vec4_t SelectWallColor;
	OBJECT* PickObject;
	CHARACTER* PickMonster;

	enum
	{
		EDIT_NONE = 0,
		EDIT_MAPPING = 1,
		EDIT_OBJECT = 2,
		EDIT_WALL = 3,
		EDIT_HEIGHT = 4,
		EDIT_LIGHT = 5,
		EDIT_SOUND = 6,
		EDIT_MONSTER = 7
	};

	
};

#endif