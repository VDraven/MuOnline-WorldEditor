#pragma once

#include "WorldEditorStruct.h"

#define OFFSET_HOOK_RENDER_MU_EDITOR		0x0050B4DE
#define OFFSET_HOOK_RENDER_TERRAINTILE		0x005FEFE5
#define OFFSET_HOOK_FIX_BMD_NAME			0x005C2F6D
#define OFFSET_HOOK_OPEN_OBJ				0x0060FE24

#define OFFSET_JMP_BACK_RENDER_MU_EDITOR	0x0050B4E3
#define OFFSET_JMP_BACK_RENDER_TERRAINTILE	0x005FF502
#define OFFSET_JMP_BACK_FIX_BMD_NAME		0x005C2F84
#define OFFSET_JMP_BACK_OPEN_OBJ			0x0060FE2A

#define MODEL_PLAYER	1512

#define World							(*(int*)				0x0181DF04)
#define __GetWorldID					((int(_cdecl*)(int))	0x00E0B64A)
#define __IsNewTerrainHeightWorld		((bool(_cdecl*)(int))	0x005FBD5C)

#define CameraDistance					(*(float *)				0x015F1858)
#define CameraAngle						(*(vec3_t *)			0x08CDCAB8)


#define g_hWnd							(*(HWND*)				0x01855CBC)
#define g_hDC							(*(HDC*)				0x01855CC4)
#define g_hRC							(*(HGLRC*)				0x01855CC8)
#define CameraViewNear					(*(float *)				0x0181DF68)
#define CameraViewFar					(*(float *)				0x0181DF6C)
#define CameraFOV						(*(float *)				0x0181DF70)
#define CameraMatrix					(*(vec34_t *)			0x08CDCA70)
#define g_fScreenRate_x					(*(float *)				0x01848D5C)
#define g_fScreenRate_y					(*(float *)				0x01848D60)
#define g_fSpecialHeight				(*(float *)				0x0181DEEC)
#define WindowSizeW						(*(DWORD *)				0x0181DF90)
#define WindowSizeH						(*(DWORD *)				0x0181DF94)
#define World							(*(int*)				0x0181DF04)
#define EditFlag						(*(int*)				0x08574D70)
#define SelectFlag						(*(int*)				0x086CDFA0)
#define SelectXF						(*(float*)				0x086CE3EC)
#define SelectYF						(*(float*)				0x086CE3E8)
#define MouseLButtonPush				(*(bool*)				0x08CDCA24)
#define MouseRButtonPush				(*(bool*)				0x08CDCA21)
#define MouseLButton					(*(bool*)				0x08CDCA26)
#define MouseRButton					(*(bool*)				0x08CDCA23)
#define MouseX							(*(int*)				0x08CDCB10)
#define MouseY							(*(int*)				0x08CDCB14)
#define MouseTarget						(*(vec3_t*)				0x08CDCA28)
#define MousePosition					(*(vec3_t*)				0x08CDCA34)
#define CollisionPosition				(*(vec3_t*)				0x08CDC8E0)
#define SelectedCharacter				(*(WORD*)				0x0181D950)
typedef vec3_t VertexTransform_t[2][15000];
#define VertexTransform					(*(VertexTransform_t*)	0x06B9F5E0)
#define CurrentLayer					(*(int*)				0x086CE3E0)
#define BackTerrainHeight				((float*)				0x0880E3F0)
#define TerrainLight					((vec3_t*)				0x08C0F440)
#define TerrainMappingAlpha				((float*)				0x087AE3F0)//83EBEE0
					 //struct TERRAIN_MAP_LAYER { BYTE data[256][256]; };
#define TerrainMappingLayer1			((BYTE*)				0x087FE3F0)
#define TerrainMappingLayer2			((BYTE*)				0x087EE3F0)
#define ObjectBlock						((OBJECT_BLOCK*)		0x08CD0110)

#define _Bitmaps						(*(CGlobalBitmap *)		0x09DABA78)
#define CGlobalBitmap__GetTexture		((BITMAP_t*(__thiscall*)(CGlobalBitmap *, GLuint))		0x005796FC)
#define CGlobalBitmap__m_BitmapCache	((void *)				(0x09DABA78 + 0x4C))
#define CBitmapCache__Find				((bool(__thiscall*)(void *, GLuint, BITMAP_t*))	0x00578F1A)
#define CBitmapCache__Add				((void(__thiscall*)(void *, GLuint, BITMAP_t*))	0x005787D7)
#define BMPHeader						((BYTE*)				0x086CDFA8)

