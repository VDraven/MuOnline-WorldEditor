#include "WorldEditor.h"
#include "imgui.hpp"

const std::map<int, std::string> WORLD_LIST { WORLD_LIST_DATA };

WNDPROC g_oriWndProc = NULL;
LRESULT CALLBACK HookWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ImGuiContext* context = ImGui::GetCurrentContext();
	if (context && context->HoveredWindow)
	{
		if (WorldEditor::Instance()->WndProcHandler(hwnd, uMsg, wParam, lParam))
		{
			return 1;
		}
	}

	return CallWindowProc(g_oriWndProc, hwnd, uMsg, wParam, lParam);
}

void WorldEditor::InitUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	//ImGuiIO& io = ImGui::GetIO(); //(void)io;
	//io.MouseDrawCursor = true;
	//io.WantCaptureMouse = 1;

	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplOpenGL2_Init();

	g_oriWndProc = (WNDPROC)SetWindowLong(g_hWnd, GWLP_WNDPROC, (LONG)&HookWndProc);
}

IMGUI_IMPL_API LRESULT WorldEditor::WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui::GetCurrentContext() == NULL)
		return 0;

	ImGuiIO& io = ImGui::GetIO();

	switch (msg)
	{
	case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
	case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
	{
		int button = 0;
		if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK) { button = 0; }
		if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK) { button = 1; }
		if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK) { button = 2; }
		if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONDBLCLK) { button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4; }
		if (!ImGui::IsAnyMouseDown() && ::GetCapture() == NULL)
			::SetCapture(hwnd);
		io.MouseDown[button] = true;
		return 1;
	}
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_XBUTTONUP:
	{
		int button = 0;
		if (msg == WM_LBUTTONUP) { button = 0; }
		if (msg == WM_RBUTTONUP) { button = 1; }
		if (msg == WM_MBUTTONUP) { button = 2; }
		if (msg == WM_XBUTTONUP) { button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4; }
		io.MouseDown[button] = false;
		if (!ImGui::IsAnyMouseDown() && ::GetCapture() == hwnd)
			::ReleaseCapture();
		return 1;
	}
	case WM_MOUSEWHEEL:
		io.MouseWheel += (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
		return 1;
	case WM_MOUSEHWHEEL:
		io.MouseWheelH += (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
		return 1;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (wParam < 256)
		{
			io.KeysDown[wParam] = 1;
		}

		return 1;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (wParam < 256)
			io.KeysDown[wParam] = 0;
		return 1;
	case WM_KILLFOCUS:
		memset(io.KeysDown, 0, sizeof(io.KeysDown));
		return 1;
	case WM_CHAR:
		// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
		if (wParam > 0 && wParam < 0x10000)
			io.AddInputCharacterUTF16((unsigned short)wParam);
		return 1;
	}
	return 0;
}

void WorldEditor::SetFreeMode()
{
	if (FreeMode)
	{
		HeroPos[0] = HeroPosOld[0] = Hero->Object.Position[0];
		HeroPos[1] = HeroPosOld[1] = Hero->Object.Position[1];
		HeroPos[2] = HeroPosOld[2] = Hero->Object.Position[2];

		__RegisterBuff(BUFF_ADMIN_INVISIBLE, &Hero->Object, 0, 0);
	}
	else
	{
		Hero->Object.Position[0] = HeroPosOld[0];
		Hero->Object.Position[1] = HeroPosOld[1];
		Hero->Object.Position[2] = HeroPosOld[2];

		CameraDistance = 1000.0f;
		CameraAngle[2] = -45.0f;

		__UnRegisterBuff(BUFF_ADMIN_INVISIBLE, &Hero->Object);
	}
}

