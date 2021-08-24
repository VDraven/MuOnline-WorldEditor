#include "WorldEditor.h"


void WorldEditor::InitEdit()
{
	PickObjectScale = 1.0f;
	PickObjectHeight = 0.0f;

	MonsterKey = 30000;
}

void WorldEditor::Edit()
{
	if (EditFlag)
	{
		//EnableAlphaBlend();

		EditObjects();

		//DisableAlphaBlend();
	}
}

//FIXME : Need a better way to rewrite this part !!!
void WorldEditor::EditCamera()
{
	
	if (!FreeMode) return;

	if (__IsKeyPress(VK_PRIOR))	//PAGE UP
		CameraAngle[2] = 45.0f * ((int)CameraAngle[2] / 45) + 45.0f;
	if (__IsKeyPress(VK_NEXT))	//PAGE DOWN
		CameraAngle[2] = 45.0f * ((int)CameraAngle[2] / 45) - 45.0f;
	if (__IsKeyPress(VK_ADD))
		CameraDistance = 100.0f * ((int)CameraDistance / 100) + 100.0f;
	if (__IsKeyPress(VK_SUBTRACT))
		CameraDistance = 100.0f * ((int)CameraDistance / 100) - 100.0f;

	int angle = (int)CameraAngle[2] % 360;
	if (angle < 0) angle += 360;
	angle = 45 * ((angle + 22) / 45);

	if (__IsKeyRepeat(VK_UP)) 
	{ 
		switch (angle)
		{
		case 315:
			HeroPos[0] -= TERRAIN_SCALE; HeroPos[1] += TERRAIN_SCALE;
			break;
		case 270:
			HeroPos[0] -= TERRAIN_SCALE;
			break;
		case 225:
			HeroPos[0] -= TERRAIN_SCALE; HeroPos[1] -= TERRAIN_SCALE;
			break;
		case 180:
			HeroPos[1] -= TERRAIN_SCALE;
			break;
		case 135:
			HeroPos[0] += TERRAIN_SCALE; HeroPos[1] -= TERRAIN_SCALE;
			break;
		case 90:
			HeroPos[0] += TERRAIN_SCALE;
			break;
		case 45:
			HeroPos[0] += TERRAIN_SCALE; HeroPos[1] += TERRAIN_SCALE;
			break;
		case 0:
			HeroPos[1] += TERRAIN_SCALE;
			break;
		default:
			HeroPos[0] -= TERRAIN_SCALE; HeroPos[1] += TERRAIN_SCALE;
			break;
		}	
	}

	if (__IsKeyRepeat(VK_DOWN))
	{
		switch (angle)
		{
		case 315:
			HeroPos[0] += TERRAIN_SCALE; HeroPos[1] -= TERRAIN_SCALE;
			break;
		case 270:
			HeroPos[0] += TERRAIN_SCALE;
			break;
		case 225:
			HeroPos[0] += TERRAIN_SCALE; HeroPos[1] += TERRAIN_SCALE;
			break;
		case 180:
			HeroPos[1] += TERRAIN_SCALE;
			break;
		case 135:
			HeroPos[0] -= TERRAIN_SCALE; HeroPos[1] += TERRAIN_SCALE;
			break;
		case 90:
			HeroPos[0] -= TERRAIN_SCALE;
			break;
		case 45:
			HeroPos[0] -= TERRAIN_SCALE; HeroPos[1] -= TERRAIN_SCALE;
			break;
		case 0:
			HeroPos[1] -= TERRAIN_SCALE;
			break;
		default:
			HeroPos[0] += TERRAIN_SCALE; HeroPos[1] -= TERRAIN_SCALE;
			break;
		}
	}

	if (__IsKeyRepeat(VK_LEFT))
	{
		switch (angle)
		{
		case 315:
			HeroPos[0] -= TERRAIN_SCALE; HeroPos[1] -= TERRAIN_SCALE;
			break;
		case 270:
			HeroPos[1] -= TERRAIN_SCALE;
			break;
		case 225:
			HeroPos[0] += TERRAIN_SCALE; HeroPos[1] -= TERRAIN_SCALE;
			break;
		case 180:
			HeroPos[0] += TERRAIN_SCALE;
			break;
		case 135:
			HeroPos[0] += TERRAIN_SCALE; HeroPos[1] += TERRAIN_SCALE;
			break;
		case 90:
			HeroPos[1] += TERRAIN_SCALE;
			break;
		case 45:
			HeroPos[0] -= TERRAIN_SCALE; HeroPos[1] += TERRAIN_SCALE;
			break;
		case 0:
			HeroPos[0] -= TERRAIN_SCALE;
			break;
		default:
			HeroPos[0] -= TERRAIN_SCALE; HeroPos[1] -= TERRAIN_SCALE;
			break;
		}
	}

	if (__IsKeyRepeat(VK_RIGHT))
	{
		switch (angle)
		{
		case 315:
			HeroPos[0] += TERRAIN_SCALE; HeroPos[1] += TERRAIN_SCALE;
			break;
		case 270:
			HeroPos[1] += TERRAIN_SCALE;
			break;
		case 225:
			HeroPos[0] -= TERRAIN_SCALE; HeroPos[1] += TERRAIN_SCALE;
			break;
		case 180:
			HeroPos[0] -= TERRAIN_SCALE;
			break;
		case 135:
			HeroPos[0] -= TERRAIN_SCALE; HeroPos[1] -= TERRAIN_SCALE;
			break;
		case 90:
			HeroPos[1] -= TERRAIN_SCALE;
			break;
		case 45:
			HeroPos[0] += TERRAIN_SCALE; HeroPos[1] -= TERRAIN_SCALE;
			break;
		case 0:
			HeroPos[0] += TERRAIN_SCALE;
			break;
		default:
			HeroPos[0] += TERRAIN_SCALE; HeroPos[1] += TERRAIN_SCALE;
			break;
		}
	}

	HeroPos[0] = Clamp(HeroPos[0], 0.0f, 256.0f * TERRAIN_SCALE);
	HeroPos[1] = Clamp(HeroPos[1], 0.0f, 256.0f * TERRAIN_SCALE);

	Hero->Object.Position[0] = HeroPos[0];
	Hero->Object.Position[1] = HeroPos[1];
	Hero->Object.Position[2] = HeroPos[2];
}

