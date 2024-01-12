#pragma once
#include <iostream>
#include <vector>
#include "../Core/Player.hpp"
#include "../Core/LocalPlayer.hpp"
#include "../Core/Offsets.hpp"

#include "../Math/Vector2D.hpp"
#include "../Math/Vector3D.hpp"
#include "../Math/QAngle.hpp"
#include "../Math/Resolver.hpp"

#include "../Utils/Memory.hpp"
#include "../Utils/XDisplay.hpp"
#include "../Utils/Conversion.hpp"
#include "../Utils/Config.hpp"
#include "../Utils/HitboxType.hpp"

// UI //
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

// Conversion
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI / 180.f) )

struct Aimbot {
    bool AimbotEnabled = true;

    bool PredictMovement = true;
    bool PredictBulletDrop = true;

    float FinalDistance = 0;

    float Speed = 40;
    float Smooth = 10;
    float ExtraSmooth = 250;
    float FOV = 10;
    float ZoomScale = 1.2;
    float MinDistance = 1;
    float HipfireDistance = 60;
    float ZoomDistance = 160;

    bool RecoilEnabled = true;
    float PitchPower = 3;
    float YawPower = 3;
    bool WinScope = true;

    XDisplay* X11Display;
    LocalPlayer* Myself;
    std::vector<Player*>* Players;

    Player* CurrentTarget = nullptr;
    bool TargetSelected = true;
    QAngle RCSLastPunch;

    Aimbot(XDisplay* X11Display, LocalPlayer* Myself, std::vector<Player*>* GamePlayers) {
        this->X11Display = X11Display;
        this->Myself = Myself;
        this->Players = GamePlayers;
    }

    void RenderUI() {
        if (ImGui::BeginTabItem("自瞄", nullptr, ImGuiTabItemFlags_NoCloseWithMiddleMouseButton | ImGuiTabItemFlags_NoReorder)) {
            ImGui::Checkbox("启用智慧核心", &AimbotEnabled);
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("勾选以开启Alt和左键自瞄");
            ImGui::Separator();

            ImGui::Checkbox("右键瞄准", &WinScope);

            ImGui::Separator();

            ImGui::Checkbox("后坐力控制", &RecoilEnabled);
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("自瞄选中后启用后坐力控制");
            ImGui::SliderFloat("水平后坐", &PitchPower, 1, 10, "%.1f");
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("水平方向后坐控制");
            ImGui::SliderFloat("竖直后坐", &YawPower, 1, 10, "%.1f");
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("竖直方向后坐控制");

            ImGui::Separator();

            ImGui::Checkbox("预测跟枪", &PredictMovement);
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("启用后会预测敌人移动");
            ImGui::SameLine();
            ImGui::Checkbox("预测下落", &PredictBulletDrop);
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("启用后自动计算下坠距离");

            ImGui::Separator();

            ImGui::SliderFloat("鼠标移速", &Speed, 1, 100, "%.0f");
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("越大移动速度越快");

            ImGui::Separator();

            ImGui::SliderFloat("平滑", &Smooth, 1, 60, "%.0f");
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("越小吸附越强");
            ImGui::SliderFloat("额外平滑", &ExtraSmooth, 100, 5000, "%.0f");
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("准星靠近敌人时移动更平滑");

            ImGui::Separator();

            ImGui::SliderFloat("自瞄范围", &FOV, 1, 90, "%.0f");
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("具体范围看透视绘制的圈");
            ImGui::SliderFloat("开镜范围", &ZoomScale, 0, 5, "%.1f");

            ImGui::Separator();

            ImGui::SliderFloat("最小自瞄距离", &HipfireDistance, 1, 500, "%.0f");
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("自瞄的最小工作距离 。不建议调整");
            ImGui::SliderFloat("最大距离", &ZoomDistance, 1, 500, "%.0f");
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("自瞄的最大工作距离");

            ImGui::EndTabItem();
        }
    }

    bool Save() {
        try {
            Config::Aimbot::Enabled = AimbotEnabled;
            Config::Aimbot::PredictMovement = PredictMovement;
            Config::Aimbot::PredictBulletDrop = PredictBulletDrop;
            Config::Aimbot::Speed = Speed;
            Config::Aimbot::Smooth = Smooth;
            Config::Aimbot::ExtraSmooth = ExtraSmooth;
            Config::Aimbot::FOV = FOV;
            Config::Aimbot::ZoomScale = ZoomScale;
            Config::Aimbot::MinDistance = MinDistance;
            Config::Aimbot::HipfireDistance = HipfireDistance;
            Config::Aimbot::ZoomDistance = ZoomDistance;
            Config::Aimbot::RecoilControl = RecoilEnabled;
            Config::Aimbot::PitchPower = PitchPower;
            Config::Aimbot::YawPower = YawPower;
            Config::Aimbot::WinScope = WinScope;
            return true;
        } catch (...) {
            return false;
        }
    }

