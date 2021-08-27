#include "WorldEditor.h"
#include "Core/ZzzMathLib.h"
#include "Core/MuCrypto.h"

void WorldEditor::Init()
{
	SetHooks();
	LoadPluginConfig();
}

void WorldEditor::Refresh()
{
	FreeMode = false;
	PickObject = NULL;
	PickMonster = NULL;
	BrushSize = 0;
	SelectMonsterType = SelectTileType = SelectWallType = SelectObjectType = -1;
	SelectLock = HeightLock = GridLock = FlatLock = false;
	Frame3D.Zoom = 1.0f;
	Frame3D.Angle[0] = 270.0f;
	Frame3D.Angle[1] = 0.0f;
	Frame3D.Angle[2] = 0.0f;

	//tiles cache clean (including ext tiles)
	for (int i = 0; i < 31; i++)
		CBitmapCache__Remove(CGlobalBitmap__m_BitmapCache, 30256 + i);
	Extension.ImportedTiles.clear();

	SaveWorldConfig.nWorld = World;
	SaveWorldConfig.nWorldID = __GetWorldID(World);

	//Get Save World Types
	if (__IsNewTerrainHeightWorld(World))
		SaveWorldConfig.SaveTerrainHeightType = OZB_TYPE::OZB_192K;
	else
		SaveWorldConfig.SaveTerrainHeightType = OZB_TYPE::OZB_64K;
	SaveWorldConfig.SaveTerrainAttributeType = ATT_TYPE::ATT_128K;

	char szFileName[256];
	sprintf(szFileName, "Data\\World%d\\EncTerrain%d.obj", SaveWorldConfig.nWorldID, SaveWorldConfig.nWorldID);

	FILE* fp = fopen(szFileName, "rb");
	if (fp)
	{
		// We only need to read the first byte (skip size == 0 check)
		BYTE version;
		fread(&version, 1u, 1, fp);
		//MapFileDecrypt (for 1 byte)
		version ^= 0xD1;
		version -= 0x5E;

		SaveWorldConfig.SaveObjectsType = (OBJ_TYPE)version;
		fclose(fp);
	}
}

void WorldEditor::LoadPluginConfig()
{
	char szNull[] = "";
	char szBuffer[1024];

	Plugin.Active = GetPrivateProfileIntA("WorldEditor", "Active", 0, "Plugins\\Config.ini");

	GetPrivateProfileStringA("WorldEditor", "DirSave", szNull, szBuffer, 1024, "Plugins\\Config.ini");
	Plugin.DirSave = std::string(szBuffer);

	//================================================

	Extension.Active = GetPrivateProfileIntA("WorldExtension", "Active", 0, "Plugins\\Config.ini");

	GetPrivateProfileStringA("WorldExtension", "DirTile", szNull, szBuffer, 1024, "Plugins\\Config.ini");
	Extension.DirTile = std::string(szBuffer);

	GetPrivateProfileStringA("WorldExtension", "DirObject", szNull, szBuffer, 1024, "Plugins\\Config.ini");
	Extension.DirObject = std::string(szBuffer);
}

void WorldEditor::LoadExtData()
{
	if (Extension.Active)
	{
		LoadExtTile();
		LoadExtObject();
	}
}

void WorldEditor::LoadExtTile()
{
	if (Extension.DirTile.empty()) return;

	fs::path pData(Extension.DirTile);
	pData.append("Tiles.data");
	if (fs::exists(pData))
	{
		std::ifstream is(pData);
		if (is.is_open())
		{
			std::string line;
			while (std::getline(is, line))
			{
				if (line.length() >= 2 && line[0] == '/' && line[1] == '/')
					continue;

				GLuint index;
				char szBuff[1024];
				sscanf_s(line.c_str(), "%u\t%s", &index, szBuff, 1024);

				if (index < 1) continue;

				std::string filename(szBuff);
				//ToLowerCaseString(filename);

				if (Extension.Tiles.find(filename) != Extension.Tiles.end()) continue;

				Extension.Tiles[filename] = index;
				//LoadBitmap(filename.c_str(), index, GL_NEAREST, GL_REPEAT, false, true);
				Bitmaps.LoadImage(index, filename.c_str(), GL_NEAREST, GL_REPEAT);
			}
			is.close();
		}
	}

	std::ofstream os(pData, std::ofstream::app);
	if (os.is_open())
	{
		fs::path dir(Extension.DirTile);
		for (auto& p : fs::recursive_directory_iterator(dir))
		{
			if (p.is_regular_file())
			{
				std::string ext = p.path().extension().string();
				ToLowerCaseString(ext);
				if (ext == ".ozt" || ext == ".ozj")
				{
					fs::path pFile = p.path();
					if (ext == ".ozt") pFile.replace_extension(".tga");
					if (ext == ".ozj") pFile.replace_extension(".jpg");

					std::string filename = pFile.string();
					//ToLowerCaseString(filename);

					if (Extension.Tiles.find(filename) != Extension.Tiles.end()) continue;

					GLuint index = CGlobalBitmap::NextBitmapIndex;
					Extension.Tiles[filename] = index;
					//LoadBitmap(filename.c_str(), index, GL_NEAREST, GL_REPEAT, false, true);
					Bitmaps.LoadImage(index, filename.c_str(), GL_NEAREST, GL_REPEAT);
					os << index << "\t" << filename << std::endl;
				}
			}
		}
		os.close();
	}
}