void WorldEditor::UI()
{
	ImGuiContext* context = ImGui::GetCurrentContext();
	if (!context) return;

	context->IO.MouseDrawCursor = context->HoveredWindow ? true : false;

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("VD's MuOnline WorldEditor");

	if (WORLD_LIST.find(World) != WORLD_LIST.end())
		ImGui::Text("%d : %s ", World, WORLD_LIST.at(World).c_str());
	else
		ImGui::Text("%d : Unknown World ", World);
	if (Hero)
	{
		int x = (int) (Hero->Object.Position[0] / TERRAIN_SCALE);
		int y = (int)(Hero->Object.Position[1] / TERRAIN_SCALE);
		ImGui::SameLine();
		ImGui::Text(" (X : %d, Y : %d)", x, y);
	}


	Frame3D.Active = BrushRing = SelectRing = false;

	static bool EditOn = false;
	ImGui::Checkbox("EditFlag", &EditOn);

	if (EditOn)
	{
		if (ImGui::Checkbox("FreeMode", &FreeMode))
		{
			SetFreeMode();
		}

		if (FreeMode)
		{
			ImGui::DragFloat("CameraDistance", &CameraDistance, 25.0f, 700.0f, 2000.0f);
			ImGui::DragFloat("CameraAngleZ", &CameraAngle[2], 5.0f, -360.0f, 360.0f, "%.0f", ImGuiSliderFlags_AlwaysClamp);
		}
		
		ImGui::Separator();

		if (ImGui::BeginTabBar("##Tabs"))
		{
			if (ImGui::BeginTabItem("Terrain"))
			{
				if (ImGui::BeginTabBar("##Tabs_Terrain"))
				{
					if (ImGui::BeginTabItem("Tile"))
					{
						UI_EditFlag_1();

						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem("Height"))
					{
						UI_EditFlag_4();

						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem("Light"))
					{
						UI_EditFlag_5();

						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem("Wall"))
					{
						UI_EditFlag_3();

						ImGui::EndTabItem();
					}
					

					ImGui::EndTabBar();
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Objects"))
			{
				if (ImGui::BeginTabBar("##Tabs_Objects"))
				{
					if (ImGui::BeginTabItem("Assets"))
					{
						UI_EditFlag_2();

						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem("Monsters"))
					{
						UI_EditFlag_7();

						ImGui::EndTabItem();
					}
					ImGui::EndTabBar();
				}

				UI_EditObject();

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("SaveWorld"))
			{
				UI_SaveWorld();

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("LoadWorld"))
			{
				UI_LoadWorld();

				ImGui::EndTabItem();
			}


			ImGui::EndTabBar();
		}
	}
	else
	{
		EditFlag = WorldEditor::EDIT_NONE;
	}

	ImGui::End();

	ImGui::EndFrame();

	ImGui::Render();

	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());


	if (Frame3D.Active)
	{
		if(EditFlag == WorldEditor::EDIT_OBJECT)
			RenderPreview3D(SelectObjectType, Frame3D.PosX, Frame3D.PosY, Frame3D.Width, Frame3D.Height, Frame3D.CenterView);
		else if (EditFlag == WorldEditor::EDIT_MONSTER && PickMonster)
			RenderPreview3D(PickMonster->Object.Type, Frame3D.PosX, Frame3D.PosY, Frame3D.Width, Frame3D.Height);
	}
}

void WorldEditor::UI_SaveWorld()
{
	EditFlag = WorldEditor::EDIT_SAVE;

	SaveWorldConfig.nWorldID = __GetWorldID(SaveWorldConfig.nWorld);
	ImGui::Text("SaveWorldID: %d", SaveWorldConfig.nWorldID);

	ImGui::InputInt("SaveWorld", &SaveWorldConfig.nWorld, 1, 10);
	if (SaveWorldConfig.nWorld < 0) SaveWorldConfig.nWorld = 0;

	const char* combo_preview_value;
	if (WORLD_LIST.find(SaveWorldConfig.nWorld) != WORLD_LIST.end())
		combo_preview_value = WORLD_LIST.at(SaveWorldConfig.nWorld).c_str();
	else
		combo_preview_value = "Unk World !!!";
	if (ImGui::BeginCombo("##SaveWorldList", combo_preview_value))
	{
		for (auto& p : WORLD_LIST)
		{
			const bool is_selected = (p.first == SaveWorldConfig.nWorld);

			if (ImGui::Selectable(p.second.c_str(), is_selected))
				SaveWorldConfig.nWorld = p.first;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	ImGui::Separator();

	ImGui::Text("SaveDir: ");
	ImGui::TextWrapped("%s\\World%d", Plugin.DirSave.c_str(), SaveWorldConfig.nWorldID);

	ImGui::Combo("AttributeType", (int*)&SaveWorldConfig.SaveTerrainAttributeType, "ATT_64K\0ATT_128K\0\0");
	ImGui::Combo("HeightType", (int*)&SaveWorldConfig.SaveTerrainHeightType, "OZB_64K\0OZB_192K\0\0");
	ImGui::Combo("ObjectsType", (int*)&SaveWorldConfig.SaveObjectsType, "OBJ_V0\0OBJ_V1\0OBJ_V2\0OBJ_V3\0\0");

	ImGui::Separator();

	static DWORD LastSave = 0;
	bool cooling_down = (GetTickCount() - LastSave) < 5000;
	if (cooling_down)
	{
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "The world has been saved to \"%s\\World%d\"", Plugin.DirSave.c_str(), SaveWorldConfig.nWorldID);
	}
	if (!cooling_down && ImGui::Button("Save"))
	{
		LastSave = GetTickCount();
		SaveWorld();
	}
}

void WorldEditor::UI_LoadWorld()
{
	EditFlag = WorldEditor::EDIT_LOAD;

	LoadWorldConfig.nWorldID = __GetWorldID(LoadWorldConfig.nWorld);
	ImGui::Text("LoadWorldID: %d", LoadWorldConfig.nWorldID);

	ImGui::InputInt("LoadWorld", &LoadWorldConfig.nWorld, 1, 10);
	if (LoadWorldConfig.nWorld < 0) LoadWorldConfig.nWorld = 0;

	const char* combo_preview_value;
	if (WORLD_LIST.find(LoadWorldConfig.nWorld) != WORLD_LIST.end())
		combo_preview_value = WORLD_LIST.at(LoadWorldConfig.nWorld).c_str();
	else
		combo_preview_value = "Unk World !!!";
	if (ImGui::BeginCombo("##LoadWorldList", combo_preview_value))
	{
		for (auto& p : WORLD_LIST)
		{
			const bool is_selected = (p.first == LoadWorldConfig.nWorld);

			if (ImGui::Selectable(p.second.c_str(), is_selected))
				LoadWorldConfig.nWorld = p.first;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	ImGui::Separator();

	ImGui::Text("LoadDirs: ");
	char szDirWorld[256];
	char szDirObject[256];
	sprintf(szDirWorld, "Data\\World%d", LoadWorldConfig.nWorldID);
	sprintf(szDirObject, "Data\\Object%d", LoadWorldConfig.nWorldID);

	const ImVec4 RED_COLOR = { 1.0f, 0.0f, 0.0f, 1.0f };
	const ImVec4 GREEN_COLOR = { 0.0f, 1.0f, 0.0f, 1.0f };
	bool b1 = fs::exists(szDirWorld);
	bool b2 = fs::exists(szDirObject);
	ImGui::TextColored(b1 ? GREEN_COLOR : RED_COLOR, szDirWorld);
	ImGui::TextColored(b2 ? GREEN_COLOR : RED_COLOR, szDirObject);

	// will check it later by file size in 
	// bool HookTerrainHeightExt(int world)
	//ImGui::Combo("HeightType", (int*)&LoadWorldConfig.LoadTerrainHeightType, "OZB_64K\0OZB_192K\0\0");

	ImGui::Separator();

	if (!b1 || !b2) return;

	static DWORD LastLoad = 0;
	bool cooling_down = (GetTickCount() - LastLoad) < 5000;
	if (cooling_down)
	{
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "The world has been Loaded from \"Data\\World%d\"", LoadWorldConfig.nWorldID);
	}
	if (!cooling_down && ImGui::Button("Load"))
	{
		LastLoad = GetTickCount();
		World = LoadWorldConfig.nWorld;
		__OpenWorld(World); 
		FreeMode = true;
		SetFreeMode();
	}
}

void WorldEditor::UI_EditObject()
{
	if (ImGui::TreeNode("3D Preview"))
	{
		static bool AutoRotate = true;
		static bool CenterView = false;

		if (ImGui::BeginListBox("##Frame3D", ImVec2(-FLT_MIN, 200.0f)))
		{
			Frame3D.Active = true;

			ImGui::Checkbox("AutoRotate", &AutoRotate); 
			if (EditFlag == WorldEditor::EDIT_OBJECT) 
			{
				ImGui::SameLine();
				ImGui::Checkbox("CenterView", &CenterView);
			}

			ImVec2 vMin, vMax;
			vMin = ImGui::GetWindowContentRegionMin();
			vMax = ImGui::GetWindowContentRegionMax();

			vMin.x += ImGui::GetWindowPos().x;
			vMin.y += ImGui::GetWindowPos().y;
			vMax.x += ImGui::GetWindowPos().x;
			vMax.y += ImGui::GetWindowPos().y;

			ImGui::GetForegroundDrawList()->AddRect(vMin, vMax, IM_COL32(255, 255, 0, 255));

			Frame3D.PosX = vMin.x;
			Frame3D.PosY = vMin.y;
			Frame3D.Width = vMax.x - vMin.x;
			Frame3D.Height = vMax.y - vMin.y;

			ImGui::EndListBox();
		}

		if (AutoRotate)
			Frame3D.Angle[1] = (float)((GetTickCount() / 10) % 360);
		Frame3D.CenterView = CenterView;


		ImGui::DragFloat("##Frame3D_Zoom", &Frame3D.Zoom, 0.05f, 0.25f, 10.0f, "x%.2f");
		ImGui::DragFloat3("##Frame3D_Angle", Frame3D.Angle, 5.0f, 0.0f, 360.0f, "%.0f");

		ImGui::TreePop();
	}

	ImGui::Separator();

	if (ImGui::TreeNode("Edit"))
	{
		if (PickObject)
			ImGui::Checkbox("SelectLock", &SelectLock);

		if (ImGui::Button("reset"))
		{
			PickObjectAngle[0] = PickObjectAngle[1] = PickObjectAngle[2] = 0.0f;
			PickObjectScale = 1.0f;
			PickObjectHeight = 0.0f;
		}

		if (PickObject)
		{
			ImGui::SameLine();
			if (ImGui::Button("drop"))
			{
				SelectLock = false;
				PickObject = NULL;
			}
		}
			
		if (PickObject)
		{
			ImGui::SameLine();
			if (ImGui::Button("teleport"))
			{
				if (!FreeMode) {
					FreeMode = true;
					SetFreeMode();
				}

				int x = (int)(PickObject->Position[0] / TERRAIN_SCALE);
				int y = (int)(PickObject->Position[1] / TERRAIN_SCALE);

				HeroPos[0] = Hero->Object.Position[0] = (x + 0.5f) * TERRAIN_SCALE;
				HeroPos[1] = Hero->Object.Position[1] = (y + 0.5f) * TERRAIN_SCALE;
			}
		}

		if (PickObject)
		{
			PickObject->Angle[0] = (float)((int)PickObject->Angle[0] % 360);
			PickObject->Angle[1] = (float)((int)PickObject->Angle[1] % 360);
			PickObject->Angle[2] = (float)((int)PickObject->Angle[2] % 360);

			memcpy(PickObjectAngle, PickObject->Angle, sizeof(vec3_t));
			PickObjectScale = PickObject->Scale;

			ImGui::LabelText("", "PickObject:");
			BMD* bmd = CNewModelManager__GetModel(CNewModelManager__GetSingleton(), PickObject->Type);
			if (bmd)	ImGui::LabelText("", "(%s)", bmd->FlagName ? bmd->NewName.c_str() : bmd->OldName);

			ImGui::InputFloat3("Position", PickObject->Position, "%.0f", ImGuiInputTextFlags_ReadOnly);
			ImGui::DragFloat3("Angle", PickObject->Angle, 1.0f, 0.0f, 360.0f, "%.0f");
			ImGui::DragFloat("Scale", &PickObject->Scale, 0.05f, 0.1f, 4.0f, "x%.2f");
			ImGui::DragFloat("Height", &PickObjectHeight, 1.0f, -500.0f, 500.0f, "%.0f");

			ImGui::Separator();

			ImGui::Combo("SaveObjectsType", (int*)&SaveWorldConfig.SaveObjectsType, "OBJ_V0\0OBJ_V1\0OBJ_V2\0OBJ_V3\0\0");

			if (SaveWorldConfig.SaveObjectsType >= 1)
			{
				ImGui::Checkbox("Light1", &PickObject->LightEnable1);
				ImGui::SameLine();
				ImGui::Checkbox("Light2", &PickObject->LightEnable2);
			}

			if (SaveWorldConfig.SaveObjectsType >= 2)
			{
				ImGui::SameLine();
				ImGui::Checkbox("Light3", &PickObject->LightEnable3);
			}

			if (SaveWorldConfig.SaveObjectsType >= 3)
			{
				constexpr ImGuiColorEditFlags FLAGS = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueWheel;
				ImGui::ColorEdit4("", PickObject->Light, FLAGS);
				ImGui::SameLine();
				ImGui::DragFloat3("Light", PickObject->Light, 0.05f, 0.0f, 1.0f, "%.2f");
			}

			if (SaveWorldConfig.SaveObjectsType >= 4)
			{
				static int CurrentAction = 0;
				CurrentAction = PickObject->CurrentAction;
				ImGui::DragInt("Action", &CurrentAction, 1.0f, 0, 1000);
				PickObject->CurrentAction = CurrentAction;

				ImGui::InputFloat("AnimFrame", &PickObject->AnimationFrame, 0.0f, 0.0f, "%.1f", ImGuiInputTextFlags_ReadOnly);
			}
		}
		else
		{
			ImGui::DragFloat3("Angle", PickObjectAngle, 1.0f, 0.0f, 360.0f, "%.0f");
			ImGui::DragFloat("Scale", &PickObjectScale, 0.05f, 0.1f, 4.0f, "x%.2f");
			ImGui::DragFloat("Height", &PickObjectHeight, 1.0f, 0.0f, 1000.0f, "%.0f");
		}

		ImGui::Separator();

		ImGui::Checkbox("GridLock", &GridLock);
		ImGui::SameLine();
		ImGui::Checkbox("HeightLock", &HeightLock);

		ImGui::TreePop();
	}
}

void WorldEditor::UI_EditFlag_1()
{
	EditFlag = WorldEditor::EDIT_MAPPING;

	BrushRing = true;
	ImGui::SliderInt("BrushSize", &BrushSize, 0, 9);

	if (ImGui::RadioButton("Layer 0", CurrentLayer == 0))
		CurrentLayer = 0;
	ImGui::SameLine();
	if (ImGui::RadioButton("Layer 1", CurrentLayer == 1))
		CurrentLayer = 1;

	ImGui::Separator();

	if (ImGui::BeginTabBar("##TabsTiles"))
	{
		if (ImGui::BeginTabItem("World"))
		{
			if (ImGui::BeginListBox("", ImVec2(-FLT_MIN, 4 * 64.0f)))
			{
				int table_flag = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders;
				if (ImGui::BeginTable("", 3, table_flag))
				{
					for (int i = 0; i < 31; i++)
					{
						BITMAP_t* bitmap = CGlobalBitmap__GetTexture(&_Bitmaps, 30256 + i);
						
						if (bitmap->BitmapIndex)
						{
							ImGui::TableNextRow();

							ImGui::TableNextColumn();
							ImGui::Text("%d", i);

							ImGui::TableNextColumn();
							ImGui::Image((void*)bitmap->TextureNumber, ImVec2(64.0f, 64.0f));

							ImGui::TableNextColumn();
							if (ImGui::Selectable(bitmap->FileName, SelectTileType == i, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0.0f, 64.0f)))
								SelectTileType = i;
						}

					}
					ImGui::EndTable();
				}
				ImGui::EndListBox();
			}

			ImGui::EndTabItem();
		}

		if (Extension.Active)
		{
			if (ImGui::BeginTabItem("Ext"))
			{
				if (ImGui::BeginListBox("", ImVec2(-FLT_MIN, 4 * 64.0f)))
				{
					int table_flag = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders;
					if (ImGui::BeginTable("", 3, table_flag))
					{
						for (auto it = Extension.Tiles.begin(); it != Extension.Tiles.end(); it++)
						{
							GLuint id = it->second;
							BITMAP_t* ext_bitmap = Bitmaps.GetTexture(id);
							if (ext_bitmap->BitmapIndex)
							{
								ImGui::TableNextRow();

								ImGui::TableNextColumn();
								ImGui::Image((void*)ext_bitmap->TextureNumber, ImVec2(64.0f, 64.0f));

								ImGui::TableNextColumn();
								static GLuint select_ext;
								if (ImGui::Selectable(ext_bitmap->FileName, select_ext == id, 0, ImVec2(0.0f, 64.0f)))
									select_ext = id;

								ImGui::TableNextColumn();

								if (select_ext != id) continue;

								bool is_imported = false;
								bool has_empty_slot = false;
								for (GLuint i = 0; i < 31; i++)
								{
									BITMAP_t* bitmap = CGlobalBitmap__GetTexture(&_Bitmaps, 30256 + i);
									if (bitmap->BitmapIndex == 0)
									{
										has_empty_slot = true;
									}
									else if (bitmap->BitmapIndex == ext_bitmap->BitmapIndex)
									{
										is_imported = true;
										ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Imported (%d)", i);
										break;
									}
								}
								if (!is_imported)
								{
									if (!has_empty_slot)
									{
										ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "No empty slot");
									}
									else if (ImGui::Button("Import", ImVec2(0.0f, 64.0f)))
									{
										for (GLuint i = 0; i < 31; i++)
										{
											BITMAP_t* bitmap = CGlobalBitmap__GetTexture(&_Bitmaps, 30256 + i);
											if (bitmap->BitmapIndex == 0)
											{	//found an empty slot
												CBitmapCache__Add(CGlobalBitmap__m_BitmapCache, 30256 + i, ext_bitmap);

												//double check then add to imported list
												bitmap = CGlobalBitmap__GetTexture(&_Bitmaps, 30256 + i);
												if (bitmap->BitmapIndex)
													Extension.ImportedTiles.insert(std::make_pair(30256 + i, bitmap->FileName));

												break;
											}
										}
									}
								}
							}
						}

						ImGui::EndTable();
					}

					ImGui::EndListBox();
				}

				ImGui::EndTabItem();
			}
		}
		
		ImGui::EndTabBar();
	}
}

void WorldEditor::UI_EditFlag_2_Local()
{
	static int Range = 8;
	// max range (radius) = diagonal edge = 256 * sqrt(2) ~= 362 < 400 OK!
	ImGui::SliderInt("Range", &Range, 1, 400, "%d", ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp);

	typedef std::vector<OBJECT*> OBJECT_LIST;

	std::map<int, OBJECT_LIST> ObjectList;

	int counter_type = 0;
	int counter_object = 0;
	float x1= Hero->Object.Position[0];
	float y1 = Hero->Object.Position[1];
	int ri = (Range / 16) + 1; 
	int rj = (Range / 16) + 1;
	int si = (int) (x1 / (16 * TERRAIN_SCALE));
	int sj = (int) (y1 / (16 * TERRAIN_SCALE));

#define CLAMP_BLOCK(i) Clamp(i, 0, 15)
	for (int i = CLAMP_BLOCK(si - ri); i <= CLAMP_BLOCK(si + ri); i++)
	{
		for (int j = CLAMP_BLOCK(sj - rj); j <= CLAMP_BLOCK(sj + rj); j++)
		{
			OBJECT_BLOCK* ob = &ObjectBlock[i * 16 + j];
			OBJECT* o = ob->Head;
			while (o != NULL)
			{
				float x2 = o->Position[0];
				float y2 = o->Position[1];
				float dx = x1 - x2;
				float dy = y1 - y2;
				float distance = sqrtf(dx * dx + dy * dy);
				if (distance < Range * TERRAIN_SCALE)
				{
					if (ObjectList.find(o->Type) == ObjectList.end())
					{
						ObjectList.insert(std::make_pair(o->Type, OBJECT_LIST()));
						counter_type++;
					}
					OBJECT_LIST& ol = ObjectList[o->Type];
					ol.push_back(o);
					counter_object++;
				}

				o = o->Next;
			}
		}
	}

	ImGui::Text("[Counter] Types: %d, Objects: %d", counter_type, counter_object);

	if (ImGui::BeginListBox("", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing())))
	{
		int table_flag = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders;
		if (ImGui::BeginTable("SelectModel", 3, table_flag))
		{
			for (auto it = ObjectList.begin(); it != ObjectList.end(); it++)
			{
				int type = it->first;
				OBJECT_LIST& ol = it->second;
				int count = ol.size();

				BMD* bmd = CNewModelManager__GetModel(CNewModelManager__GetSingleton(), type);
				if (bmd && bmd->NumMeshs)
				{
					ImGui::TableNextRow();

					ImGui::TableNextColumn();
					ImGui::Text("%d", type);

					ImGui::TableNextColumn();
					ImGuiTreeNodeFlags node_flag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth;
					if (SelectObjectType == type)
						node_flag |= ImGuiTreeNodeFlags_Selected;
					bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)type, node_flag, bmd->FlagName ? bmd->NewName.c_str() : bmd->OldName);
					
					if (ImGui::IsItemClicked())
					{
						PickObject = NULL;
						SelectObjectType = type;

						Frame3D.Angle[0] = 270.0f;
						Frame3D.Angle[1] = 0.0f;
						Frame3D.Angle[2] = 0.0f;
					}

					ImGui::TableNextColumn();
					ImGui::Text("%d", count);


					if (node_open)
					{
						for (size_t i = 0; i < ol.size(); i++)
						{
							ImGui::TableNextRow();

							ImGui::TableNextColumn();
							ImGui::TableNextColumn();

							OBJECT* o = ol[i];
							if (!o) continue;

							char coord[256];
							int x = (int)(o->Position[0] / TERRAIN_SCALE);
							int y = (int)(o->Position[1] / TERRAIN_SCALE);

							sprintf(coord, "X: %d Y: %d", x, y);

							if (ImGui::Selectable(coord, PickObject == o))
							{
								PickObject = o;
								SelectObjectType = type;
								PickObjectHeight = PickObject->Position[2] - __RequestTerrainHeight(PickObject->Position[0], PickObject->Position[1]);

								Frame3D.Angle[0] = 270.0f;
								Frame3D.Angle[1] = 0.0f;
								Frame3D.Angle[2] = 0.0f;
							}

							ImGui::TableNextColumn();
							if (PickObject == o)
							{
								if (ImGui::Button("teleport"))
								{
									if (!FreeMode)
									{
										FreeMode = true;
										SetFreeMode();
									}

									HeroPos[0] = Hero->Object.Position[0] = (x + 0.5f) * TERRAIN_SCALE;
									HeroPos[1] = Hero->Object.Position[1] = (y + 0.5f) * TERRAIN_SCALE;
								}
							}
						}


						ImGui::TreePop();
					}
				}
			}

			ImGui::EndTable();
		}

		ImGui::EndListBox();
	}
}