    void Update() {
        if (!AimbotEnabled) { ReleaseTarget(); return; }

        if (Myself->IsZooming)
            FinalDistance = ZoomDistance;
        else FinalDistance = HipfireDistance;

        if (!Myself->IsCombatReady()) { ReleaseTarget(); return; }

        if (!X11Display->KeyDown(XK_Alt_L) && (!Myself->IsInAttack) && (!Myself->IsZooming && WinScope)) { ReleaseTarget(); return; }

        if (Myself->IsHoldingGrenade) { ReleaseTarget(); return; }

        Player* Target = CurrentTarget;
        if (!IsValidTarget(Target)) {
            Target = FindBestTarget();
            if (!IsValidTarget(Target)) {
                ReleaseTarget();
                return;
            }
            
            CurrentTarget = Target;
            CurrentTarget->IsLockedOn = true;
            TargetSelected = true;
        }
        
        StartAiming();
    }

    void StartAiming() {
        // Get Target Angle
        QAngle DesiredAngles = QAngle(0, 0);
        if (!GetAngle(CurrentTarget, DesiredAngles))
            return;

        // Recoil Control
        RecoilControl(DesiredAngles);

        // Calculate Increment
        Vector2D DesiredAnglesIncrement = Vector2D(CalculatePitchIncrement(DesiredAngles), CalculateYawIncrement(DesiredAngles));

        // Calculate Smooth
        float Extra = ExtraSmooth / CurrentTarget->DistanceToLocalPlayer;
        float TotalSmooth = Smooth + Extra;

        // Aimbot calcs
        Vector2D aimbotDelta = DesiredAnglesIncrement.Divide(TotalSmooth).Multiply(Speed);
        double aimYawIncrement = aimbotDelta.y * -1;
        double aimPitchIncrement = aimbotDelta.x;

        // Turn into integers
        int totalPitchIncrementInt = RoundHalfEven(AL1AF0(aimPitchIncrement));
        int totalYawIncrementInt = RoundHalfEven(AL1AF0(aimYawIncrement));

        // Move Mouse
        if (totalPitchIncrementInt == 0 && totalYawIncrementInt == 0) return;
        X11Display->MoveMouse(totalPitchIncrementInt, totalYawIncrementInt);
    }

    bool GetAngle(Player* Target, QAngle& Angle) {
        const QAngle CurrentAngle = QAngle(Myself->ViewAngles.x, Myself->ViewAngles.y).fixAngle();
        if (!CurrentAngle.isValid())
            return false;

        if (!GetAngleToTarget(Target, Angle))
            return false;

        return true;
    }

    bool GetAngleToTarget(Player* Target, QAngle& Angle) {
        Vector3D TargetPosition = Target->GetBonePosition(static_cast<HitboxType>(GetBestBone(Target)));
        Vector3D TargetVelocity = Target->AbsoluteVelocity;
        Vector3D CameraPosition = Myself->CameraPosition;
        QAngle CurrentAngle = QAngle(Myself->ViewAngles.x, Myself->ViewAngles.y).fixAngle();
        
        if (Myself->WeaponProjectileSpeed > 1.0f) {
            if (PredictBulletDrop && PredictMovement) {
                return Resolver::CalculateAimRotationNew(CameraPosition, TargetPosition, TargetVelocity, Myself->WeaponProjectileSpeed, Myself->WeaponProjectileScale, 255, Angle);
            }
            else if (PredictBulletDrop) {
                return Resolver::CalculateAimRotationNew(CameraPosition, TargetPosition, Vector3D(0, 0, 0), Myself->WeaponProjectileSpeed, Myself->WeaponProjectileScale, 255, Angle);
            }
            else if (PredictMovement) {
                return Resolver::CalculateAimRotation(CameraPosition, TargetPosition, TargetVelocity, Myself->WeaponProjectileSpeed, Angle);
            }
        }

        Angle = Resolver::CalculateAngle(CameraPosition, TargetPosition);
        return true;   
    }

    bool IsValidTarget(Player* target) {
        if (target == nullptr ||
            !target->IsValid() || 
            !target->IsCombatReady() || 
            !target->IsVisible || 
            !target->IsHostile || 
            target->Distance2DToLocalPlayer < Conversion::ToGameUnits(MinDistance) || 
            target->Distance2DToLocalPlayer > Conversion::ToGameUnits(FinalDistance))
            return false;
        return true;
    }

