#pragma once
#include "sdk.hpp"
#include <iostream>
#include "globals.hpp"
#include "structs.hpp"
#include "check.h" 
#include "../Driver/driver.hpp"




using namespace Globals;
using namespace Camera;
using namespace UE4;

static std::vector<EnemyPtr> enemy_collection;

typedef struct _FNlEntity {
	uint64_t Actor;
	uint64_t MeshComponent;
	uintptr_t root_component;
}FNlEntity;
std::vector<FNlEntity> entityList;

int LocalUID;
DWORD_PTR PlayerState, RootComponent, LocalPawn;

bool aimbot;
float aimbotfov = 10.0f;
bool circle;

GWorld* UWorld;
GGameInstance* UGameInstance;
GLocalPlayer* ULocalPlayer;
GPlayerController* APlayerController;
GPawn* APawn;
GPrivatePawn* APrivatePawn;
GULevel* ULevel;
GUSkeletalMeshComponent* USkeletalMeshComponent;

bool cached = false;
uintptr_t WorldPtr ;


//using namespace UE4Structs;

//ue4 ftransform class
namespace guarded_pointers
{
	uintptr_t guard_apawn = 0x0;
	uintptr_t guard_controller = 0x0;
}

struct player
{
	uintptr_t for_actor;
	uintptr_t for_mesh;

	uintptr_t actor;
	uintptr_t mesh;
	uintptr_t bone_array;
	uintptr_t root_component;
	uintptr_t damage_handler;

	INT32 bone_count;
	INT32 ammo_count;

	std::string weapon_name;
	std::string agent_name;
	std::string player_name;

	float distance;
	float health;
	float shield;

	bool is_valid;
	bool is_damage_handler_guarded;
	bool is_mesh_guarded;
};

boolean in_rect(double centerX, double centerY, double radius, double x, double y) {
	return x >= centerX - radius && x <= centerX + radius &&
		y >= centerY - radius && y <= centerY + radius;
}
///////////////////added
inline bool operator==(const player& a, const player& b) {
	if (a.actor == b.actor)
		return true;

	return false;
}

int center_x = GetSystemMetrics(0) / 2 - 3;
int center_y = GetSystemMetrics(1) / 2 - 3;


auto CacheGame() -> void
{
	auto guardedregion = driver.guarded_region();

	while (true)
	{

		///////////
	
		///////////

		std::vector<ValEntity> CachedList;

		WorldPtr = GetWorld(guardedregion);

		auto ULevelPtr = UWorld->ULevel(WorldPtr);
		auto UGameInstancePtr = UWorld->GameInstance(WorldPtr);

		auto ULocalPlayerPtr = UGameInstance->ULocalPlayer(UGameInstancePtr);
		auto APlayerControllerPtr = ULocalPlayer->APlayerController(ULocalPlayerPtr);

		PlayerCameraManager = APlayerController->APlayerCameraManager(APlayerControllerPtr);
		auto MyHUD = APlayerController->AHUD(APlayerControllerPtr);

		auto APawnPtr = APlayerController->APawn(APlayerControllerPtr);
		/////////////////////////added from here









		if (APawnPtr != 0)
		{
			MyUniqueID = APawn->UniqueID(APawnPtr);
			MyRelativeLocation = APawn->RelativeLocation(APawnPtr);
		}

		if (MyHUD != 0)
		{
			auto PlayerArray = ULevel->AActorArray(ULevelPtr);

			for (uint32_t i = 0; i < PlayerArray.Count; ++i)
			{
				auto Pawns = PlayerArray[i];
				if (Pawns != APawnPtr)
				{
					if (MyUniqueID == APawn->UniqueID(Pawns))
					{
						ValEntity Entities{ Pawns };
						CachedList.push_back(Entities);
					}
				}
			}

			ValList.clear();
			ValList = CachedList;
			Sleep(1000);
			std::vector<FNlEntity> tmpList;

		}
	}
}

std::vector<player> player_pawns;



namespace Kamera
{
	fvector location;
	fvector rotation;
	float fov;
}

namespace pointer
{
	uintptr_t apawn;
	uintptr_t apawn_old;
	uintptr_t player_controller;
	uintptr_t camera_manager;
}
  

