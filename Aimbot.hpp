#pragma once

#include "Classes.hpp"
#include "includes.hpp"
#include "Settings.hpp"
#include "native.hpp"

namespace Aimbot
{

	uintptr_t GetDistancePlayer()
	{
		uintptr_t ClosestPlayer = 0;
		float closestdist = FLT_MAX;
		float dist;
		hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
		if (!World)
			return NULL;

		hk_Ped* LocalPlayer = World->LocalPlayer();
		if (!LocalPlayer)
			return NULL;

		hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
		if (!ReplayInterface)
			return NULL;

		hk_PedInterface* PedInterface = ReplayInterface->PedInterface();
		if (!PedInterface)
			return NULL;

		for (int index = 0; index < PedInterface->PedMaximum(); index++)
		{
			hk_Ped* Peds = PedInterface->PedList()->Ped(index);
			if (!Peds) continue;

	
			if (Settings::Aimbot::TypeOfTarget == 1)
			{

				auto ped_type = Peds->GetPedType();
				if (!ped_type) continue;
				ped_type = ped_type << 11 >> 25;


				if (ped_type != 2) {
					continue;
				}
			}
			if (Settings::Aimbot::TypeOfTarget == 2)
			{
				auto ped_type = Peds->GetPedType();
				if (!ped_type) continue;

				ped_type = ped_type << 11 >> 25;


				if (ped_type == 2) {
					continue;
				}
			}

			if (Peds == LocalPlayer) continue;

			if (Peds->GetHealth() <= 0) continue;

			Vector3 DistanceCalculation = (LocalPlayer->GetCoordinate() - Peds->GetCoordinate());
			double Distance = sqrtf(DistanceCalculation.x * DistanceCalculation.x + DistanceCalculation.y * DistanceCalculation.y + DistanceCalculation.z * DistanceCalculation.z);







			if (Distance <= Settings::Aimbot::DistanceAimbotLimit)
			{
				ImVec2 PedPos = FiveM::WorldToScreen(Peds->GetCoordinate());

				ImVec2 Head = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x796e);
				if (!IsOnScreen(Head)) continue;

				ImVec2 middle = ImVec2(FiveM::WindowSize.x / 2 , FiveM::WindowSize.y / 2 );
				dist = FiveM::pythag(middle, Head);

				if (dist < closestdist)
				{
					{


						closestdist = dist;
						ClosestPlayer = reinterpret_cast<uint64_t>(Peds);


					}
				}

			}
		}
		return ClosestPlayer;
	}
	float get_distance(Vector3 pointA, Vector3 pointB) {
		float x_ba = (float)(pointB.x - pointA.x);
		float y_ba = (float)(pointB.y - pointA.y);
		float z_ba = (float)(pointB.z - pointA.z);
		float y_2 = y_ba * y_ba;
		float x_2 = x_ba * x_ba;
		float sum_2 = y_2 + x_2;
		return (float)sqrtf(sum_2 + z_ba);
	}
	
	PVector3 ToPVector3(Vector3 pVec)
	{
		return PVector3(pVec.x, pVec.y, pVec.z);
	}

	Vector3 ToVector3(PVector3 pVec)
	{
		return Vector3(pVec.x, pVec.y, pVec.z);
	}
	DWORD64 TICKS = GetTickCount64();


	void aimbot_silent(void)
	{

		auto player = PLAYER::PLAYER_PED_ID();
		auto WeaponHash = WEAPON::GET_SELECTED_PED_WEAPON(player);
		auto WeaponDamage = WEAPON::GET_WEAPON_DAMAGE(WeaponHash, NULL);
		
		if (Settings::Aimbot::Check_Fov)
		{
			auto get_distance = [](double x1, double y1, double x2, double y2) {
				return sqrtf(pow(x2 - x1, 2.0) + pow(y2 - y1, 2.0));
			};
			hk_Ped* PedCC = (hk_Ped*)GetDistancePlayer();
			if (!PedCC) return;
			ImVec2 screen = FiveM::WorldToScreen(PedCC->GetCoordinate());

			auto center_x = FiveM::WindowSize.x / 2;
			auto center_y = FiveM::WindowSize.y / 2;
			auto fov = get_distance(center_x, center_y, screen.x, screen.y);
			if (fov < Settings::Aimbot::AimbotFov)
				return;

		}


		if (GetDistancePlayer() != NULL && PED::IS_PED_SHOOTING(player))
		{

			if (!Settings::Aimbot::magic)
			{
				if (GetTickCount64() - TICKS > 450.f)
				{
				
					hk_Ped* PedC = (hk_Ped*)GetDistancePlayer();
					if (!PedC) return;
					auto InPut = ENTITY::GET_ENTITY_COORDS(player, (bool)true);

					auto Out = ToPVector3(PedC->GetCoordinate());
					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(InPut.x, InPut.y, InPut.z, Out.x, Out.y, Out.z, (int)WeaponDamage, false, WeaponHash, player, true, false, 3.f);
				}

			}
			else
			{

				if (GetTickCount64() - TICKS > 450.f)
				{

					hk_Ped* PedC = (hk_Ped*)GetDistancePlayer();
					if (!PedC) return;

					auto Out = ToPVector3(PedC->GetCoordinate());
					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(Out.x -0.1, Out.y, Out.z , Out.x + 0.1, Out.y, Out.z, (int)WeaponDamage, false, WeaponHash, player, true, false, 3.f);
				}
			}
			TICKS = GetTickCount64();


		}
	}

	void AimVec(Vector3 point)
	{
	//	auto draw_list = ImGui::GetBackgroundDrawList();
		DWORD64 addr = FiveM::GetCamera();
		if (addr)
		{
			Vector3 CrosshairPos = *(Vector3*)(addr + 0x60);
			Vector3 caca(CrosshairPos.x - point.x, CrosshairPos.y - point.y, CrosshairPos.z - point.z);
			float distance = caca.Length();

			__try
			{
				uint64_t CamData = *(DWORD64*)(addr + 0x10);
				if (CamData != NULL)
				{
					if (*(float*)(CamData + 0x130) == 8.0f)
					{
						*(float*)(CamData + 0x130) = 111.0f;
						*(float*)(CamData + 0x134) = 111.0f;
						*(float*)(CamData + 0x4CC) = 0.0f;

						if (*(float*)(CamData + 0x49C) == 1.0f)
							*(float*)(CamData + 0x49C) = 0.0f;

						*(float*)(CamData + 0x2AC) = 0.0f;
						*(float*)(CamData + 0x2B0) = 0.0f;
					}
				}
			}
			__except ((GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {}



			int smooth = 0;

			smooth = Settings::Aimbot::AimbotSmooth + 1.5f;

			float threshold = 0.5f / (10 * 20);
			Vector3 Out = Vector3((point.x - CrosshairPos.x) / distance, (point.y - CrosshairPos.y) / distance, (point.z - CrosshairPos.z) / distance);

			if (smooth <= 1)
			{
				*(Vector3*)(addr + 0x40) = Out;  //FPS
				*(Vector3*)(addr + 0x3D0) = Out; //TPS
			}
			else
			{
				//First Person
				{
					Vector3 angles = *(Vector3*)(addr + 0x40);
					if (((Out.x - angles.x) > threshold) || ((angles.x - Out.x) > threshold))
					{
						if (angles.x > Out.x)
							*(float*)(addr + 0x40 + 0x0) -= threshold;
						else if (angles.x < Out.x)
							*(float*)(addr + 0x40 + 0x0) += threshold;
					}
					if (((Out.y - angles.y) > threshold) || ((angles.y - Out.y) > threshold))
					{
						if (angles.y > Out.y)
							*(float*)(addr + 0x40 + 0x4) -= threshold;
						else if (angles.y < Out.y)
							*(float*)(addr + 0x40 + 0x4) += threshold;
					}
					if (((Out.z - angles.z) > threshold) || ((angles.z - Out.z) > threshold))
					{
						if (angles.z > Out.z)
							*(float*)(addr + 0x40 + 0x8) -= threshold;
						else if (angles.z < Out.z)
							*(float*)(addr + 0x40 + 0x8) += threshold;
					}
				}
				//Third Person
				{
					Vector3 angles = *(Vector3*)(addr + 0x3D0);
					if (((Out.x - angles.x) > threshold) || ((angles.x - Out.x) > threshold))
					{
						if (angles.x > Out.x)
							*(float*)(addr + 0x3D0 + 0x0) -= threshold;
						else if (angles.x < Out.x)
							*(float*)(addr + 0x3D0 + 0x0) += threshold;
					}
					if (((Out.y - angles.y) > threshold) || ((angles.y - Out.y) > threshold))
					{
						if (angles.y > Out.y)
							*(float*)(addr + 0x3D0 + 0x4) -= threshold;
						else if (angles.y < Out.y)
							*(float*)(addr + 0x3D0 + 0x4) += threshold;
					}
					if (((Out.z - angles.z) > threshold) || ((angles.z - Out.z) > threshold))
					{
						if (angles.z > Out.z)
							*(float*)(addr + 0x3D0 + 0x8) -= threshold;
						else if (angles.z < Out.z)
							*(float*)(addr + 0x3D0 + 0x8) += threshold;
					}
				}
			}
		}
	}
	void do_aimbot(uintptr_t entity)
	{ // pretty buggy, needs playing around with sensitivity

		auto get_distance = [](double x1, double y1, double x2, double y2) {
			return sqrtf(pow(x2 - x1, 2.0) + pow(y2 - y1, 2.0));
		};

		auto bone_pos = FiveM::GetBonePosW2S(entity, 0x796e);
		//Vec3 bone_postest = GetBonePos(entity, SKEL_Head);
		auto center_x = FiveM::WindowSize.x / 2;
		auto center_y = FiveM::WindowSize.y / 2;
		if (bone_pos.x < 0 || bone_pos.x > FiveM::WindowSize.x || bone_pos.y < 0 || bone_pos.y > FiveM::WindowSize.y) return;

		float aimspeedon = Settings::Aimbot::AimbotSmooth + 1.5f;


		switch (Settings::Aimbot::AimbotBone)
		{
		case 0:
			//bone_pos = FiveM::GetBonePosW2S(entity, SKEL_Head);
			break;
		case 1:
			bone_pos = FiveM::GetBonePosW2S(entity, 0x60F2);
			break;
		case 2:
			bone_pos = FiveM::GetBonePosW2S(entity, 0xF9BB);
			break;
		case 3:
			bone_pos = FiveM::GetBonePosW2S(entity, 0x9000);
			break;
		case 4:
			bone_pos = FiveM::GetBonePosW2S(entity, 0x9D4D);
			break;
		case 5:
			bone_pos = FiveM::GetBonePosW2S(entity, 0xb1);
			break;
		}



		ImVec2 screen = bone_pos;

		auto fov = get_distance(center_x, center_y, screen.x, screen.y);
		float TargetX = 0;
		float TargetY = 0;

		if (screen.x != 0) {
			if (screen.x > center_x) {
				TargetX = -(center_x - screen.x);
				TargetX /= aimspeedon + 1.4f;
				if (TargetX + center_x > center_x * 2) TargetX = 0;
			}

			if (screen.x < center_x) {
				TargetX = screen.x - center_x;
				TargetX /= aimspeedon + 1.4f;
				if (TargetX + center_x < 0) TargetX = 0;
			}
		}

		if (screen.y != 0) {
			if (screen.y > center_y) {
				TargetY = -(center_y - screen.y);
				TargetY /= aimspeedon + 1.4f;
				if (TargetY + center_y > center_y * 2) TargetY = 0;
			}

			if (screen.y < center_y) {
				TargetY = screen.y - center_y;
				TargetY /= aimspeedon + 1.4f;
				if (TargetY + center_y < 0) TargetY = 0;
			}
		}


		/*
		float theNum = floor(TargetX / Settings::Aimbot::AimbotSmooth);
		float result = theNum / 6.666666666666667f;

		float theNum1 = floor(TargetY / Settings::Aimbot::AimbotSmooth);
		float resulte = theNum1 / 6.666666666666667f;
		float result1 = -(resulte);
		*/


		if (fov < Settings::Aimbot::AimbotFov)
		{

	
				mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(TargetX), static_cast<DWORD>(TargetY), 0, 0);

			


			
		}



	}

	void Hook(void)
	{
	

		if (Settings::Aimbot::Aimbot)
		{

			

			if (SAFE_CALL(GetAsyncKeyState)(Settings::Aimbot::Hotkey) & 0x8000) {

				if (Settings::Aimbot::AimbotTypes == 0)
				{
			uintptr_t pCPed = GetDistancePlayer();
			if (!pCPed) return;
			do_aimbot(pCPed);

				}
				else
				{
					auto get_distance = [](double x1, double y1, double x2, double y2) {
						return sqrtf(pow(x2 - x1, 2.0) + pow(y2 - y1, 2.0));
					};

					auto center_x = FiveM::WindowSize.x / 2;
					auto center_y = FiveM::WindowSize.y / 2;
					


					uintptr_t entity = GetDistancePlayer();
					if (!entity) return;
					auto bone_pos = FiveM::GetBonePos(entity, 0x796e);

	

					switch (Settings::Aimbot::AimbotBone)
					{
					case 0:
						//bone_pos = FiveM::GetBonePosW2S(entity, SKEL_Head);
						break;
					case 1:
						bone_pos = FiveM::GetBonePos(entity, 0x60F2);
						break;
					case 2:
						bone_pos = FiveM::GetBonePos(entity, 0xF9BB);
						break;
					case 3:
						bone_pos = FiveM::GetBonePos(entity, 0x9000);
						break;
					case 4:
						bone_pos = FiveM::GetBonePos(entity, 0x9D4D);
						break;
					case 5:
						bone_pos = FiveM::GetBonePos(entity, 0xb1);
						break;
					}
					ImVec2 screen = FiveM::WorldToScreen(bone_pos);

					auto fov = get_distance(center_x, center_y, screen.x, screen.y);

					if (fov < Settings::Aimbot::AimbotFov)
					{
//						if (bone_pos.x < 0 || bone_pos.x > FiveM::WindowSize.x || bone_pos.y < 0 || bone_pos.y > FiveM::WindowSize.y) return;


						AimVec(bone_pos);
					}
				}
		

			}



		}
	

	}
}