void WorldEditor::EditObjects()
{
	static ObjCreatePrepare temp;
	static DWORD last_tick = 0;
	
	if (__IsKeyRepeat(VK_SHIFT) && (EditFlag == WorldEditor::EDIT_OBJECT || EditFlag == WorldEditor::EDIT_MONSTER))
	{

		if (__IsKeyPress(VK_LBUTTON) && GetTickCount() - last_tick > 250)
		{
			MouseLButtonPush = 1;
			last_tick = GetTickCount();
		}

		if (__IsKeyPress(VK_RBUTTON) && GetTickCount() - last_tick > 250)
		{
			MouseRButtonPush = 1;
			last_tick = GetTickCount();
		}
	}

	if (EditFlag == EDIT_OBJECT && PickObject)
	{
		if (__IsKeyPress(VK_SPACE))
			SelectLock = !SelectLock;
	}

	EditCamera();

	//======================================================================================

	if (EditFlag == WorldEditor::EDIT_OBJECT || EditFlag == WorldEditor::EDIT_MONSTER)
	{
		static bool moving_object = false;

		if (__IsKeyRepeat(VK_SHIFT))
		{
			if (MouseRButtonPush)
			{
				MouseRButtonPush = 0;
				PickObject = NULL;
				moving_object = false;

				if (EditFlag == WorldEditor::EDIT_OBJECT)
				{
					if (RenderTerrainTile(SelectXF, SelectYF, (int)SelectXF, (int)SelectYF, 1.0f, 1, 1))
					{
						
						PickObject = __CreateObject(ObjCreatePrepare_ctor(&temp, SelectObjectType, CollisionPosition, PickObjectAngle, PickObjectScale, 1, 1, 1));
					}
				}
				else if (EditFlag == WorldEditor::EDIT_MONSTER)
				{
					if (RenderTerrainTile(SelectXF, SelectYF, (int)SelectXF, (int)SelectYF, 1.0, 1, 1))
					{
						CHARACTER* c = __CreateMonster(
							SelectMonsterType,
							((int)CollisionPosition[0] / 100),
							((int)CollisionPosition[1] / 100),
							MonsterKey++,
							0);
						if (c) 
						{
							c->Object.Kind = 32;
							PickMonster = c;
							PickObject = &c->Object;
						}
					}
				}

				if (PickObject)
				{
					if (GridLock)
					{
						PickObject->Position[0] = (float)(50 * ((int)PickObject->Position[0] / 50));
						PickObject->Position[1] = (float)(50 * ((int)PickObject->Position[1] / 50));
					}

					if (!HeightLock)
						PickObjectHeight = 0;

					PickObject->Position[2] = PickObjectHeight + __RequestTerrainHeight(PickObject->Position[0], PickObject->Position[1]);
				}
			}

			if (MouseLButtonPush)
			{
				MouseLButtonPush = 0;
				if (EditFlag == WorldEditor::EDIT_OBJECT)
				{
					if (PickObject && SelectLock)
					{
						moving_object = true;
					}
					else
					{
						OBJECT* o = CollisionDetectObjects();

						if (PickObject && PickObject == o)
						{
							moving_object = true;
						}
						else
						{
							moving_object = false;
							PickObject = o;
						}
					}
				}
				else if (EditFlag == WorldEditor::EDIT_MONSTER)
				{
					CHARACTER* c = CCharacterManager__GetCharacter(CCharacterManager__GetSingleton(), SelectedCharacter);
					if (c && c->Object.Kind == 32)
					{
						if (PickObject && PickObject == &c->Object)
						{
							moving_object = true;
						}
						else
						{
							moving_object = false;
							PickMonster = c;
							PickObject = &c->Object;
							SelectMonsterType = PickMonster->MonsterIndex;
						}
					}
				}

				if (PickObject)
				{
					SelectObjectType = PickObject->Type;
					PickObjectHeight = PickObject->Position[2] - __RequestTerrainHeight(PickObject->Position[0], PickObject->Position[1]);
				}
			}
		}


		if (PickObject)
		{
			if (moving_object)
			{
				if (MouseLButton)
				{
					if (RenderTerrainTile(SelectXF, SelectYF, (int)SelectXF, (int)SelectYF, 1.0, 1, 1))
					{
						PickObject->Position[0] = CollisionPosition[0];
						PickObject->Position[1] = CollisionPosition[1];

						if (GridLock)
						{
							PickObject->Position[0] = (float)(50 * ((int)PickObject->Position[0] / 50));
							PickObject->Position[1] = (float)(50 * ((int)PickObject->Position[1] / 50));
						}

						if (!HeightLock)
						{
							PickObjectHeight = PickObject->Position[2] - __RequestTerrainHeight(PickObject->Position[0], PickObject->Position[1]);
						}

					}
				}
				else
				{
					moving_object = false;
				}
			}

			PickObject->Position[2] = __RequestTerrainHeight(PickObject->Position[0], PickObject->Position[1]) + PickObjectHeight;

			//	NUMPAD MAP
			//	RotX-	Height+		RotX+	
			//	RotZ-	x3			RotZ+
			//	Scale-	Height-		Scale+
			//	0 : Deselect
			float mul = __IsKeyPress(VK_NUMPAD5) | __IsKeyRepeat(VK_NUMPAD5) ? 3.0f : 1.0f;
			if (__IsKeyPress(VK_NUMPAD9))
				PickObject->Angle[0] += (5.0f * mul);
			if (__IsKeyPress(VK_NUMPAD7))
				PickObject->Angle[0] -= (5.0f * mul);
			if (__IsKeyPress(VK_NUMPAD6))
				PickObject->Angle[2] += (15.0f * mul);
			if (__IsKeyPress(VK_NUMPAD4))
				PickObject->Angle[2] -= (15.0f * mul);
			if (__IsKeyPress(VK_NUMPAD8))
				PickObjectHeight += (5.0f * mul);
			if (__IsKeyPress(VK_NUMPAD2))
				PickObjectHeight -= (5.0f * mul);
			if (__IsKeyPress(VK_NUMPAD3))
				PickObject->Scale += (0.1f * mul / 3.0f);
			if (__IsKeyPress(VK_NUMPAD1))
				PickObject->Scale -= (0.1f * mul / 3.0f);
			if (PickObject->Scale < 0.1f)
				PickObject->Scale = 0.1f;
			if (__IsKeyPress(VK_NUMPAD0))
				PickObject = NULL;
			if (__IsKeyPress(VK_DELETE))
			{
				if (EditFlag == WorldEditor::EDIT_OBJECT)
				{
					__DeleteObject(PickObject, &ObjectBlock[PickObject->Block]);
					PickObject = NULL;
				}
				else if (EditFlag == WorldEditor::EDIT_MONSTER)
				{
					if (PickMonster)
						__DeleteCharacter(PickMonster->Key);
					PickMonster = NULL;
					PickObject = NULL;
				}
			}
		}

		if (!PickObject)
			moving_object = false;
	}

	//======================================================================================

	if (EditFlag == WorldEditor::EDIT_HEIGHT && __IsKeyRepeat(VK_SHIFT))
	{
		if (MouseLButton || MouseRButton)
		{
			if (FlatLock)
			{
				SetTerrainHeight(CollisionPosition[0], CollisionPosition[1], FlatHeight, BrushSize + 1, BackTerrainHeight);
			}
			else
			{
				AddTerrainHeight(CollisionPosition[0], CollisionPosition[1], MouseLButton ? 10.0f : -10.0f, BrushSize + 1, BackTerrainHeight);
			}
			__CreateTerrainNormal();
			__CreateTerrainLight();
		}
	}

	//======================================================================================

	if (EditFlag == WorldEditor::EDIT_LIGHT && __IsKeyRepeat(VK_SHIFT))
	{
		if (MouseLButton)
		{
			if (__IsKeyRepeat(VK_CONTROL))
			{
				SetTerrainLight(CollisionPosition[0], CollisionPosition[1], SelectLightColor, BrushSize + 1, TerrainLight);
				__CreateTerrainLight();
			}
			else
			{
				__AddTerrainLightClip(CollisionPosition[0], CollisionPosition[1], SelectLightColor, BrushSize + 1, TerrainLight);
				__CreateTerrainLight();
			}
		}
		if (MouseRButton)
		{
			int mx = (int)(CollisionPosition[0] / TERRAIN_SCALE);
			int my = (int)(CollisionPosition[1] / TERRAIN_SCALE);
			for (int y = my - 2; y <= my + 2; y++)
			{
				for (int x = mx - 2; x <= mx + 2; x++)
				{
					int Index1 = TERRAIN_INDEX_REPEAT(x, y);
					int Index2 = TERRAIN_INDEX_REPEAT(x - 1, y);
					int Index3 = TERRAIN_INDEX_REPEAT(x + 1, y);
					int Index4 = TERRAIN_INDEX_REPEAT(x, y - 1);
					int Index5 = TERRAIN_INDEX_REPEAT(x, y + 1);
					for (int i = 0; i < 3; i++)
					{
						//average value (sum / 5 -> sum * 0.2)
						TerrainLight[Index1][i] = (TerrainLight[Index1][i] + TerrainLight[Index2][i] + TerrainLight[Index3][i] + TerrainLight[Index4][i] + TerrainLight[Index5][i]) * 0.2f;
					}
				}
			}
			__CreateTerrainLight();
		}
	}

	//======================================================================================

	if (EditFlag == WorldEditor::EDIT_MAPPING && __IsKeyRepeat(VK_SHIFT))
	{
		int sx = (int)SelectXF;
		int sy = (int)SelectYF;
		int idx1 = TERRAIN_INDEX_REPEAT(sx, sy);
		int idx2 = TERRAIN_INDEX_REPEAT(sx + 1, sy);
		int idx3 = TERRAIN_INDEX_REPEAT(sx + 1, sy + 1);
		int idx4 = TERRAIN_INDEX_REPEAT(sx, sy + 1);
		if (CGlobalBitmap__GetTexture(&_Bitmaps, SelectTileType + 30256))//->ChannelNum != 4)
		{
			if (MouseLButton)
			{
				if (CurrentLayer == 0)
				{
					for (int i = sy - BrushSize; i <= BrushSize + sy; ++i)
					{
						for (int j = sx - BrushSize; j <= BrushSize + sx; ++j)
							TerrainMappingLayer1[TERRAIN_INDEX_REPEAT(j, i)] = SelectTileType;
					}
				}
				if (CurrentLayer == 1)
				{
					for (int i = sy - 1; i <= sy + 1; ++i)
					{
						for (int j = sx - 1; j <= sx + 1; ++j)
							TerrainMappingLayer2[TERRAIN_INDEX_REPEAT(j, i)] = SelectTileType;
					}
					TerrainMappingAlpha[idx1] += 0.1f;
					TerrainMappingAlpha[idx2] += 0.1f;
					TerrainMappingAlpha[idx3] += 0.1f;
					TerrainMappingAlpha[idx4] += 0.1f;

					if (TerrainMappingAlpha[idx1] > 1.0f)
						TerrainMappingAlpha[idx1] = 1.0f;
					if (TerrainMappingAlpha[idx2] > 1.0f)
						TerrainMappingAlpha[idx2] = 1.0f;
					if (TerrainMappingAlpha[idx3] > 1.0f)
						TerrainMappingAlpha[idx3] = 1.0f;
					if (TerrainMappingAlpha[idx4] > 1.0f)
						TerrainMappingAlpha[idx4] = 1.0f;
				}
			}
			if (MouseRButton && CurrentLayer == 1)
			{
				for (int i = sy - 1; i <= sy + 1; ++i)
				{
					for (int j = sx - 1; j <= sx + 1; ++j)
						TerrainMappingLayer2[TERRAIN_INDEX_REPEAT(j, i)] = 0xFF;
				}
				TerrainMappingAlpha[idx1] = 0.0;
				TerrainMappingAlpha[idx2] = 0.0;
				TerrainMappingAlpha[idx3] = 0.0;
				TerrainMappingAlpha[idx4] = 0.0;
			}
		}
	}

	//======================================================================================

	if (EditFlag == WorldEditor::EDIT_WALL && __IsKeyRepeat(VK_SHIFT))
	{
		if (MouseLButton || MouseRButton)
			SetTerrainWall(CollisionPosition[0], CollisionPosition[1], BrushSize, MouseLButton ? true : false);
	}
}