std::string GetCharacterName(int id)
{

	switch (id)
	{
	case 12814502: return "ASTRA";
		break;
	case 14230510: return "JETT";
		break;
	case 14211583: return "CHAMBER";
		break;
	case 14219028: return "KILLJOY";
		break;
	case 14209948: return "RAZE"; //ok
		break;
	case 14228884: return "REYNA"; //ok
		break;
	case 14228027: return "SAGE"; //ok
		break;
	case 12807475: return "SKYE";
		break;
	case 14225108: return "NEON";
		break;
	case 14220733: return "VIPER";
		break;
	case 12801214: return "BREACH";
		break;
	case 14224190: return "BRIMSTONE";
		break;
	case 14216462: return "CYPHER";
		break;
	case 14229275: return "OMEN";
		break;
	case 14221852: return "PHOENIX"; //ok
		break;
	case 14218064: return "SOVA"; //ok
		break;
	case 12818495: return "YORU";
		break;
	case 12806381: return "KAY/O";
		break;
	case 14207406: return "FADE";
		break;
	case 14228837: return  "BOT"; //ok
		break;
	case 14228831: return "Train BOT"; //ok
		break;
	default:
		return std::to_string(id);
		break;
	}
}





static std::string utf8_encode(const std::wstring& wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}




void DrawLineSimple(Vector3 first_bone_position, Vector3 second_bone_position) {

	auto first_bone_screen_position = ProjectWorldToScreen(first_bone_position);
	if (first_bone_screen_position.x == 0 && first_bone_screen_position.y == 0) return;

	auto second_bone_screen_position = ProjectWorldToScreen(second_bone_position);
	if (second_bone_screen_position.x == 0 && second_bone_screen_position.y == 0) return;

	ImColor White = { 68, 255, 0, 255 };

	ImGui::GetForegroundDrawList()->AddLine(ImVec2(first_bone_screen_position.x, first_bone_screen_position.y), ImVec2(second_bone_screen_position.x, second_bone_screen_position.y), White, 0.5f);
}




//converter
float degree_to_radian(float degree)
{
	return degree * (M_PI / 180);
}


//function to calculate an angle
void angle_rotation(const fvector& angles, fvector* forward)
{
	float	sp, sy, cp, cy;

	sy = sin(degree_to_radian(angles.y));
	cy = cos(degree_to_radian(angles.y));

	sp = sin(degree_to_radian(angles.x));
	cp = cos(degree_to_radian(angles.x));

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}


//start from bottom (x, y), to top (w, h)
void draw_health_bar(ImVec2 min, ImVec2 max, float health)
{
	float health_percentage = health;
	health_percentage *= 0.01f;

	float lenght_left_to_right = max.x - min.x;
	lenght_left_to_right *= health_percentage;

	float healthbar_size_y = 5.f;

	float g = health_percentage * 255.f;
	float r = 255.f - g;
	float b = 0.f;

	r /= 255.f;
	g /= 255.f;
	b /= 255.f;

	//background
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(min.x, min.y - healthbar_size_y), ImVec2(max.x, max.y), ImGui::GetColorU32({ 0.1f, 0.1f, 0.1f, 1.f }), 0.f, 15);

	//health bar
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(min.x, min.y - healthbar_size_y), ImVec2(min.x + lenght_left_to_right, max.y), ImGui::GetColorU32({ r, g, b, 1.f }), 0.f, 15);

	//outline
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(min.x, min.y - healthbar_size_y), ImVec2(max.x, max.y), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), 0.f, 15, 1.f);
}


