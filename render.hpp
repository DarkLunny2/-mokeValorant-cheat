#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>
#include <d3dx9tex.h>


#include <dwmapi.h>

#include "../../Includes/Imgui/imgui_internal.h"
#include "../../Includes/Imgui/imgui.h"
#include "../../Includes/Imgui/imgui_impl_win32.h"
#include "../../Includes/Imgui/imgui_impl_dx9.h"
#include "../game/globals.hpp"
#include "../Game/structs.hpp"
#include "menu.hpp"

#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "Dwmapi.lib")

IDirect3D9Ex* p_Object = NULL;
IDirect3DDevice9Ex* p_Device = NULL;
D3DPRESENT_PARAMETERS p_Params = { NULL };

HWND MyWnd = NULL;
HWND GameWnd = NULL;
MSG Message = { NULL };

RECT GameRect = { NULL };
D3DPRESENT_PARAMETERS d3dpp;

DWORD ScreenCenterX;
DWORD ScreenCenterY;

static ULONG Width = GetSystemMetrics(SM_CXSCREEN);
static ULONG Height = GetSystemMetrics(SM_CYSCREEN);

WPARAM main_loop();
void render();

void DefaultTheme() {
//thanks https://www.unknowncheats.me/forum/members/4566270.html	
	
	ImGuiStyle& s = ImGui::GetStyle();


	s.AntiAliasedFill = true;
	s.AntiAliasedLines = true;

	s.ChildRounding = 0.0f;
	s.FrameBorderSize = 1.0f;
	s.FrameRounding = 0.0f;
	s.PopupRounding = 0.0f;
	s.ScrollbarRounding = 0.0f;
	s.ScrollbarSize = 0.0f;
	s.TabRounding = 0.0f;
	s.WindowRounding = 0.0f;

}

auto init_wndparams(HWND hWnd) -> HRESULT
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(3);

	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = hWnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = Width;
	p_Params.BackBufferHeight = Height;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;
	p_Params.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device))) {
		p_Object->Release();
		exit(4);
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGuiStyle& s = ImGui::GetStyle();
	io.IniFilename = NULL;

	DefaultTheme();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(p_Device);
	return S_OK;
}
/////////////////////////////


float RadianToDegree(float radian)
{
	return radian * (180 / M_PI);
}

float DegreeToRadian(float degree)
{
	return degree * (M_PI / 180);

}

