#include "WorldEditor.h"
#include "Core/Core.h"
#include "Core/OZB.h"
#include "Core/MAP.h"
#include "Core/ATT.h"
#include "Core/OBJ.h"

void WriteBitmap1(const char* szFileName, std::vector<BYTE>& buffer);
void WriteBitmap3(const char* szFileName, std::vector<BYTE>& buffer);

void WorldEditor::SaveWorld()
{
	int nWorldID = SaveWorldConfig.nWorldID;
	char szWorldName[32];
	char szFileName[256];
	char szEncFileName[256];
	const char* szDirSave = PluginConfig.DirSave.c_str();
	sprintf(szWorldName, "World%d", nWorldID);

	sprintf(szFileName, "%s\\%s\\Decrypted\\TerrainLight.bmp", szDirSave, szWorldName);
	sprintf(szEncFileName, "%s\\%s\\TerrainLight.ozb", szDirSave, szWorldName);
	SaveTerrainLight(szFileName, szEncFileName);

	sprintf(szFileName, "%s\\%s\\Decrypted\\TerrainHeight.bmp", szDirSave, szWorldName);
	sprintf(szEncFileName, "%s\\%s\\TerrainHeight.ozb", szDirSave, szWorldName);
	SaveTerrainHeight(szFileName, szEncFileName);

	sprintf(szFileName, "%s\\%s\\Decrypted\\Terrain%d.pam", szDirSave, szWorldName, nWorldID);
	sprintf(szEncFileName, "%s\\%s\\EncTerrain%d.map", szDirSave, szWorldName, nWorldID);
	SaveTerrainMapping(szFileName, szEncFileName, nWorldID);

	sprintf(szFileName, "%s\\%s\\Decrypted\\Terrain%d.tta", szDirSave, szWorldName, nWorldID);
	sprintf(szEncFileName, "%s\\%s\\EncTerrain%d.att", szDirSave, szWorldName, nWorldID);
	SaveTerrainAttribute(szFileName, szEncFileName, nWorldID);

	sprintf(szFileName, "%s\\%s\\Decrypted\\Terrain%d.jbo", szDirSave, szWorldName, nWorldID);
	sprintf(szEncFileName, "%s\\%s\\EncTerrain%d.obj", szDirSave, szWorldName, nWorldID);
	SaveObjects(szFileName, szEncFileName, nWorldID);
}

void WorldEditor::SaveTerrainLight(const char* szFileName, const char* szEncFileName)
{
	CreateParentDir(szFileName);

	constexpr size_t SZ = TERRAIN_SIZE * TERRAIN_SIZE;
	constexpr size_t BITS_SIZE = SZ * 3;
	std::vector<BYTE> buffer(BITS_SIZE);
	for (int i = 0; i < SZ; i++)
	{
		float* pLight = &TerrainLight[0][0];
		buffer[3 * i + 0] = (BYTE)Clamp((int)(pLight[3 * i + 2] * 255.f), 0, 255);
		buffer[3 * i + 1] = (BYTE)Clamp((int)(pLight[3 * i + 1] * 255.f), 0, 255);
		buffer[3 * i + 2] = (BYTE)Clamp((int)(pLight[3 * i + 0] * 255.f), 0, 255);
	}
	
	WriteBitmap3(szFileName, buffer);

	if (fs::exists(szFileName))
	{
		CreateParentDir(szEncFileName);
		sInstance(OZB)->Pack(szFileName, szEncFileName);
	}
}

void WorldEditor::SaveTerrainHeight(const char* szFileName, const char* szEncFileName)
{
	if(SaveWorldConfig.SaveTerrainHeightType == OZB_TYPE::OZB_192K)
		return SaveTerrainHeightNew(szFileName, szEncFileName);

	CreateParentDir(szFileName);

	constexpr size_t BITS_SIZE = TERRAIN_SIZE * TERRAIN_SIZE;
	std::vector<BYTE> buffer(BITS_SIZE);
	for (int i = 0; i < BITS_SIZE; i++)
	{
		int height = (int)(BackTerrainHeight[i] / 1.5f);
		buffer[i] = (BYTE)Clamp(height, 0, 255);
	}

	WriteBitmap1(szFileName, buffer);

	if (fs::exists(szFileName)) 
	{
		CreateParentDir(szEncFileName);
		sInstance(OZB)->Pack(szFileName, szEncFileName);
	}
}

void WorldEditor::SaveTerrainHeightNew(const char* szFileName, const char* szEncFileName)
{
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

	WriteBitmap3(szFileName, buffer);

	if (fs::exists(szFileName))
	{
		CreateParentDir(szEncFileName);
		sInstance(OZB)->Pack(szFileName, szEncFileName);
	}
}

void WorldEditor::SaveTerrainMapping(const char* szFileName, const char* szEncFileName, int iMapNumber)
{
	FILE* fp = fopen(szFileName, "wb");

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

	if (fs::exists(szFileName))
	{
		CreateParentDir(szEncFileName);
		sInstance(MAP)->Pack(szFileName, szEncFileName);
	}
}

