#pragma once

#include <Windows.h>
#include <map>
#include <d3d9types.h>
#include "../Overlay/render.hpp"
#include "../Driver/driver.hpp"
#include "structs.hpp"
#include <vector>
#include "globals.hpp"
#include "../strings.hpp"

using namespace UE4Structs;

namespace Globals
{
	DWORD_PTR
		LocalPlayer,
		PlayerController,
		PlayerCameraManager;

	int MyUniqueID, MyTeamID, BoneCount;

	fvector MyRelativeLocation, closestPawn;

	namespace Camera
	{
		fvector CameraLocation, CameraRotation;
		Vector3 cameraLocation, cameraRotation;    //
		float FovAngle;
	}

	namespace camera
	{
		Vector3 cameraLocation, cameraRotation;
		float FovAngle;
	}
}

using namespace Globals;
using namespace Camera;



                           ////////////////////////vector3////////////////////

auto GetBone(DWORD_PTR mesh, int id) -> Vector3
{
	DWORD_PTR array = driver.read<uintptr_t>(mesh + 0x5c0);
	if (array == NULL)
		array = driver.read<uintptr_t>(mesh + 0x5c0);

	FTransform bone = driver.readv<FTransform>(array + (id * 0x30));

	FTransform ComponentToWorld = driver.readv<FTransform>(mesh + 0x250);
	D3DMATRIX Matrix;

	Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}


auto ProjectWorldToScreen(Vector3 WorldLocation) -> Vector3
{
	Vector3 Screenlocation = Vector3(0, 0, 0);

	cameraLocation = driver.readv<Vector3>(PlayerCameraManager + offsets::camera_position);
	cameraRotation = driver.readv<Vector3>(PlayerCameraManager + offsets::camera_rotation);
	float camera_fov = driver.read<float>(PlayerCameraManager + offsets::camera_fov);

	D3DMATRIX tempMatrix = Matrix(cameraRotation, Vector3(0, 0, 0));

	Vector3 vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]),
		vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]),
		vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	Vector3 vDelta = WorldLocation - cameraLocation;
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f) vTransformed.z = 1.f;

	FovAngle = camera_fov;

	float ScreenCenterX = Width / 2.0f;
	float ScreenCenterY = Height / 2.0f;

	Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
	Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

	return Screenlocation;
}


Vector3 VectorToRotation(Vector3 relativeLocation)
{
	constexpr auto radToUnrRot = 57.2957795f;

	return Vector3(
		atan2(relativeLocation.z, sqrt((relativeLocation.x * relativeLocation.x) + (relativeLocation.y * relativeLocation.y))) * radToUnrRot,
		atan2(relativeLocation.y, relativeLocation.x) * radToUnrRot,
		0.f);
}

Vector3 aimAtVector(Vector3 targetLocation, Vector3 cameraLocation)
{
	return VectorToRotation(targetLocation - cameraLocation);
}

Vector3 SmoothAngles(Vector3 rot1, Vector3 rot2, float smooth)
{
	Vector3 ret(0, 0, 0);

	ret.x = (rot2.x - rot1.x) / smooth + rot1.x;
	ret.y = (rot2.y - rot1.y) / smooth + rot1.y;
	ret.z = (rot2.z - rot1.z) / smooth + rot1.z;

	return ret;
}

void Aimbotty(Vector3 Target, Vector3 Camera_rotation, float SmoothFactor)
{
	Vector3 ConvertRotation = Camera_rotation;

	auto ControlRotation = driver.readv<Vector3>(PlayerController + 0x440);
	Vector3 DeltaRotation = ConvertRotation - ControlRotation;

	ConvertRotation = Target - (DeltaRotation * SmoothFactor);

	Vector3 Smoothed = SmoothAngles(Camera_rotation, ConvertRotation, SmoothFactor);

	driver.Write<Vector3>(PlayerController + 0x440, Smoothed);
}