void WorldEditor::UI_EditFlag_2_All()
{
	if (ImGui::BeginListBox("", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing())))
	{
		constexpr int table_flag = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders;
		if (ImGui::BeginTable("SelectModel", 3, table_flag))
		{
			VS2008::map<int, BMD*>* m = (VS2008::map<int, BMD*>*) ModelMap;

			for (auto it = m->begin(); it != m->end(); it++)
			{
				int type = it->first;
				BMD* bmd = it->second;

				if (bmd && bmd->NumMeshs > 0)
				{
					ImGui::TableNextRow();

					ImGui::TableNextColumn();
					ImGui::Text("%d", type);

					ImGui::TableNextColumn();
					if (ImGui::Selectable(bmd->FlagName ? bmd->NewName.c_str() : bmd->OldName,
						SelectObjectType == type,
						ImGuiSelectableFlags_SpanAllColumns))
					{
						PickObject = NULL;
						SelectObjectType = type;
						Frame3D.Angle[0] = 270.0f;
						Frame3D.Angle[1] = 0.0f;
						Frame3D.Angle[2] = 0.0f;
					}
				}
			}

			ImGui::EndTable();
		}

		ImGui::EndListBox();
	}
}

void WorldEditor::UI_EditFlag_2()
{
	EditFlag = WorldEditor::EDIT_OBJECT;

	SelectRing = true;

	ImGui::InputInt("SelectObject", &SelectObjectType, 0, 0);//, ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginTabBar(""))
	{
		if (ImGui::BeginTabItem("Local"))
		{
			UI_EditFlag_2_Local();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("All"))
		{
			UI_EditFlag_2_All();

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
}

void WorldEditor::UI_EditFlag_3()
{
	EditFlag = WorldEditor::EDIT_WALL;

	BrushRing = true;

	ImGui::SliderInt("BrushSize", &BrushSize, 0, 9);
	ImGui::Separator();

	constexpr ImGuiColorEditFlags FLAGS = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueWheel;

#define RADIO_BUTTON_TW(TW) RADIO_BUTTON_TW_COLOR(TW, 0.0f, 1.0f, 1.0f)
#define RADIO_BUTTON_TW_COLOR(TW, R, G, B)									\
	if (ImGui::RadioButton(#TW, SelectWallType == TW)) SelectWallType = TW;	\
	static vec4_t COLOR_##TW = {R, G, B, 0.3f};								\
	if (SelectWallType == TW) {												\
		ImGui::SameLine();													\
		ImGui::ColorEdit4("", COLOR_##TW, FLAGS);							\
		SelectWallColor[0] = COLOR_##TW[0];									\
		SelectWallColor[1] = COLOR_##TW[1];									\
		SelectWallColor[2] = COLOR_##TW[2];									\
		SelectWallColor[3] = COLOR_##TW[3];									\
	}

	RADIO_BUTTON_TW_COLOR(TW_NONE, 0.5f, 0.5f, 0.5f);
	RADIO_BUTTON_TW_COLOR(TW_SAFEZONE, 0.0f, 1.0f, 0.0f);
	RADIO_BUTTON_TW_COLOR(TW_CHARACTER, 0.0f, 0.0f, 1.0f);
	RADIO_BUTTON_TW_COLOR(TW_NOMOVE, 1.0f, 0.0f, 0.0f);
	RADIO_BUTTON_TW(TW_NOGROUND);
	RADIO_BUTTON_TW(TW_WATER);
	RADIO_BUTTON_TW(TW_ACTION);
	RADIO_BUTTON_TW(TW_HEIGHT);
	RADIO_BUTTON_TW(TW_CAMERA_UP);
	RADIO_BUTTON_TW(TW_NOATTACKZONE);
	RADIO_BUTTON_TW(TW_UNK_ATT1);
	RADIO_BUTTON_TW(TW_UNK_ATT2);
	RADIO_BUTTON_TW(TW_UNK_ATT3);
	RADIO_BUTTON_TW(TW_UNK_ATT4);
	RADIO_BUTTON_TW(TW_UNK_ATT5);
	RADIO_BUTTON_TW(TW_UNK_ATT6);
	RADIO_BUTTON_TW(TW_UNK_ATT7);

}

void WorldEditor::UI_EditFlag_4()
{
	EditFlag = WorldEditor::EDIT_HEIGHT;

	static float Height;
	BrushRing = true;

	ImGui::SliderInt("BrushSize", &BrushSize, 0, 9);
	ImGui::Separator();

	Height = __RequestTerrainHeight(CollisionPosition[0], CollisionPosition[1]);
	ImGui::InputFloat("Height", &Height, 0.0f, 0.0f, "%.2f", ImGuiInputTextFlags_ReadOnly);

	if(ImGui::Checkbox("Flatten", &FlatLock))
		FlatHeight = Height;
	if (FlatLock)
	{
		ImGui::SameLine();
		ImGui::DragFloat("", &FlatHeight);
	}

	static float AdjustBaseValue = 0.0f;
	ImGui::DragFloat("AdjustBaseValue", &AdjustBaseValue, 1.0f, -10.0f, 10.0f, "%.0f");
	if (ImGui::Button("AdjustBase"))
	{
		for (int i = 0; i < TERRAIN_SIZE * TERRAIN_SIZE; i++)
			BackTerrainHeight[i] += AdjustBaseValue;
	}

}

void WorldEditor::UI_EditFlag_5()
{
	EditFlag = WorldEditor::EDIT_LIGHT;

	BrushRing = true;
	ImGui::SliderInt("BrushSize", &BrushSize, 0, 9);
	ImGui::Separator();

	static vec3_t Light;
	__RequestTerrainLight(CollisionPosition[0], CollisionPosition[1], Light);
	ImGui::InputFloat3("LightColor", Light, "%.3f", ImGuiInputTextFlags_ReadOnly);

	ImGui::ColorPicker3("##ColorPicker", SelectLightColor, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_Float);
}

void WorldEditor::UI_EditFlag_6()
{
	EditFlag = WorldEditor::EDIT_SOUND;

}

void WorldEditor::UI_EditFlag_7()
{
	EditFlag = WorldEditor::EDIT_MONSTER;

	SelectRing = true;

	ImGui::InputInt("SelectMonster", &SelectMonsterType, 0, 0, ImGuiInputTextFlags_ReadOnly);
	if (ImGui::BeginListBox("##ListMonster", ImVec2(-FLT_MIN, 7 * ImGui::GetTextLineHeightWithSpacing())))
	{
		int table_flag = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders;

		VS2008::map<int, Data_NpcName>* m = (VS2008::map<int, Data_NpcName>*)NpcNameMap;
		for (auto it = m->begin(); it != m->end(); it++)
		{
			Data_NpcName& data = it->second;
			if (ImGui::Selectable(data.Name.c_str(), SelectMonsterType == data.Type, 0))
			{
				SelectMonsterType = data.Type;
			}
		}

		ImGui::EndListBox();
	}
}