void WorldEditor::SaveTerrainAttribute(const char* szFileName, const char* szEncFileName, int iMapNumber)
{
	FILE* fp = fopen(szFileName, "wb");

	BYTE Version = 0;
	BYTE Width = 255;
	BYTE Height = 255;
	fwrite(&Version, 1, 1, fp);
	fwrite(&iMapNumber, 1, 1, fp);
	fwrite(&Width, 1, 1, fp);
	fwrite(&Height, 1, 1, fp);

	constexpr size_t SZ = TERRAIN_SIZE * TERRAIN_SIZE;
	if (SaveWorldConfig.SaveTerrainAttributeType == ATT_TYPE::ATT_64K)
	{
		std::vector<BYTE> buf(SZ);
		for (size_t i = 0; i < SZ; i++)
		{
			buf[i] = (BYTE)(TerrainWall[i] & 0xFF);
		}
		fwrite(buf.data(), SZ, 1, fp);
	}
	else
	{
		fwrite(TerrainWall, 2 * SZ, 1, fp);
	}

	fclose(fp);

	if (fs::exists(szFileName))
	{
		CreateParentDir(szEncFileName);
		sInstance(ATT)->Pack(szFileName, szEncFileName);
	}
}

void WorldEditor::SaveObjects(const char* szFileName, const char* szEncFileName, int iMapNumber)
{
	FILE* fp = fopen(szFileName, "wb");

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

	if (fs::exists(szFileName))
	{
		CreateParentDir(szEncFileName);
		sInstance(OBJ)->Pack(szFileName, szEncFileName);
	}
}

//FIXME: I feel wrong with the +2 (TerrainHeight.bmp 64K)
void WriteBitmap1(const char* szFileName, std::vector<BYTE>& buffer)
{
	constexpr size_t BITS_SIZE = TERRAIN_SIZE * TERRAIN_SIZE;
	if (buffer.size() != BITS_SIZE) return;

	constexpr size_t HEADER_SIZE =	// 1078
		sizeof(BITMAPFILEHEADER)
		+ sizeof(BITMAPINFOHEADER)
		+ 0x400;	//bmiColors

	constexpr size_t FILE_SIZE =	// 66616
		HEADER_SIZE
		+ 2							// +2 ??? Webzen's mess
		+ BITS_SIZE;

	constexpr BITMAPFILEHEADER header
	{
		0x4D42,			//bfType		//"BM"
		FILE_SIZE,		//bfSize
		0,				//bfReserved1
		0,				//bfReserved2
		HEADER_SIZE		//bfOffBits
	};

	constexpr BITMAPINFOHEADER bmiHeader
	{
		sizeof(BITMAPINFOHEADER),	//biSize
		256,						//biWidth
		256,						//biHeight
		1,							//biPlanes
		8,							//biBitCount		// 1 BYTE
		BI_RGB,						//biCompression		// 0 (no compression)
		0,							//biSizeImage		// 0 (for BI_RGB)
		2834,						//biXPelsPerMeter	// not important 
		2834,						//biYPelsPerMeter	// (72 DPI × 39.3701 inches per meter yields 2834.6472)
		0,							//biClrUsed
		0							//biClrImportant
	};

	//create colors data only once with static magic
	static std::vector<RGBQUAD> bmiColors;
	if (bmiColors.empty())
	{	//Create ref colors table (biBitCount = 8). 
		bmiColors.resize(256);
		for (int i = 0; i < 256; i++)
		{
			bmiColors[i].rgbRed = bmiColors[i].rgbGreen = bmiColors[i].rgbBlue = (BYTE)i;
			bmiColors[i].rgbReserved = 0;
		}
	}

	FILE* fp = fopen(szFileName, "wb");
	fwrite(&header, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(bmiColors.data(), bmiColors.size() * sizeof(RGBQUAD), 1, fp);
	fwrite(buffer.data(), 2, 1, fp);
	fwrite(buffer.data(), buffer.size(), 1, fp);
	fclose(fp);
}


void WriteBitmap3(const char* szFileName, std::vector<BYTE>& buffer)
{
	constexpr size_t SZ = TERRAIN_SIZE * TERRAIN_SIZE;
	constexpr size_t BITS_SIZE = SZ * 3;
	if (buffer.size() != BITS_SIZE) return;

	constexpr size_t HEADER_SIZE =	// 54
		sizeof(BITMAPFILEHEADER)
		+ sizeof(BITMAPINFOHEADER);
	constexpr size_t FILE_SIZE =	// 196662
		HEADER_SIZE
		+ BITS_SIZE;

	constexpr BITMAPFILEHEADER header
	{
		0x4D42,			//bfType		//"BM"
		FILE_SIZE,		//bfSize
		0,				//bfReserved1
		0,				//bfReserved2
		HEADER_SIZE		//bfOffBits
	};

	constexpr BITMAPINFOHEADER bmiHeader
	{
		sizeof(BITMAPINFOHEADER),	//biSize
		256,						//biWidth
		256,						//biHeight
		1,							//biPlanes
		24,							//biBitCount		// 3 BYTES
		BI_RGB,						//biCompression		// 0 (no compression)
		0,							//biSizeImage		// 0 (for BI_RGB)
		0,							//biXPelsPerMeter	// not important 
		0,							//biYPelsPerMeter
		0,							//biClrUsed
		0							//biClrImportant
	};

	FILE* fp = fopen(szFileName, "wb");
	fwrite(&header, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(buffer.data(), buffer.size(), 1, fp);
	fclose(fp);
}