void WorldEditor::LoadExtObject()
{

}

OBJECT * WorldEditor::CollisionDetectObjects()
{
	__InitCollisionDetectLineToFace();
	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 16; ++j)
		{
			OBJECT_BLOCK* block = &ObjectBlock[j + 16 * i];
			for (OBJECT* obj = block->Head; obj; obj = obj->Next)
			{
				if (obj->Visible && obj->Alpha >= 0.01f)
				{
					BMD* bmd = CNewModelManager__GetModel(CNewModelManager__GetSingleton(), obj->Type);
					if (bmd)
					{
						bmd->BodyScale = obj->Scale;
						bmd->CurrentAction = obj->CurrentAction;
						bmd->BodyOrigin[0] = obj->Position[0];
						bmd->BodyOrigin[1] = obj->Position[1];
						bmd->BodyOrigin[2] = obj->Position[2];
						BMD__Animation(bmd, BoneTransform, obj->AnimationFrame, obj->PriorAnimationFrame, obj->CurrentAction, obj->Angle, obj->HeadAngle, 0, 0);
						BMD__Transform(bmd, BoneTransform, obj->BoundingBoxMin, obj->BoundingBoxMax, &obj->OBB, 1, 0.0f, 0.0f);

						if (__CollisionDetectLineToOBB(MousePosition, MouseTarget, obj->OBB))
						{
							if(CollisionDetectLineToMesh(bmd, MousePosition, MouseTarget, 1, -1, -1))
								return obj;
						}
					}
				}
			}
		}
	}
	return NULL;
}

bool WorldEditor::CollisionDetectLineToMesh(BMD * bmd, vec3_t position, vec3_t target, bool collision, int mesh, int triangle)
{

	for (int i = 0; i < bmd->NumMeshs; ++i)
	{
		for (int j = 0; j < bmd->Meshs[i].NumTriangles; ++j)
		{
			if (i != mesh || j != triangle)
			{
				short* v = bmd->Meshs[i].Triangles[j].VertexIndex;
				
				float* v1 = VertexTransform[i][v[0]];
				float* v2 = VertexTransform[i][v[1]];
				float* v3 = VertexTransform[i][v[2]];
				float* v4 = VertexTransform[i][v[3]];

				vec3_t normal;
				__FaceNormalize(v1, v2, v3, normal);
				if (__CollisionDetectLineToFace(position, target, bmd->Meshs[i].Triangles[j].Polygon, v1, v2, v3, v4, normal, collision))
					return true;
			}
		}
	}
	return false;
}

void WorldEditor::AddTerrainHeight(float xf, float yf, float height, int range, float* buffer)
{
	float rf = (float)range;

	xf = xf / TERRAIN_SCALE;
	yf = yf / TERRAIN_SCALE;
	int   xi = (int)xf;
	int   yi = (int)yf;
	int   syi = yi - range;
	int   eyi = yi + range;
	float syf = (float)(syi);
	for (; syi <= eyi; syi++, syf += 1.f)
	{
		int   sxi = xi - range;
		int   exi = xi + range;
		float sxf = (float)(sxi);
		for (; sxi <= exi; sxi++, sxf += 1.f)
		{
			float xd = xf - sxf;
			float yd = yf - syf;
			float lf = (rf - sqrtf(xd * xd + yd * yd)) / rf;
			if (lf > 0.f)
			{
				buffer[TERRAIN_INDEX_REPEAT(sxi, syi)] += height * lf;
			}
		}
	}
}

void WorldEditor::SetTerrainHeight(float xf, float yf, float height, int range, float* buffer)
{
	float rf = (float)range;

	xf = xf / TERRAIN_SCALE;
	yf = yf / TERRAIN_SCALE;
	int   xi = (int)xf;
	int   yi = (int)yf;
	int   syi = yi - range;
	int   eyi = yi + range;
	float syf = (float)(syi);
	for (; syi <= eyi; syi++, syf += 1.f)
	{
		int   sxi = xi - range;
		int   exi = xi + range;
		float sxf = (float)(sxi);
		for (; sxi <= exi; sxi++, sxf += 1.f)
		{
			float xd = xf - sxf;
			float yd = yf - syf;
			buffer[TERRAIN_INDEX_REPEAT(sxi, syi)] = height;
		}
	}
}