//void RenderBitmap(int Texture,float x,float y,float Width,float Height,float u,float v,float uWidth,float vHeight,bool Scale,bool StartScale,float Alpha)
#define __RenderBitmap					((void(_cdecl* )(int, float, float, float, float, float, float, float, float, bool, bool, float))	0x0066FA46)
#define __LoadBitmap					((bool(_cdecl* )(const char*,GLuint , GLuint, GLuint, bool, bool))		0x008366CA)
#define __DeleteBitmap					((void(_cdecl* )(GLuint, bool))		0x00836856)
#define __RenderInfomation				((void(*)())			0x00506F6A)
#define __EnableDepthMask				((void*(*)())			0x0066D2DD)
#define __EnableDepthTest				((void(*)())			0x0066D299)
#define __DisableDepthTest				((void(*)())			0x0066D2BB)
#define __EnableAlphaBlend				((void(*)())			0x0066D4DA)
#define __DisableAlphaBlend				((void(*)())			0x0066D3CB)
#define __EnableAlphaTest				((void(_cdecl*)(bool))	0x0066D445)
#define __DisableTexture				((void(_cdecl*)(bool))	0x0066D35F)
#define __BeginSprite					((void(*)())			0x0066E67A)
#define __BeginBitmap					((void(*)())			0x0066F5C3)
#define __BeginOpengl					((void(*)())			0x0066D8A5)
#define __EndSprite						((void(*)())			0x0066E68B)
#define __EndBitmap						((void(*)())			0x0066F6A0)
#define __EndOpengl						((void(*)())			0x0066DFB1)

#define KeyTable						(*(BYTE* *)				0x09DABBD8)
#define __IsKeyNone						((bool(_cdecl*)(int))	0x0083FE22)
#define __IsKeyRelease					((bool(_cdecl*)(int))	0x0083FE36)
#define __IsKeyPress					((bool(_cdecl*)(int))	0x0083FE4A)
#define __IsKeyRepeat					((bool(_cdecl*)(int))	0x0083FE5E)


#define TERRAIN_SCALE     100.f
#define TERRAIN_SIZE      256
#define TERRAIN_SIZE_MASK 255
//TERRAIN_INDEX(col, row)	//use TERRAIN_INDEX_REPEAT instead to make sure x,y <= 255
#define TERRAIN_INDEX(x, y)			(x + ((y) << 8))
#define TERRAIN_INDEX_REPEAT(x, y)	(y&TERRAIN_SIZE_MASK)*TERRAIN_SIZE + (x&TERRAIN_SIZE_MASK)

#define TerrainIndex1							(*(int*)		0x086CDF9C)
#define TerrainIndex2							(*(int*)		0x086CDF98)
#define TerrainIndex3							(*(int*)		0x086CDF94)
#define TerrainIndex4							(*(int*)		0x086CDF90)
#define TerrainWall								((WORD*)		0x086CE3F0)	//WORD TerrainWall[TERRAIN_SIZE*TERRAIN_SIZE];
#define TerrainVertex							((vec3_t*)		0x086CDF3C) //vec3_t TerrainVertex[4];

enum TW
{								// [Google Translate]
	TW_NONE			= 0x0000,
	TW_SAFEZONE		= 0x0001,	// safe zone
	TW_CHARACTER	= 0x0002,	// Character is occupied
	TW_NOMOVE		= 0x0004,	// general property ( cannot be moved)
	TW_NOGROUND		= 0x0008,	// ground-breaking attribute
	TW_WATER		= 0x0010,	// water properties
	TW_ACTION		= 0x0020,	// Attribute to take a specific action.
	TW_HEIGHT		= 0x0040,	// Climb to a constant height.
	TW_CAMERA_UP	= 0x0080,	// Move the camera up.
	TW_NOATTACKZONE	= 0x0100,	// Non-attackable area (client handling magic).
	TW_UNK_ATT1		= 0x0200,
	TW_UNK_ATT2		= 0x0400,
	TW_UNK_ATT3		= 0x0800,
	TW_UNK_ATT4		= 0x1000,
	TW_UNK_ATT5		= 0x2000,
	TW_UNK_ATT6		= 0x4000,
	TW_UNK_ATT7		= 0x8000,
};
#define __RenderTerrainAlphaBitmap		((void(_cdecl*)(int,float,float,float,float,vec3_t,float,float,float,bool,bool))		0x005FFDC8)
#define __RenderTerrainFace				((void(_cdecl*)(float, float, int, int))												0x005FE12B)
#define __RequestTerrainHeight			((float(_cdecl*)(float, float))															0x005FC4A4)
#define __RequestTerrainLight			((void(_cdecl*)(float, float, vec3_t))													0x005FCCCE)
#define __AddTerrainLightClip			((void(_cdecl*)(float, float, float*, int, vec3_t*))									0x005FCB33)
#define __CreateTerrainNormal			((void(*)())																			0x005FB10C)
#define __CreateTerrainLight			((void(*)())																			0x005FB4DA)
#define __RenderTerrainTile				((bool(_cdecl*)(float, float, int, int, float, int, bool))								0x005FEFC7)
#define __InitCollisionDetectLineToFace	((void(*)())																			0x00670ECA)
#define	Hero							(*(CHARACTER**)		0x09DB106C)
#define __CreateCharacter				((CHARACTER*(_cdecl*)(int, int, int, int, float))										0x00A6CDF2)
#define __CreateMonster					((CHARACTER*(_cdecl*)(int, int, int, int, int))											0x00A6ED4C)
#define ObjCreatePrepare_ctor			((ObjCreatePrepare*(__thiscall*)(ObjCreatePrepare*, int, vec3_t, vec3_t, float, bool, bool, bool))	0x0090C384)
#define __CreateObject					((OBJECT*(_cdecl*)(ObjCreatePrepare* ))													0x0060E651)
#define __DeleteObject					((void(_cdecl*)(OBJECT*, OBJECT_BLOCK*))												0x0060F909)
#define __DeleteCharacter					((void(_cdecl*)(int))																	0x00A6B088)
#define OBJECT__DestroyModel					((void(__thiscall*)(OBJECT*, BYTE))														0x004FAEB3)