auto CallAimbot(DWORD_PTR entity) -> void
{
	int hitboxid;
	Vector3 Head = GetBone(driver.read<uint64_t>(entity + 0x430), 8);
	Vector3 neck = GetBone(driver.read<uint64_t>(entity + 0x430), 7);
	Vector3 body = GetBone(driver.read<uint64_t>(entity + 0x430), 5);

	/*if (settings::aimbot::selectedhitbox == 2)
	{
		AimAtVector(body, cameraLocation);
	}*/

	Vector3 angles_out = aimAtVector(neck, cameraLocation);

#include "cheat.hpp"


	if (Settings::aimbot::selectedhitbox == 0)
	{
		Vector3 angles_out = aimAtVector(Head, cameraLocation);
	}
	else if (Settings::aimbot::selectedhitbox == 1)
	{
		Vector3 angles_out = aimAtVector(neck, cameraLocation);
	}
	else if (Settings::aimbot::selectedhitbox == 2)
	{
		Vector3 angles_out = aimAtVector(body, cameraLocation);
	}
	

	
	if (Settings::aimbot::smooth)
	{
		Aimbotty(angles_out, cameraRotation, Settings::aimbot::smoothness);
	}
	else driver.Write<Vector3>(PlayerController + 0x440, angles_out);



}



//////////////////////////////////////vector3ended here//////////////////////////



namespace UE4
{
	struct GWorld
	{
		uintptr_t GameInstance(uintptr_t GameWorld) {
			return driver.readv<uintptr_t>(GameWorld + offsets::Gameinstance);
		};

		uintptr_t ULevel(uintptr_t World) {
			return driver.read<uintptr_t>(World + offsets::Ulevel);
		};
	};

	struct GGameInstance {
		uintptr_t ULocalPlayer(uintptr_t UGameInstance) {
			auto ULocalPlayerArray = driver.readv<uintptr_t>(UGameInstance + offsets::LocalPlayers);
			return driver.readv<uintptr_t>(ULocalPlayerArray);
		};
	};

	struct GULevel {
		TArrayDrink<uintptr_t> AActorArray(uintptr_t ULevel) {
			return driver.readv<TArrayDrink<uintptr_t>>(ULevel + offsets::AActorArray);
		};
	};

	struct GPrivatePawn {
		uintptr_t USkeletalMeshComponent(uintptr_t Pawn) {
			return driver.read<uintptr_t>(Pawn + offsets::MeshComponent);
		};
	};

	struct GUSkeletalMeshComponent {
		int BoneCount(uintptr_t Mesh) {
			return driver.read<uintptr_t>(Mesh + offsets::BoneCount);
		};
	};

	struct GLocalPlayer {
		uintptr_t APlayerController(uintptr_t ULocalPlayer) {
			return driver.read<uintptr_t>(ULocalPlayer + offsets::PlayerController);
		};
	};

	struct GPlayerController {
		uintptr_t APlayerCameraManager(uintptr_t APlayerController) {
			return driver.read<uintptr_t>(APlayerController + offsets::PlayerCameraManager);
		};
		uintptr_t AHUD(uintptr_t APlayerController) {
			return driver.read<uintptr_t>(APlayerController + offsets::MyHUD);
		};
		uintptr_t APawn(uintptr_t APlayerController) {
			return driver.read<uintptr_t>(APlayerController + offsets::AcknowledgedPawn);
		};
	};

	struct GPawn {
		auto TeamID(uintptr_t APawn) -> int {
			auto PlayerState = driver.readv<uintptr_t>(APawn + offsets::PlayerState);
			auto TeamComponent = driver.readv<uintptr_t>(PlayerState + offsets::TeamComponent);
			return driver.readv<int>(TeamComponent + offsets::TeamID);
		};

		auto UniqueID(uintptr_t APawn) -> int {
			return driver.readv<int>(APawn + offsets::UniqueID);
		};

		auto FNameID(uintptr_t APawn) -> int {
			return driver.readv<int>(APawn + offsets::FNameID);
		};

