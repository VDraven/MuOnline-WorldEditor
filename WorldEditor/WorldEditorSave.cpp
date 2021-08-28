#include "WorldEditor.h"
#include "Core/Core.h"
#include "Core/OZJ.h"
#include "Core/OZB.h"
#include "Core/MAP.h"
#include "Core/ATT.h"
#include "Core/OBJ.h"
#include "Core/OZP.h"
#include "ZzzTexture.h"

void WorldEditor::SaveWorld()
{
	BaseIO::SetOverwrite(1);

	int nWorldID = SaveWorldConfig.nWorldID;
	char szWorldName[32];
	char szFileName[256];
	char szEncFileName[256];
	const char* szDirSave = Plugin.DirSave.c_str();
	sprintf(szWorldName, "World%d", nWorldID);

	//Save Terrain Light (both types bmp + jpg)
	//TerrainLight.ozb : new version
	sprintf(szFileName, "%s\\Decrypted\\%s\\TerrainLight.bmp", szDirSave, szWorldName);
	sprintf(szEncFileName, "%s\\%s\\TerrainLight.ozb", szDirSave, szWorldName);
	SaveTerrainLightBMP(szFileName, szEncFileName);
	//TerrainLight.ozj : lower version like S6
	sprintf(szFileName, "%s\\Decrypted\\%s\\TerrainLight.jpg", szDirSave, szWorldName);
	sprintf(szEncFileName, "%s\\%s\\TerrainLight.ozj", szDirSave, szWorldName);
	SaveTerrainLightJPG(szFileName, szEncFileName);


	sprintf(szFileName, "%s\\Decrypted\\%s\\TerrainHeight.bmp", szDirSave, szWorldName);
	sprintf(szEncFileName, "%s\\%s\\TerrainHeight.ozb", szDirSave, szWorldName);
	SaveTerrainHeight(szFileName, szEncFileName);

	sprintf(szFileName, "%s\\Decrypted\\%s\\Terrain%d.pam", szDirSave, szWorldName, nWorldID);
	sprintf(szEncFileName, "%s\\%s\\EncTerrain%d.map", szDirSave, szWorldName, nWorldID);
	SaveTerrainMapping(szFileName, szEncFileName, nWorldID);

	sprintf(szFileName, "%s\\Decrypted\\%s\\Terrain%d.tta", szDirSave, szWorldName, nWorldID);
	sprintf(szEncFileName, "%s\\%s\\EncTerrain%d.att", szDirSave, szWorldName, nWorldID);
	SaveTerrainAttribute(szFileName, szEncFileName, nWorldID);

	sprintf(szFileName, "%s\\Decrypted\\%s\\Terrain%d.jbo", szDirSave, szWorldName, nWorldID);
	sprintf(szEncFileName, "%s\\%s\\EncTerrain%d.obj", szDirSave, szWorldName, nWorldID);
	SaveObjects(szFileName, szEncFileName, nWorldID);

	if (nWorldID < 10)
	{
		sprintf(szFileName, "%s\\Decrypted\\%s\\Navimap0%d.png", szDirSave, szWorldName, nWorldID);
		sprintf(szEncFileName, "%s\\%s\\Navimap0%d.ozp", szDirSave, szWorldName, nWorldID);
	}
	else
	{
		sprintf(szFileName, "%s\\Decrypted\\%s\\Navimap%d.png", szDirSave, szWorldName, nWorldID);
		sprintf(szEncFileName, "%s\\%s\\Navimap%d.ozp", szDirSave, szWorldName, nWorldID);
	}
	SaveNaviMap(szFileName, szEncFileName);
}

void WorldEditor::SaveTerrainLightBMP(const char* szFileName, const char* szEncFileName)
{
	if (!szFileName) return;
	CreateParentDir(szFileName);

	constexpr size_t SZ = TERRAIN_SIZE * TERRAIN_SIZE;
	constexpr size_t BITS_SIZE = SZ * 3;
	std::vector<BYTE> buffer(BITS_SIZE);
	for (int i = 0; i < SZ; i++)
	{
		float* pLight = &TerrainLight[0][0];
		//Flip
		buffer[3 * i + 0] = (BYTE)Clamp((int)(pLight[3 * i + 2] * 255.f), 0, 255);
		buffer[3 * i + 1] = (BYTE)Clamp((int)(pLight[3 * i + 1] * 255.f), 0, 255);
		buffer[3 * i + 2] = (BYTE)Clamp((int)(pLight[3 * i + 0] * 255.f), 0, 255);
	}
	
	WriteMapBitmap3(szFileName, buffer);

	if (fs::exists(szFileName) && szEncFileName)
	{
		CreateParentDir(szEncFileName);
		sInstance(OZB)->Pack(szFileName, szEncFileName);
	}
}