using namespace SDK;
void draw_3d_box(fvector base, fvector top_reach, float wide, ImVec4 col, float thickness)
{

	//calculate bottom rect
	fvector bottom_rect_1 = fvector(base.x + wide, base.y + wide, base.z);
	fvector bottom_rect_2 = fvector(base.x + wide, base.y - wide, base.z);
	fvector bottom_rect_3 = fvector(base.x - wide, base.y + wide, base.z);
	fvector bottom_rect_4 = fvector(base.x - wide, base.y - wide, base.z);

	//calculate top rect
	fvector top_rect_1 = fvector(top_reach.x + wide, top_reach.y + wide, top_reach.z);
	fvector top_rect_2 = fvector(top_reach.x + wide, top_reach.y - wide, top_reach.z);
	fvector top_rect_3 = fvector(top_reach.x - wide, top_reach.y + wide, top_reach.z);
	fvector top_rect_4 = fvector(top_reach.x - wide, top_reach.y - wide, top_reach.z);

	//w2s bottom rect
	bottom_rect_1 = w2s(bottom_rect_1);
	bottom_rect_2 = w2s(bottom_rect_2);
	bottom_rect_3 = w2s(bottom_rect_3);
	bottom_rect_4 = w2s(bottom_rect_4);

	//w2s top rect
	top_rect_1 = w2s(top_rect_1);
	top_rect_2 = w2s(top_rect_2);
	top_rect_3 = w2s(top_rect_3);
	top_rect_4 = w2s(top_rect_4);

	//render bottom rect
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(bottom_rect_1.x, bottom_rect_1.y), ImVec2(bottom_rect_2.x, bottom_rect_2.y), ImGui::GetColorU32({ col.x, col.y, col.z, col.w }), thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(bottom_rect_2.x, bottom_rect_2.y), ImVec2(bottom_rect_4.x, bottom_rect_4.y), ImGui::GetColorU32({ col.x, col.y, col.z, col.w }), thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(bottom_rect_4.x, bottom_rect_4.y), ImVec2(bottom_rect_3.x, bottom_rect_3.y), ImGui::GetColorU32({ col.x, col.y, col.z, col.w }), thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(bottom_rect_3.x, bottom_rect_3.y), ImVec2(bottom_rect_1.x, bottom_rect_1.y), ImGui::GetColorU32({ col.x, col.y, col.z, col.w }), thickness);

	//render top rect
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(top_rect_1.x, top_rect_1.y), ImVec2(top_rect_2.x, top_rect_2.y), ImGui::GetColorU32({ col.x, col.y, col.z, col.w }), thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(top_rect_2.x, top_rect_2.y), ImVec2(top_rect_4.x, top_rect_4.y), ImGui::GetColorU32({ col.x, col.y, col.z, col.w }), thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(top_rect_4.x, top_rect_4.y), ImVec2(top_rect_3.x, top_rect_3.y), ImGui::GetColorU32({ col.x, col.y, col.z, col.w }), thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(top_rect_3.x, top_rect_3.y), ImVec2(top_rect_1.x, top_rect_1.y), ImGui::GetColorU32({ col.x, col.y, col.z, col.w }), thickness);

	//render connection lines
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(bottom_rect_1.x, bottom_rect_1.y), ImVec2(top_rect_1.x, top_rect_1.y), ImGui::GetColorU32({ col.x, col.y, col.z, col.w }), thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(bottom_rect_2.x, bottom_rect_2.y), ImVec2(top_rect_2.x, top_rect_2.y), ImGui::GetColorU32({ col.x, col.y, col.z, col.w }), thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(bottom_rect_3.x, bottom_rect_3.y), ImVec2(top_rect_3.x, top_rect_3.y), ImGui::GetColorU32({ col.x, col.y, col.z, col.w }), thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(bottom_rect_4.x, bottom_rect_4.y), ImVec2(top_rect_4.x, top_rect_4.y), ImGui::GetColorU32({ col.x, col.y, col.z, col.w }), thickness);

	

}






void update_camera()
{
	Kamera::location = driver.readv<fvector>(pointer::camera_manager + 0x1260);
	Kamera::rotation = driver.readv<fvector>(pointer::camera_manager + 0x126C);
	Kamera::fov = driver.readv<float>(pointer::camera_manager + 0x1278);
}