void WorldEditor::SetTerrainLight(float xf, float yf, vec3_t light, int range, vec3_t* buffer)
{
	float rf = (float)range;

	xf = (xf / TERRAIN_SCALE);
	yf = (yf / TERRAIN_SCALE);
	int   xi = (int)xf;
	int   yi = (int)yf;
	int   syi = yi - range;
	int   eyi = yi + range;
	float syf = (float)(syi);
	for (; syi <= eyi; syi++, syf += 1.f)
	{
		int   sxi = xi - range;
		int   exi = xi + range;
		float sxf = (float)(sxi);
		for (; sxi <= exi; sxi++, sxf += 1.f)
		{
			float xd = xf - sxf;
			float yd = yf - syf;

			float* b = &buffer[TERRAIN_INDEX_REPEAT(sxi, syi)][0];
			for (int i = 0; i < 3; i++)
			{
				b[i] = light[i];
				if (b[i] < 0.f) b[i] = 0.f;
				else if (b[i] > 1.f) b[i] = 1.f;
			}
			
		}
	}
}

void WorldEditor::SetTerrainWall(float xf, float yf, int range, bool set)
{
	float rf = (float)range;

	xf = (xf / TERRAIN_SCALE);
	yf = (yf / TERRAIN_SCALE);
	int   xi = (int)xf;
	int   yi = (int)yf;
	int   syi = yi - range;
	int   eyi = yi + range;
	float syf = (float)(syi);
	for (; syi <= eyi; syi++, syf += 1.f)
	{
		int   sxi = xi - range;
		int   exi = xi + range;
		float sxf = (float)(sxi);
		for (; sxi <= exi; sxi++, sxf += 1.f)
		{
			float xd = xf - sxf;
			float yd = yf - syf;

			int idx = TERRAIN_INDEX_REPEAT(sxi, syi);
			if (SelectWallType == 0)
				TerrainWall[idx] = 0;
			else if (set)
				TerrainWall[idx] |= SelectWallType;
			else
				TerrainWall[idx] &= ~SelectWallType;
		}
	}
}


void WorldEditor::RenderPreview3D(int model_idx, float x, float y, float width, float height, int view)
{
	BMD* bmd = CNewModelManager__GetModel(CNewModelManager__GetSingleton(), model_idx);
	if (!bmd) return;

	__EnableDepthTest();
	__EnableDepthMask();

	__Perspective_Backup();
	__CameraMatrix_Backup();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	__glViewport2((int)x, (int)y, (int)width, (int)height);
	__gluPerspective2(1.0f / Frame3D.Zoom, width / height, CameraViewNear, CameraViewFar);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	__GetOpenGLMatrix(CameraMatrix);

	Angle_Info[0] = Frame3D.Angle[0];
	Angle_Info[1] = Frame3D.Angle[1];
	Angle_Info[2] = Frame3D.Angle[2];

	float mx, my;
	vec3_t v;

	switch (view)
	{
	case 1:
		mx = ((width * 0.5f) + x) / g_fScreenRate_x;
		my = ((height * 0.52f) + y) / g_fScreenRate_y;
		break;
	default:
		mx = ((width * 0.5f) + x) / g_fScreenRate_x;
		my = ((height * 0.6f) + y) / g_fScreenRate_y;
		break;
	}
	__CreateScreenVector(mx, my, v, 0);
	__RenderObjectScreen(model_idx, 0, 0, 0, v, 0, 0, -1, -1, 0.004f);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	__Perspective_Restore();
	__CameraMatrix_Restore(); 
	__UpdateMousePosition();
}