void WorldEditor::SaveTerrainLightJPG(const char* szFileName, const char* szEncFileName)
{
	if (!szFileName) return;
	CreateParentDir(szFileName);

	constexpr size_t SZ = TERRAIN_SIZE * TERRAIN_SIZE;
	constexpr size_t BITS_SIZE = SZ * 3;
	std::vector<BYTE> buffer(BITS_SIZE);
	for (int i = 0; i < SZ; i++)
	{
		float* pLight = &TerrainLight[0][0];
		//No Flip
		buffer[3 * i + 0] = (BYTE)Clamp((int)(pLight[3 * i + 0] * 255.f), 0, 255);
		buffer[3 * i + 1] = (BYTE)Clamp((int)(pLight[3 * i + 1] * 255.f), 0, 255);
		buffer[3 * i + 2] = (BYTE)Clamp((int)(pLight[3 * i + 2] * 255.f), 0, 255);
	}
	WriteJpeg(szFileName, TERRAIN_SIZE, TERRAIN_SIZE, buffer.data(), 100);

	if (fs::exists(szFileName) && szEncFileName)
	{
		CreateParentDir(szEncFileName);
		sInstance(OZJ)->Pack(szFileName, szEncFileName);
	}
}

void WorldEditor::SaveTerrainHeight(const char* szFileName, const char* szEncFileName)
{
	if(SaveWorldConfig.SaveTerrainHeightType == OZB_TYPE::OZB_192K)
		return SaveTerrainHeightNew(szFileName, szEncFileName);

	if (!szFileName) return;
	CreateParentDir(szFileName);

	constexpr size_t BITS_SIZE = TERRAIN_SIZE * TERRAIN_SIZE;
	std::vector<BYTE> buffer(BITS_SIZE);
	for (int i = 0; i < BITS_SIZE; i++)
	{
		int height = (int)(BackTerrainHeight[i] / 1.5f);
		buffer[i] = (BYTE)Clamp(height, 0, 255);
	}

	WriteMapBitmap1(szFileName, buffer);

	if (fs::exists(szFileName) && szEncFileName)
	{
		CreateParentDir(szEncFileName);
		sInstance(OZB)->Pack(szFileName, szEncFileName);
	}
}

void WorldEditor::SaveTerrainHeightNew(const char* szFileName, const char* szEncFileName)
{
	if (!szFileName) return;
	CreateParentDir(szFileName);

	constexpr size_t SZ = TERRAIN_SIZE * TERRAIN_SIZE;
	constexpr size_t BITS_SIZE = SZ * 3;
	std::vector<BYTE> buffer(BITS_SIZE);
	for (int i = 0; i < SZ; i++)
	{
		int height = (int)BackTerrainHeight[i] + 500;
		//flip
		buffer[3 * i + 0] = (BYTE) ((height & 0x00FF0000) >> 16);
		buffer[3 * i + 1] = (BYTE) ((height & 0x0000FF00) >> 8);
		buffer[3 * i + 2] = (BYTE) (height & 0x000000FF);
	}

	WriteMapBitmap3(szFileName, buffer);

	if (fs::exists(szFileName) && szEncFileName)
	{
		CreateParentDir(szEncFileName);
		sInstance(OZB)->Pack(szFileName, szEncFileName);
	}
}