#define NpcNameLoader					(0x0A6A6AD8)
#define NpcNameMap						(NpcNameLoader + 0x40)

#define CCharacterManager__GetSingleton ((void*(*)())																			0x00458077)
#define CCharacterManager__GetCharacter	((CHARACTER*(__thiscall*)(void*, WORD))													0x00AB7659)
//	CCharacterManager__GetCharacter(CCharacterManager__GetSingleton(), index);

#define CNewModelManager__GetSingleton	((void*(*)())																			0x004FB610)
#define CNewModelManager__GetModel		((BMD*(__thiscall*)(void*, int))														0x00CADDD8)
//	CNewModelManager__GetModel(CNewModelManager__GetSingleton(), index);
#define ModelMap						((DWORD)CNewModelManager__GetSingleton() + 0x4)


#define BoneTransform					(*(vec34_t*)		0x06B9AF88)
//void BMD::Animation(float(*BoneMatrix)[3][4], float AnimationFrame, float PriorFrame, WORD PriorAction, vec3_t Angle, vec3_t HeadAngle, bool Parent, bool Translate)
#define BMD__Animation					((void(__thiscall*)(BMD*, vec34_t, float, float, WORD, vec3_t ,vec3_t , bool, bool))	0x005BCE65)
//void BMD::Transform(float (*BoneMatrix)[3][4],vec3_t BoundingBoxMin,vec3_t BoundingBoxMax,OBB_t *OBB,bool Translate, float, float)
#define BMD__Transform					((void(__thiscall*)(BMD*, vec34_t, vec3_t, vec3_t, OBB_t *, bool, float, float))		0x005BD527)
//bool CollisionDetectLineToOBB(vec3_t p1, vec3_t p2, OBB_t obb)
#define __CollisionDetectLineToOBB		((bool(_cdecl*)(vec3_t, vec3_t, OBB_t))													0x006717A5)
//void FaceNormalize(vec3_t v1,vec3_t v2,vec3_t v3,vec3_t Normal)
#define __FaceNormalize					((void(_cdecl*)(vec3_t, vec3_t, vec3_t, vec3_t))										0x00A80417)
//bool CollisionDetectLineToFace(vec3_t Position,vec3_t Target,int Polygon,float *v1,float *v2,float *v3,float *v4,vec3_t Normal,bool Collision = true)
#define __CollisionDetectLineToFace		((bool(_cdecl*)(vec3_t, vec3_t, int, float *, float *, float *, float *, vec3_t, bool))	0x00670EDB)

#define __Perspective_Backup			((void*(*)())																			0x0066CD91)
#define __CameraMatrix_Backup			((void*(*)())																			0x0066CCEB)
#define __Perspective_Restore			((void*(*)())																			0x0066CDAE)
#define __CameraMatrix_Restore			((void*(*)())																			0x0066CD3E)
#define __UpdateMousePosition			((void*(*)())																			0x0066DFD8)
#define __glViewport2					((void(_cdecl*)(int, int, int, int))													0x0066D816)
#define __gluPerspective2				((void(_cdecl*)(float, float, float, float))											0x0066CBF0)
#define __GetOpenGLMatrix				((void(_cdecl*)(vec34_t))																0x0066CB93)
#define __CreateScreenVector			((void(_cdecl*)(float, float, vec3_t, bool))											0x0066CDCB)
#define Angle_Info						((float*)																				0x086CDC80)
#define __RenderObjectScreen			((void(_cdecl*)(int, int, int, int, vec3_t, bool, bool, int, int, float))				0x005F1EBA)
#define __UnRegisterBuff				((void(_cdecl*)(int, OBJECT*))															0x00D382F6)
#define __RegisterBuff					((void(_cdecl*)(int, OBJECT*, int, int))												0x00D381BA)
#define BUFF_INVISIBLE			18
#define BUFF_ADMIN_INVISIBLE	27

#define __fopen							((FILE*(_cdecl*)(const char*, const char*))												0x0123A7DD)
#define __fread							((size_t(_cdecl*)(void*, size_t, size_t, FILE*))										0x0123A6FC)