class FVector2 {
public:
	FVector2() : x(0.f), y(0.f), z(0.f) {}
	FVector2(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	~FVector2() {}

	float x;
	float y;
	float z;

	inline float Dot(FVector2 v) {
		return x * v.x + y * v.y + z * v.z;
	}

	inline float Distance(FVector2 v) {
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	FVector2 operator+(FVector2 v) {
		return FVector2(x + v.x, y + v.y, z + v.z);
	}

	FVector2 operator-(FVector2 v) {
		return FVector2(x - v.x, y - v.y, z - v.z);
	}
	FVector2 operator/(float flDiv) {
		return FVector2(x / flDiv, y / flDiv, z / flDiv);
	}

	FVector2 operator*(float Scale) {

		return FVector2(x * Scale, y * Scale, z * Scale);
	}
	inline float Length() {
		return sqrtf((x * x) + (y * y) + (z * z));
	}
	FVector2 operator-=(FVector2 v) {

		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
};

struct FVector2D {
public:
	float x;
	float y;

	inline FVector2D() : x(0), y(0) {}
	inline FVector2D(float x, float y) : x(x), y(y) {}

	inline float Distance(FVector2D v) {
		return sqrtf(((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y)));
	}

	inline FVector2D operator+(const FVector2D& v) const {
		return FVector2D(x + v.x, y + v.y);
	}

	inline FVector2D operator-(const FVector2D& v) const {
		return FVector2D(x - v.x, y - v.y);
	}
};

FVector2 RadianToDegree(FVector2 radians)
{
	FVector2 degrees;
	degrees.x = radians.x * (180 / M_PI);
	degrees.y = radians.y * (180 / M_PI);
	degrees.z = radians.z * (180 / M_PI);
	return degrees;
}

FVector2 DegreeToRadian(FVector2 degrees)
{
	FVector2 radians;
	radians.x = degrees.x * (M_PI / 180);
	radians.y = degrees.y * (M_PI / 180);
	radians.z = degrees.z * (M_PI / 180);
	return radians;
}

typedef struct {
	uintptr_t actor_ptr;
	uintptr_t damage_handler_ptr;
	uintptr_t player_state_ptr;
	uintptr_t root_component_ptr;
	uintptr_t mesh_ptr;
	uintptr_t bone_array_ptr;
	int bone_count;
} EnemyPtr;



//////////////////////
/////////////////////
auto get_process_wnd(uint32_t pid) -> HWND
{
	std::pair<HWND, uint32_t> params = { 0, pid };
	BOOL bResult = EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
		auto pParams = (std::pair<HWND, uint32_t>*)(lParam);
		uint32_t processId = 0;

		if (GetWindowThreadProcessId(hwnd, reinterpret_cast<LPDWORD>(&processId)) && processId == pParams->second) {
			SetLastError((uint32_t)-1);
			pParams->first = hwnd;
			return FALSE;
		}

		return TRUE;

		}, (LPARAM)&params);

	if (!bResult && GetLastError() == -1 && params.first)
		return params.first;

	return NULL;
}

auto cleanup_d3d() -> void
{
	if (p_Device != NULL) {
		p_Device->EndScene();
		p_Device->Release();
	}
	if (p_Object != NULL) {
		p_Object->Release();
	}
}

auto set_window_target() -> void
{
	while (true) {
		GameWnd = get_process_wnd(processid);
		if (GameWnd) {
			ZeroMemory(&GameRect, sizeof(GameRect));
			GetWindowRect(GameWnd, &GameRect);
			DWORD dwStyle = GetWindowLong(GameWnd, GWL_STYLE);
			if (dwStyle & WS_BORDER)
			{
				GameRect.top += 32;
				Height -= 39;
			}
			ScreenCenterX = Width / 2;
			ScreenCenterY = Height / 2;
			MoveWindow(MyWnd, GameRect.left, GameRect.top, Width, Height, true);
		}
	}
}

auto setup_window() -> void 
{
	HANDLE Windowthread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)set_window_target, 0, 0, 0);
	CloseHandle(Windowthread);
	
	WNDCLASSEXA wcex = {
		sizeof(WNDCLASSEXA),
		0,
		DefWindowProcA,
		0,
		0,
		nullptr,
		LoadIcon(nullptr, IDI_APPLICATION),
		LoadCursor(nullptr, IDC_ARROW),
		nullptr,
		nullptr,
		("MirakaKindaCuteNgl"),
		LoadIcon(nullptr, IDI_APPLICATION)
	};

	RECT Rect;
	GetWindowRect(GetDesktopWindow(), &Rect);

	RegisterClassExA(&wcex);

	MyWnd = CreateWindowExA(NULL, ("MirakaKindaCuteNgl"), ("MirakaKindaCuteNgl"), WS_POPUP, Rect.left, Rect.top, Rect.right, Rect.bottom, NULL, NULL, wcex.hInstance, NULL);
	SetWindowLong(MyWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
	SetLayeredWindowAttributes(MyWnd, RGB(0, 0, 0), 255, LWA_ALPHA);

	MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(MyWnd, &margin);

	ShowWindow(MyWnd, SW_SHOW);
	UpdateWindow(MyWnd);
}

using namespace ColorStructs;
////////////////////////////////
auto DrawLine(const ImVec2& x, const ImVec2 y, ImU32 color, const FLOAT width) -> void
{
	ImGui::GetForegroundDrawList()->AddLine(x, y, color, width);
}

void DrawLine1(int x1, int y1, int x2, int y2, RGBA* color, int thickness)
{
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), thickness);
}
void DrawCircleFilled(int x, int y, int radius, RGBA* color, float segments)
{
	ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 153.0, color->B / 51.0, color->A / 255.0)), segments);
}


void DrawCircle(int x, int y, int radius, RGBA* color, float segments)
{
	ImGui::GetForegroundDrawList()->AddCircle(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 153.0, color->B / 51.0, color->A / 255.0)), segments);
}

void DrawCircleFilled1(int x, int y, int radius, RGBA* color)
{
	ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
}

static int enemy_size = 3;
void DrawCircleFilled2(int x, int y, int radius, RGBA* color)
{
	ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
}



void DrawFilledRect(int x, int y, int w, int h, RGBA* color)
{
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 153.0, color->B / 51.0, color->A / 255.0)), 0, 0);
}

void DrawFilledRect2(int x, int y, int w, int h, ImColor color)
{
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0);
}