void WorldEditor::SaveTerrainMapping(const char* szFileName, const char* szEncFileName, int iMapNumber)
{
	if (!szFileName) return;
	FILE* fp = fopen(szFileName, "wb");
	if (!fp) return;

	BYTE Version = 0;
	fwrite(&Version, 1, 1, fp);
	fwrite(&iMapNumber, 1, 1, fp);	//
	fwrite(TerrainMappingLayer1, TERRAIN_SIZE * TERRAIN_SIZE, 1, fp);
	fwrite(TerrainMappingLayer2, TERRAIN_SIZE * TERRAIN_SIZE, 1, fp);
	for (int i = 0; i < TERRAIN_SIZE * TERRAIN_SIZE; i++)
	{
		BYTE Alpha = (BYTE) Clamp((int)(TerrainMappingAlpha[i] * 255.f), 0, 255);
		fwrite(&Alpha, 1, 1, fp);
	}
	fclose(fp);

	if (fs::exists(szFileName) && szEncFileName)
	{
		CreateParentDir(szEncFileName);
		sInstance(MAP)->Pack(szFileName, szEncFileName);
	}
}

void WorldEditor::SaveTerrainAttribute(const char* szFileName, const char* szEncFileName, int iMapNumber)
{
	if (!szFileName) return;
	FILE* fp = fopen(szFileName, "wb");
	if (!fp) return;

	BYTE Version = 0;
	BYTE Width = 255;		//Webzen's troll ??? 256 instead (width/height is not index)
	BYTE Height = 255;		//Don't change to 256, client will check these values
	fwrite(&Version, 1, 1, fp);
	fwrite(&iMapNumber, 1, 1, fp);
	fwrite(&Width, 1, 1, fp);
	fwrite(&Height, 1, 1, fp);

	constexpr size_t SZ = TERRAIN_SIZE * TERRAIN_SIZE;
	if (SaveWorldConfig.SaveTerrainAttributeType == ATT_TYPE::ATT_64K)
	{
		std::vector<BYTE> buffer(SZ);
		for (size_t i = 0; i < SZ; i++)
		{
			buffer[i] = (BYTE)(TerrainWall[i] & 0xFF);
		}
		fwrite(buffer.data(), SZ, 1, fp);
	}
	else
	{
		fwrite(TerrainWall, 2 * SZ, 1, fp);
	}

	fclose(fp);

	if (fs::exists(szFileName) && szEncFileName)
	{
		CreateParentDir(szEncFileName);
		sInstance(ATT)->Pack(szFileName, szEncFileName);
	}
}

void WorldEditor::SaveObjects(const char* szFileName, const char* szEncFileName, int iMapNumber)
{
	if (!szFileName) return;
	FILE* fp = fopen(szFileName, "wb");
	if (!fp) return;

	short ObjectCount = 0;
	BYTE Version = SaveWorldConfig.SaveObjectsType;
	fwrite(&Version, sizeof(BYTE), 1, fp);
	fwrite(&iMapNumber, 1, 1, fp);
	fseek(fp, 4, SEEK_SET);
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			OBJECT_BLOCK* ob = &ObjectBlock[i * 16 + j];
			OBJECT* o = ob->Head;
			while (1)
			{
				if (o != NULL)
				{
					if (o->Live)
					{
						fwrite(&o->Type, 2, 1, fp);
						fwrite(o->Position, sizeof(vec3_t), 1, fp);
						fwrite(o->Angle, sizeof(vec3_t), 1, fp);
						fwrite(&o->Scale, sizeof(float), 1, fp);

						if (Version >= 1)
						{
							fwrite(&o->LightEnable1, 1, 1, fp);
							fwrite(&o->LightEnable2, 1, 1, fp);
						}

						if (Version >= 2)
						{
							fwrite(&o->LightEnable3, 1, 1, fp);
						}

						if (Version >= 3)
						{
							fwrite(o->Light, sizeof(vec3_t), 1, fp);
						}
					}
					ObjectCount++;
					if (o->Next == NULL) break;
					o = o->Next;
				}
				else break;
			}
		}
	}
	int EndPoint = ftell(fp);
	fseek(fp, 2, SEEK_SET);
	fwrite(&ObjectCount, 2, 1, fp);
	fseek(fp, EndPoint, SEEK_SET);

	fclose(fp);

	if (fs::exists(szFileName) && szEncFileName)
	{
		CreateParentDir(szEncFileName);
		sInstance(OBJ)->Pack(szFileName, szEncFileName);
	}
}

