#include "WorldEditor.h"

void HookRenderMuEditor();
void HookRenderTerrainTile(float xf, float yf, int xi, int yi, float lodf, int lodi, bool Flag);
void HookFixBmdName(char* DirName, char* ModelFileName, char bReAlloc);
void HookOpenObj();
void HookOpenWorld();
bool HookTerrainHeightExt(int world);

void WorldEditor::SetHooks()
{
	//JMP 0xE9
	SetCompleteHook(0xE9, OFFSET_HOOK_RENDER_MU_EDITOR, &HookRenderMuEditor);
	SetCompleteHook(0xE9, OFFSET_HOOK_RENDER_TERRAINTILE, &HookRenderTerrainTile);
	SetCompleteHook(0xE9, OFFSET_HOOK_FIX_BMD_NAME, &HookFixBmdName);
	//SetCompleteHook(0xE9, OFFSET_HOOK_OPEN_OBJ, &HookOpenObj);

	//CALL 0xE8
	SetCompleteHook(0xE8, OFFSET_HOOK_OPEN_WORLD, &HookOpenWorld);
	SetCompleteHook(0xE8, OFFSET_HOOK_TERRAIN_HEIGHT_EXT, &HookTerrainHeightExt);
}

WorldEditor* WE = WorldEditor::Instance();

bool HookTerrainHeightExt(int world)
{
	if (world == WE->LoadWorldConfig.nWorld)
		return WE->LoadWorldConfig.LoadTerrainHeightType == OZB_192K ? true : false;
	else
		return __IsTerrainHeightExtMap(world);
}

void HookOpenWorld()
{
	WE->Refresh();

	//orginal call before hooking
	__OpenWorldModels();

	return;
}

void __declspec(naked) HookRenderMuEditor()
{
	static bool InitWorldEditor = false;
	static const DWORD JMP_BACK_RENDER_MU_EDITOR = OFFSET_JMP_BACK_RENDER_MU_EDITOR;

	if (!InitWorldEditor)
	{
		InitWorldEditor = true;

		WE->LoadExtData();
		WE->InitUI();
		WE->InitEdit();
	}
	else
	{
		WE->UI();
		WE->Edit();
	}

	//orginal call before hooking
	__EndOpengl();

	_asm
	{
		jmp dword ptr[JMP_BACK_RENDER_MU_EDITOR];
	}
}

void __declspec(naked) HookRenderTerrainTile(float xf, float yf, int xi, int yi, float lodf, int lodi, bool Flag)
{
	static const DWORD JMP_BACK_RENDER_TERRAINTILE = OFFSET_JMP_BACK_RENDER_TERRAINTILE;
	static bool result;
	result = WorldEditor::Instance()->RenderTerrainTile(xf, yf, xi, yi, lodf, lodi, Flag);
	_asm
	{
		mov al, result;
		jmp dword ptr[JMP_BACK_RENDER_TERRAINTILE];
	}
}

void FixBmdName(const char* buf, size_t pos, BMD* bmd, char* DirName, char* ModelFileName)
{
	//NoFixNames can be found in MuunRenderInfo.bmd/txt
	static const std::unordered_set<std::string> NoFixNames = {
		"WolfA.SMD",
		"WolfB.SMD",
		"rippen_ride.SMD" ,
		"rippenUp_ride.SMD" ,
		"pon_ride.SMD" ,
		"ponUp_ride.SMD" ,
		"griffs_ride.SMD"  ,
		"griffsUp_ride.SMD"
	};

	char temp[33];
	memcpy(temp, &buf[pos], 32); temp[32] = 0;

	std::string Name = std::string(temp);

	if (NoFixNames.find(Name) == NoFixNames.end())
	{
		bmd->OldName[0] = 'O'; bmd->OldName[1] = 'l'; bmd->OldName[2] = 'd'; bmd->OldName[3] = 0;
		bmd->NewName = std::string(DirName) + std::string(ModelFileName);
		bmd->FlagName = 1;
	}
	else
	{
		memcpy(bmd->OldName, Name.c_str(), Name.length());
		bmd->FlagName = 0;
	}
}

void __declspec(naked) HookFixBmdName(char* DirName, char* ModelFileName, char bReAlloc)
{
	static const DWORD JMP_BACK_FIX_BMD_NAME = OFFSET_JMP_BACK_FIX_BMD_NAME;

	static BMD* bmd;
	static const char* buf;
	static size_t pos;
	_asm {
		mov ecx, [ebp - 0x68];
		mov buf, ecx;
		mov ecx, [ebp - 0x60];
		mov pos, ecx;
		mov ecx, [ebp - 0x11C];
		mov bmd, ecx;
	}

	FixBmdName(buf, pos, bmd, DirName, ModelFileName);

	_asm
	{
		jmp dword ptr[JMP_BACK_FIX_BMD_NAME];
	}
}

void __declspec(naked) HookOpenObj()
{
	static const DWORD JMP_BACK_OPEN_OBJ = OFFSET_JMP_BACK_OPEN_OBJ;
	static BYTE version;
	_asm
	{
		mov[ebp - 0x11], al;
		mov version, al;
	}

	WE->SaveWorldConfig.SaveObjectsType = (OBJ_TYPE)version;

	_asm
	{
		mov eax, [ebp - 0x18];
		jmp dword ptr[JMP_BACK_OPEN_OBJ];
	}
}
