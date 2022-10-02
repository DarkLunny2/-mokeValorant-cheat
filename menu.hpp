#pragma once
#include "../../Includes/Imgui/imgui.h"
#include "../../Includes/Imgui/imgui_internal.h"
#include "string.h"
#include <string>

namespace bools
{
	static float boxsize = 1;
	static float boxheight = 1;
	static float boxwidth = 1;
	static float tracersize = 0.1f;
	static bool aimbot = false;
	static bool headesp = false;
	static int fovvalue = 150;
	static bool aimbotfov = false;

}

namespace hotkeys
{
	int aimkey;
	int airstuckey;
	int instares;
}

static int keystatus = 0;
static int realkey = 0;

bool GetKey(int key)
{
	realkey = key;
	return true;
}

void ChangeKey(void* blank)
{
	keystatus = 1;
	while (true)
	{
		for (int i = 0; i < 0x87; i++)
		{
			if (GetKeyState(i) & 0x8000)
			{
				hotkeys::aimkey = i;
				keystatus = 0;
				return;
			}
		}
	}
}

static const char* keyNames[] =
{
	"Press any key..",
	"Left Mouse",
	"Right Mouse",
	"Cancel",
	"Middle Mouse",
	"Mouse 5",
	"Mouse 4",
	"",
	"Backspace",
	"Tab",
	"",
	"",
	"Clear",
	"Enter",
	"",
	"",
	"Shift",
	"Control",
	"Alt",
	"Pause",
	"Caps",
	"",
	"",
	"",
	"",
	"",
	"",
	"Escape",
	"",
	"",
	"",
	"",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"",
	"",
	"",
	"Print",
	"Insert",
	"Delete",
	"",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"",
	"",
	"",
	"",
	"",
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",
	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",
	"Numpad 8",
	"Numpad 9",
	"Multiply",
	"Add",
	"",
	"Subtract",
	"Decimal",
	"Divide",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
};

static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
	const char* const* items = (const char* const*)data;
	if (out_text)
		*out_text = items[idx];
	return true;
}


void HotkeyButton(int aimkey, void* changekey, int status)
{
	const char* preview_value = NULL;
	if (aimkey >= 0 && aimkey < IM_ARRAYSIZE(keyNames))
		Items_ArrayGetter(keyNames, aimkey, &preview_value);

	std::string aimkeys;
	if (preview_value == NULL)
		aimkeys = "Select Key";
	else
		aimkeys = preview_value;

	if (status == 1)
	{

		aimkeys = "Press the Key";
	}
	if (ImGui::Button(aimkeys.c_str(), ImVec2(125, 20)))
	{
		if (status == 0)
		{
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekey, nullptr, 0, nullptr);
		}
	}
}



///////////////////////////////////////


static float FovCircleColor[3] = { 0.f, 1.0f, 0.f };

auto ColorRNew = FovCircleColor[0] * 255;
auto ColorGNew = FovCircleColor[1] * 255;
auto ColorBNew = FovCircleColor[2] * 255;


/////////////////////////////
std::string space = " ";
static int tab = 0;
//////////////////

auto drawmenu() -> void
{
	if (GetAsyncKeyState(VK_INSERT) & 1) { Settings::bMenu = !Settings::bMenu; }
	 
	if (Settings::bMenu)
	{
		ImGui::SetNextWindowSize({ 350,600 });
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoBackground;
		// etc.
		bool* open_ptr ;
		ImGui::Begin("$moke.val", open_ptr, window_flags); {

			ImGui::Spacing();
			ImGui::Checkbox("3D Box", &Settings::Visuals::player_box_3d);
			ImGui::Checkbox("2D Box", &Settings::Visuals::bBox);
			ImGui::Checkbox("Box V2", &Settings::Visuals::bboxv2);
			ImGui::Checkbox("Cornered Box", &Settings::Visuals::bBoxOutlined);
			ImGui::Checkbox("Snaplines", &Settings::Visuals::bSnaplines);
			ImGui::Checkbox("Distance", &Settings::Visuals::bDistance);
			
			ImGui::Checkbox("HP BAR", &Settings::Visuals::player_healthbar);
			ImGui::SliderFloat("Box Size", &bools::boxsize, 1, 10);
			ImGui::SliderFloat("Tracer Size", &bools::tracersize, 0, 10);
			
			ImGui::Checkbox("HeadCircle ESP", &Settings::Visuals::headcircle);

			if (Settings::Visuals::headcircle == true)
			{
				ImGui::Combo("HeadCircle Type", &Settings::Visuals::headboxtype, Settings::Visuals::headboxt, IM_ARRAYSIZE(Settings::Visuals::headboxt));
			}

			
		}

		ImGui::End();
	}
}