auto CheatLoop() -> void
{
	

	for (ValEntity ValEntityList : ValList)
	{   
	
		////////////////////////////
		float closestDistance = FLT_MAX;
		DWORD_PTR closestPawn = NULL;
		//////////////////////////////
	
		auto SkeletalMesh = APrivatePawn->USkeletalMeshComponent(ValEntityList.Actor);

		auto RelativeLocation = APawn->RelativeLocation(ValEntityList.Actor);
		auto RelativeLocationProjected = UE4::SDK::w2s(RelativeLocation);

		auto RelativePosition = RelativeLocation - CameraLocation;
		auto RelativeDistance = RelativePosition.lenght() / 10000 * 2;

	//	auto HeadBone = UE4::SDK::bone_matrix(SkeletalMesh, 8);
		auto HeadBone = UE4::SDK::GetEntityBone(SkeletalMesh, 8);
		auto HeadBoneProjected = UE4::SDK::w2s(HeadBone);

		//auto RootBone = UE4::SDK::bone_matrix(SkeletalMesh, 0);
		auto RootBone = UE4::SDK::GetEntityBone(SkeletalMesh, 0);
		auto RootBoneProjected = UE4::SDK::w2s(RootBone);
		auto RootBoneProjected2 = UE4::SDK::w2s(fvector(RootBone.x, RootBone.y, RootBone.z - 15));

		auto Distance = MyRelativeLocation.distance(RelativeLocation);

		float BoxHeight = abs(HeadBoneProjected.y - RootBoneProjected.y);
		float BoxWidth = BoxHeight * 0.40;




		auto ESPColor = ImColor(255, 0, 255);
		

		auto Health = APawn->Health(ValEntityList.Actor);
		if (Health <= 0) continue;
				
		fvector zero = UE4::SDK::GetEntityBone(SkeletalMesh, 0);
		fvector head = UE4::SDK::GetEntityBone(SkeletalMesh, 8);

		

		fvector zero_r = fvector(zero.x, zero.y, zero.z - 5);
		fvector head_r = fvector(head.x, head.y, head.z + 20);
		fvector head_r_2 = fvector(head.x, head.y, head.z + 30);

		fvector zero_w2s = w2s(zero);
		fvector head_w2s = w2s(head);

		fvector zero_w2s_r = w2s(zero_r);
		fvector head_w2s_r = w2s(head_r);

		fvector head_w2s_r_2 = w2s(head_r_2);

	

		if (APawn->bIsDormant(ValEntityList.Actor))
		{
			if (Settings::Visuals::bSnaplines)
				DrawTracers(RootBoneProjected, ESPColor);

			if (Settings::Visuals::bBox)
				Draw2DBox(RelativeLocationProjected, BoxWidth, BoxHeight, ESPColor);

			if (Settings::Visuals::bBoxOutlined)
				DrawOutlinedBox(RelativeLocationProjected, BoxWidth, BoxHeight, ESPColor);

			
			if (Settings::Visuals::bDistance)
				DrawDistance(RootBoneProjected2, Distance);

			if (Settings::Visuals::bboxv2)
				renderBox(RelativeLocationProjected, RelativeDistance);

		
			if (Settings::Visuals::player_box_3d)
			{
				draw_3d_box(zero, fvector(head.x, head.y, head.z + 20), 43, ESPColor, 2.f);
			}

			
			if (Settings::aimbot::drawfov)
			{
				DrawCircle(ScreenCenterX, ScreenCenterY, Settings::aimbot::fov, &Col.white, 10000);
			}

			



			if (Settings::Visuals::headcircle)
			{
				if (Settings::Visuals::headboxtype == 0)
				{
					DrawCircle(zero_w2s.x, head_w2s.y, BoxHeight / 10, &Col.white, 100);
				}

				else if (Settings::Visuals::headboxtype == 1)
				{
					DrawCircle(zero_w2s.x, head_w2s.y, BoxHeight / 10, &Col.white, 100);
					DrawCircleFilled(zero_w2s.x, head_w2s.y, BoxHeight / 10, &Col.filled, 100);
				}

				else if (Settings::Visuals::headboxtype == 2)

				{
					DrawCircleFilled(zero_w2s.x, head_w2s.y, BoxHeight / 20, &Col.white, 100);
					DrawCircle(zero_w2s.x, head_w2s.y, BoxHeight / 40, &Col.white, 100);
				}

				//DrawCircle(RootBone.x, HeadBone.y, BoxHeight / 10, &Col.white, 100);
			}

			if (Settings::Visuals::player_healthbar)
			{
				float BoxHeight = zero_w2s_r.y - head_w2s_r.y;
				float BoxWidth = BoxHeight / 2.f;

				float x1 = zero_w2s_r.x - (BoxWidth / 2);
				float x2 = zero_w2s_r.x + (BoxWidth / 2);

				draw_health_bar(ImVec2(x1, head_w2s_r_2.y), ImVec2(x2, head_w2s_r_2.y), Health);
			}


			auto entity = driver.read<uintptr_t>(SkeletalMesh + offsets::BoneCount);

			auto dx = HeadBoneProjected.x - (Width / 2);
			auto dy = HeadBoneProjected.y - (Height / 2);
			auto dist = sqrtf(dx * dx + dy * dy);


			if (dist < Settings::aimbot::fov && dist < closestDistance)
			{
				if (Settings::aimbot::vischeck)
				{
					if (isVisible)
					{
						closestDistance = dist;
						closestPawn = entity;
					}
				}
				else
				{
					closestDistance = dist;
					closestPawn = entity;
				}
			}
			int fov = 103;
			if (Settings::aimbot::drawfov)
			{
				DrawCircle(ScreenCenterX, ScreenCenterY, Settings::aimbot::fov, &Col.white, 10000);
			}

			if (Settings::aimbot::aimbot)
			{
				if (closestPawn && GetAsyncKeyState(hotkeys::aimkey) < 0)
				{
					CallAimbot(closestPawn);
				}
			}
			
			
			
						
		}		
	


	}
				
}



