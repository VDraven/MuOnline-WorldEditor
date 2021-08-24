#ifndef _IMGUI_H_
#define _IMGUI_H_

#define IMGUI_IMPLEMENTATION

#include "imgui/imgui.h"
#include "imgui_impl_opengl2.hpp"
#include "imgui_impl_win32.hpp"

#ifdef IMGUI_IMPLEMENTATION
#include "imgui/imgui.cpp"
#include "imgui/imgui_demo.cpp"
#include "imgui/imgui_draw.cpp"
#include "imgui/imgui_tables.cpp"
#include "imgui/imgui_widgets.cpp"
#endif

#include "imgui/ImGuiFileDialogConfig.h"
#include "imgui/ImGuiFileDialog.h"
#include "imgui/ImGuiFileDialog.cpp"

#endif