bool WorldEditor::RenderTerrainTile(float xf, float yf, int xi, int yi, float lodf, int lodi, bool flag)
{
	TerrainIndex1 = TERRAIN_INDEX(xi, yi);
	if ((TerrainWall[TerrainIndex1] & TW_NOGROUND) == TW_NOGROUND && !flag) return false;

	TerrainIndex2 = TERRAIN_INDEX(xi + lodi, yi);
	TerrainIndex3 = TERRAIN_INDEX(xi + lodi, yi + lodi);
	TerrainIndex4 = TERRAIN_INDEX(xi, yi + lodi);

	float sx = xf * TERRAIN_SCALE;
	float sy = yf * TERRAIN_SCALE;

	Vector(sx, sy, BackTerrainHeight[TerrainIndex1], TerrainVertex[0]);
	Vector(sx + TERRAIN_SCALE, sy, BackTerrainHeight[TerrainIndex2], TerrainVertex[1]);
	Vector(sx + TERRAIN_SCALE, sy + TERRAIN_SCALE, BackTerrainHeight[TerrainIndex3], TerrainVertex[2]);
	Vector(sx, sy + TERRAIN_SCALE, BackTerrainHeight[TerrainIndex4], TerrainVertex[3]);

	if ((TerrainWall[TerrainIndex1] & TW_HEIGHT) == TW_HEIGHT) TerrainVertex[0][2] = g_fSpecialHeight;
	if ((TerrainWall[TerrainIndex2] & TW_HEIGHT) == TW_HEIGHT) TerrainVertex[1][2] = g_fSpecialHeight;
	if ((TerrainWall[TerrainIndex3] & TW_HEIGHT) == TW_HEIGHT) TerrainVertex[2][2] = g_fSpecialHeight;
	if ((TerrainWall[TerrainIndex4] & TW_HEIGHT) == TW_HEIGHT) TerrainVertex[3][2] = g_fSpecialHeight;

	if (!flag)
	{
		__RenderTerrainFace(xf, yf, xi, yi);
	}
	else
	{
		if (EditFlag != WorldEditor::EDIT_LIGHT)
		{
			__DisableTexture(false);
			glColor3f(0.5f, 0.5f, 0.5f);
			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < 4; i++)
			{
				glVertex3fv(TerrainVertex[i]);
			}
			glEnd();
			__DisableAlphaBlend();
		}
		bool b = (SelectFlag && SelectXF == xf && SelectYF == yf) ? true : false;

		vec3_t Normal;
		//  현재 타일의 노멜벡터값을 구한다.
		__FaceNormalize(TerrainVertex[0], TerrainVertex[1], TerrainVertex[2], Normal);
		bool Success = __CollisionDetectLineToFace(MousePosition, MouseTarget, 3, TerrainVertex[0], TerrainVertex[1], TerrainVertex[2], TerrainVertex[3], Normal, true);
		if (Success == false)
		{
			__FaceNormalize(TerrainVertex[0], TerrainVertex[2], TerrainVertex[3], Normal);
			Success = __CollisionDetectLineToFace(MousePosition, MouseTarget, 3, TerrainVertex[0], TerrainVertex[2], TerrainVertex[3], TerrainVertex[1], Normal, true);
		}
		if (Success == true)
		{
			SelectFlag = true;
			SelectXF = xf;
			SelectYF = yf;
		}

		if (EditFlag == EDIT_WALL && (
			(TerrainWall[TerrainIndex1] == 0 && SelectWallType==0) || (TerrainWall[TerrainIndex1] & SelectWallType)
			))
		{
			__DisableDepthTest();
			__EnableAlphaTest(true);
			__DisableTexture(false);

			glBegin(GL_TRIANGLE_FAN);
			glColor4f(SelectWallColor[0], SelectWallColor[1], SelectWallColor[2], SelectWallColor[3]);
			for (int i = 0; i < 4; i++)
			{
				glVertex3fv(TerrainVertex[i]);
			}
			glEnd();

			__DisableAlphaBlend();
		}
		
		if (b && BrushRing)
		{
			GLuint texture = 32367;		//Effect\\bluering0001_R.jpg
			float x = sx + (TERRAIN_SCALE * 0.5f);
			float y = sy + (TERRAIN_SCALE * 0.5f);
			float sz = (1.0f + (2.0f * BrushSize)) * sqrtf(2.0f);
			vec3_t light = { 1.0f, 1.0f, 1.0f };
			float rotation = 0.0f;
			float alpha = 1.0f;
			float height = 5.0f;

			__EnableAlphaBlend();
			__RenderTerrainAlphaBitmap(texture, x, y, sz, sz, light, rotation, alpha, height, 1, 0);
			__DisableAlphaBlend();
		}
		
		if (b && SelectRing)
		{
			if (PickObject && (EditFlag == EDIT_OBJECT || EditFlag == EDIT_MONSTER))
			{
				OBJECT* o = PickObject;
				GLuint texture = 32367;		//Effect\\bluering0001_R.jpg
				float x = o->Position[0];
				float y = o->Position[1];
				float sz = max(o->OBB.XAxis[0], o->OBB.YAxis[1]) / TERRAIN_SCALE;
				vec3_t light = { 1.0f, 1.0f, 1.0f };
				float rotation = 0.0f;
				float alpha = 1.0f;
				float height = 5.0f;

				__EnableAlphaBlend();
				__RenderTerrainAlphaBitmap(texture, x, y, sz, sz, light, rotation, alpha, height, 1, 0);
				__DisableAlphaBlend();
			}
		}

		return Success;
	}
	return false;
}