		auto RelativeLocation(uintptr_t APawn) -> fvector {
			auto RootComponent = driver.readv<uintptr_t>(APawn + offsets::RootComponent);
			return driver.readv<fvector>(RootComponent + offsets::RelativeLocation);
		};

		auto bIsDormant(uintptr_t APawn) -> bool {
			return driver.readv<bool>(APawn + offsets::bIsDormant);
		};

		auto Health(uintptr_t APawn) -> float {
			auto DamageHandler = driver.read<uintptr_t>(APawn + offsets::DamageHandler);
			return driver.readv<float>(DamageHandler + offsets::Health);
		};
	};

	auto GetWorld(uintptr_t Pointer) -> uintptr_t
	{
		std::uintptr_t uworld_addr = driver.readv<uintptr_t>(Pointer + 0x60);

		unsigned long long uworld_offset;

		if (uworld_addr > 0x10000000000)
		{
			uworld_offset = uworld_addr - 0x10000000000;
		}
		else {
			uworld_offset = uworld_addr - 0x8000000000;
		}

		return Pointer + uworld_offset;
	}



	auto VectorToRotation(fvector relativeLocation) -> fvector
	{
		constexpr auto radToUnrRot = 57.2957795f;

		return fvector(
			atan2(relativeLocation.z, sqrt((relativeLocation.x * relativeLocation.x) + (relativeLocation.y * relativeLocation.y))) * radToUnrRot,
			atan2(relativeLocation.y, relativeLocation.x) * radToUnrRot,
			0.f);
	}
	
	auto isVisible(DWORD_PTR mesh) -> bool
	{
		float fLastSubmitTime = driver.readv<float>(mesh + offsets::last_submit_time);
		float fLastRenderTimeOnScreen = driver.readv<float>(mesh + offsets::last_render_time);

		const float fVisionTick = 0.06f;
		bool bVisible = fLastRenderTimeOnScreen + fVisionTick >= fLastSubmitTime;

		return bVisible;
	}



	auto AimAtVector(fvector targetLocation, fvector cameraLocation) -> fvector
	{
		return VectorToRotation(targetLocation - cameraLocation);
	}

	namespace SDK
	{
		using namespace math;
		auto GetEntityBone(DWORD_PTR mesh, int id) -> fvector
		{
			DWORD_PTR array = driver.readv<uintptr_t>(mesh + offsets::BoneArray);
			if (array == NULL)
				array = driver.readv<uintptr_t>(mesh + offsets::BoneArrayCache);

			ftransform bone = driver.readv<ftransform>(array + (id * 0x30));

			ftransform ComponentToWorld = driver.readv<ftransform>(mesh + offsets::ComponentToWorld);
			D3DMATRIX Matrix;

			Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

			return fvector(Matrix._41, Matrix._42, Matrix._43);
		}

		
		///////////////////////////////////////////////////////////////////////////////////////////

	
		auto w2s(fvector WorldLocation) -> fvector
		{
			fvector Screenlocation = fvector(0, 0, 0);

			auto ViewInfo = driver.readv<FMinimalViewInfo>(PlayerCameraManager + 0x1fe0 + 0x10);

			CameraLocation = ViewInfo.Location;
			CameraRotation = ViewInfo.Rotation;


			D3DMATRIX tempMatrix = matrix(CameraRotation, fvector(0, 0, 0));

				fvector vAxisX = fvector(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]),
				vAxisY = fvector(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]),
				vAxisZ = fvector(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

			fvector vDelta = WorldLocation - CameraLocation;
			fvector vTransformed = fvector(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

			if (vTransformed.z < 1.f) vTransformed.z = 1.f;

			FovAngle = ViewInfo.FOV;

			float ScreenCenterX = Width / 2.0f;
			float ScreenCenterY = Height / 2.0f;

			Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
			Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

			return Screenlocation;
		}

		bool IsVec3Valid(fvector vec3)
		{
			return !(vec3.x == 0 && vec3.y == 0 && vec3.z == 0);
		}
	}
}