void DrawCornerBox(float x, float y, float w, float h, const ImColor& color)
{
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y), ImVec2(x + w / 4.f, y), color);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y), ImVec2(x, y + h / 4.f), color);

	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w, y), ImVec2(x + w - w / 4.f, y), color);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w, y), ImVec2(x + w, y + h / 4.f), color);

	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y + h), ImVec2(x + w / 4.f, y + h), color);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y + h), ImVec2(x, y + h - h / 4.f), color);

	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w, y + h), ImVec2(x + w, y + h - h / 4.f), color);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w, y + h), ImVec2(x + w - w / 4.f, y + h), color);
}

void DrawNormalBox(int x, int y, int w, int h, int borderPx, ImColor color)
{
	DrawFilledRect2(x + borderPx, y, w, borderPx, color);
	DrawFilledRect2(x + w - w + borderPx, y, w, borderPx, color);
	DrawFilledRect2(x, y, borderPx, h, color);
	DrawFilledRect2(x, y + h - h + borderPx * 2, borderPx, h, color);
	DrawFilledRect2(x + borderPx, y + h + borderPx, w, borderPx, color);
	DrawFilledRect2(x + w - w + borderPx, y + h + borderPx, w, borderPx, color);
	DrawFilledRect2(x + w + borderPx, y, borderPx, h, color);
	DrawFilledRect2(x + w + borderPx, y + h - h + borderPx * 2, borderPx, h, color);
}



using namespace UE4Structs;

auto Draw2DBox(fvector RootPosition, float Width, float Height, ImColor Color) -> void
{
	DrawNormalBox(RootPosition.x - Width / 2, RootPosition.y - Height / 2, Width, Height, bools::boxsize, Color);
}

void DrawRect(int x, int y, int w, int h, RGBA* color, int thickness)
{
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), 0, 0, thickness);
}

auto DrawOutlinedBox(fvector RootPosition, float Width, float Height, ImColor Color) -> void
{
	DrawCornerBox(RootPosition.x - Width / 2, RootPosition.y - Height / 2, Width, Height, Color);
	DrawCornerBox(RootPosition.x - Width / 2 - 1, RootPosition.y - Height / 2 - 1, Width, Height, ImColor(0, 0, 0));
	DrawCornerBox(RootPosition.x - Width / 2 + 1, RootPosition.y - Height / 2 + 1, Width, Height, ImColor(0, 0, 0));
}

auto DrawDistance(fvector Location, float Distance) -> void
{
	char dist[64];
	sprintf_s(dist, "%.fm", Distance);

	ImVec2 TextSize = ImGui::CalcTextSize(dist);
	ImGui::GetForegroundDrawList()->AddText(ImVec2(Location.x - TextSize.x / 2, Location.y - TextSize.y / 2), ImGui::GetColorU32({ 255, 255, 255, 255 }), dist);
}

auto DrawTracers(fvector Target, ImColor Color) -> void
{
	ImGui::GetForegroundDrawList()->AddLine(
		ImVec2(ScreenCenterX, Height),
		ImVec2(Target.x, Target.y),
		Color,
		bools::tracersize
	);
}

auto DrawHealthBar(fvector RootPosition, float Width, float Height, float Health, float RelativeDistance)-> void
{
	auto HPBoxWidth = 1 / RelativeDistance;

	auto HPBox_X = RootPosition.x - Width / 2 - 5 - HPBoxWidth;
	auto HPBox_Y = RootPosition.y - Height / 2 + (Height - Height * (Health / 100));

	int HPBoxHeight = Height * (Health / 100);

	DrawFilledRect(HPBox_X, HPBox_Y, HPBoxWidth, HPBoxHeight, &ColorStructs::Col.green);
	DrawRect(HPBox_X - 1, HPBox_Y - 1, HPBoxWidth + 2, HPBoxHeight + 2, &ColorStructs::Col.black, 1);
}

static FVector2 pRadar;

void DrawBorder(float x, float y, float w, float h, float px, RGBA* BorderColor)
{
	DrawRect(x, (y + h - px), w, px, BorderColor, 1 / 2);
	DrawRect(x, y, px, h, BorderColor, 1 / 2);
	DrawRect(x, y, w, px, BorderColor, 1 / 2);
	DrawRect((x + w - px), y, px, h, BorderColor, 1 / 2);
}



inline void renderBox(fvector head_at_screen, float distance_modifier) {
	int head_x = head_at_screen.x;
	int head_y = head_at_screen.y;
	int start_x = head_x - 45 / distance_modifier;
	int start_y = head_y - 25 / distance_modifier;
	int end_x = head_x + 45 / distance_modifier;
	int end_y = head_y + 165 / distance_modifier;
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(start_x, start_y), ImVec2(end_x, end_y), ImColor(0, 0, 0));
}