#define SET_COLOR_RGBA(a, b) a[0] = b[0];  a[1] = b[1]; a[2] = b[2]; a[3] = b[3];
void DrawBorderTW(WORD mask, WORD w, const BYTE* color, BYTE* pixel_a, BYTE* pixel_b)
{
	bool is_border = false;
	if(w == 0xFFFF)
		is_border = true;
	else if (mask == TW_NONE && w != TW_NONE)
		is_border = true;
	else if (mask == TW_SAFEZONE && (w & mask) != TW_SAFEZONE)
		is_border = true;

	if (is_border)
	{
		SET_COLOR_RGBA(pixel_a, color);
		SET_COLOR_RGBA(pixel_b, color);
		pixel_a[3] = pixel_b[3] = 255;
	}
}

void WorldEditor::SaveNaviMap(const char* szFileName, const char* szEncFileName)
{
	if (!szFileName) return;

	constexpr BYTE COLOR_NONE[4] { 222 , 252 , 254 , 117 };
	constexpr BYTE COLOR_SAFE[4] { 192 , 254 , 243 , 118 };

	constexpr size_t SZ = 512 * 512;

	std::vector<BYTE> buffer;
	buffer.resize(4 * SZ, 0);

	typedef BYTE QuadRow[512][4];
	QuadRow* data = (QuadRow*) buffer.data();

	for (size_t i = 0; i < 256; i++)
	{
		for (size_t j = 0; j < 256; j++) 
		{
			BYTE* pixel_1 = (BYTE*)&data[2 * i][2 * j];
			BYTE* pixel_2 = (BYTE*)&data[2 * i][2 * j + 1];
			BYTE* pixel_3 = (BYTE*)&data[2 * i + 1][2 * j];
			BYTE* pixel_4 = (BYTE*)&data[2 * i + 1][2 * j + 1];

			WORD w = TerrainWall[i * TERRAIN_SIZE + j];
			WORD mask = 0;
			const BYTE* color = NULL;

			if (w == TW_NONE) 
			{
				color = COLOR_NONE;
				mask = TW_NONE;
			}
			else if (w & TW_SAFEZONE)
			{
				color = COLOR_SAFE;
				mask = TW_SAFEZONE;
			}
			if (color == NULL) continue;

			pixel_1[3] = pixel_2[3] = 59;
			SET_COLOR_RGBA(pixel_3, color);
			SET_COLOR_RGBA(pixel_4, color);

			w = (i + 1 <= 255) ? TerrainWall[(i + 1) * TERRAIN_SIZE + j] : 0xFFFF;
			DrawBorderTW(mask, w, color, pixel_3, pixel_4);

			w = (i - 1 >= 0) ? TerrainWall[(i - 1) * TERRAIN_SIZE + j] : 0xFFFF;
			DrawBorderTW(mask, w, color, pixel_1, pixel_2);

			w = (j + 1 <= 255) ? TerrainWall[i * TERRAIN_SIZE + (j + 1)] : 0xFFFF;
			DrawBorderTW(mask, w, color, pixel_2, pixel_4);

			w = (j - 1 >= 0) ? TerrainWall[i * TERRAIN_SIZE + (j - 1)] : 0xFFFF;
			DrawBorderTW(mask, w, color, pixel_1, pixel_3);
		}
	}

	//Shadow
	for (size_t i = 1; i < 512; i++)
	{
		for (size_t j = 0; j < 511; j++)
		{
			BYTE* pixel = (BYTE*)&data[i][j];
			BYTE* sd_pixel = (BYTE*)&data[i - 1][j + 1];
			if (pixel[3] == 255 && sd_pixel[3] != 255)
			{
				float alpha_sd = 0.8f;
				float alpha_bg = sd_pixel[3] / 255.0f;
				float alpha = alpha_bg + (1.0f - alpha_bg) * alpha_sd;

				sd_pixel[0] = (1 - alpha) * sd_pixel[0];
				sd_pixel[1] = (1 - alpha) * sd_pixel[1];
				sd_pixel[2] = (1 - alpha) * sd_pixel[2];
				sd_pixel[3] = alpha * 255;
			}
		}
	}

	WriteMapPng(szFileName, buffer);

	if (fs::exists(szFileName) && szEncFileName)
	{
		CreateParentDir(szEncFileName);
		sInstance(OZP)->Pack(szFileName, szEncFileName);
	}
}