    float CalculatePitchIncrement(QAngle AimbotDesiredAngles) {
        float wayA = AimbotDesiredAngles.x - Myself->ViewAngles.x;
        float wayB = 180 - abs(wayA);
        if (wayA > 0 && wayB > 0)
            wayB *= -1;
        if (fabs(wayA) < fabs(wayB))
            return wayA;
        return wayB;
    }

    float CalculateYawIncrement(QAngle AimbotDesiredAngles) {
        float wayA = AimbotDesiredAngles.y - Myself->ViewAngles.y;
        float wayB = 360 - abs(wayA);
        if (wayA > 0 && wayB > 0)
            wayB *= -1;
        if (fabs(wayA) < fabs(wayB))
            return wayA;
        return wayB;
    }

    double CalculateDistanceFromCrosshair(Vector3D TargetPosition) {
        Vector3D CameraPosition = Myself->CameraPosition;
        QAngle CurrentAngle = QAngle(Myself->ViewAngles.x, Myself->ViewAngles.y).fixAngle();

        if (CameraPosition.Distance(TargetPosition) <= 0.0001f)
            return -1;

        QAngle TargetAngle = Resolver::CalculateAngle(CameraPosition, TargetPosition);
        if (!TargetAngle.isValid())
            return -1;
        
        return CurrentAngle.distanceTo(TargetAngle);
    }

    void ReleaseTarget() {
        if (CurrentTarget != nullptr && CurrentTarget->IsValid())
            CurrentTarget->IsLockedOn = false;
        
        TargetSelected = false;
        CurrentTarget = nullptr;
    }

    int RoundHalfEven(float x) {
        return (x >= 0.0)
            ? static_cast<int>(std::round(x))
            : static_cast<int>(std::round(-x)) * -1;
    }

    float AL1AF0(float num) {
        if (num > 0) return std::max(num, 1.0f);
        return std::min(num, -1.0f);
    }

    float GetFOVScale() {
        if (Myself->IsValid()) {
            if (Myself->IsZooming) {
                if (Myself->TargetZoomFOV > 1.0 && Myself->TargetZoomFOV < 90.0) {
                    return tanf(DEG2RAD(Myself->TargetZoomFOV) * (0.64285714285));
                }
            }
        }
        return 1.0;
    }

    void RecoilControl(QAngle& Angle) {
        QAngle CurrentPunch = QAngle(Myself->PunchAngles.x, Myself->PunchAngles.y).fixAngle();
        QAngle NewPunch = { CurrentPunch.x - RCSLastPunch.x, CurrentPunch.y - RCSLastPunch.y };

		Angle.x -= NewPunch.x * YawPower;
		Angle.y -= NewPunch.y * PitchPower;

        RCSLastPunch = CurrentPunch;
    }

    int GetBestBone(Player* Target) {
        float NearestDistance = 999;
        int NearestBone = 2;
        for (int i = 0; i < 6; i++) {
            HitboxType Bone = static_cast<HitboxType>(i);
            double DistanceFromCrosshair = CalculateDistanceFromCrosshair(Target->GetBonePosition(Bone));
            if (DistanceFromCrosshair < NearestDistance) {
                NearestBone = i;
                NearestDistance = DistanceFromCrosshair;
            }
        }
        return NearestBone;
    }

    Player* FindBestTarget() {
        Player* NearestTarget = nullptr;

        float BestDistance = 9999;
        int BestHP = 100;
        int BestArmor = 125;
        float BestFOV = std::min(FOV, FOV * (GetFOVScale() * ZoomScale));

        Vector3D CameraPosition = Myself->CameraPosition;
        for (int i = 0; i < Players->size(); i++) {
            Player* p = Players->at(i);
            if (!IsValidTarget(p)) continue;
            
            if (p->DistanceToLocalPlayer < Conversion::ToGameUnits(ZoomDistance)) {
                HitboxType BestBone = static_cast<HitboxType>(GetBestBone(p));
                Vector3D TargetPosition = p->GetBonePosition(BestBone);

                float Distance = CameraPosition.Distance(TargetPosition);
                float FOV = CalculateDistanceFromCrosshair(TargetPosition);
                float HP = p->Health;
                float Armor = p->Shield;
                
                if (Distance > BestDistance) continue;
                if (FOV > BestFOV) continue;
                if (HP > BestHP) continue;
                if (Armor > BestArmor) continue;

                NearestTarget = p;
                BestDistance = Distance;
                BestHP = HP;
                BestArmor = Armor;
            }
        }
        return NearestTarget;
    }
};
