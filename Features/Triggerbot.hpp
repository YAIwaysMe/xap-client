#pragma once
#include <iostream>
#include <vector>
#include <set>
#include "../Core/Player.hpp"
#include "../Core/LocalPlayer.hpp"
#include "../Core/Offsets.hpp"
#include "../Utils/Memory.hpp"
#include "../Math/Vector2D.hpp"
#include "../Math/Vector3D.hpp"
#include "../Utils/XDisplay.hpp"
#include "../Utils/Conversion.hpp"
#include "../Utils/Config.hpp"

// UI //
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

struct Triggerbot {
    bool TriggerbotEnabled = true;
    float TriggerbotRange = 200;

    std::set<int> WeaponList = { 1, 84, 95, 86, 102, 94, 104, 88, 110, 89, 112, 106, 108, 91 };

    XDisplay* X11Display;
    LocalPlayer* Myself;
    std::vector<Player*>* Players;

    Triggerbot(XDisplay* X11Display, LocalPlayer* Myself, std::vector<Player*>* GamePlayers)
     {
        this->X11Display = X11Display;
        this->Myself = Myself;
        this->Players = GamePlayers;
    }

    void RenderUI() {
        if (ImGui::BeginTabItem("扳机", nullptr, ImGuiTabItemFlags_NoCloseWithMiddleMouseButton | ImGuiTabItemFlags_NoReorder)) {
            ImGui::Checkbox("启用扳机", &TriggerbotEnabled);
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("按住Shift启动扳机");
            ImGui::SliderFloat("扳机生效距离", &TriggerbotRange, 0, 1000, "%.0f");
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("字面意思");
                
            ImGui::EndTabItem();
        }
    }
    
       

    bool Save() {
        try {
            Config::Triggerbot::Enabled = TriggerbotEnabled;
            Config::Triggerbot::Range = TriggerbotRange;

            return true;
        } catch (...) {
            return false;
        }
    }

    void Update() {
        if (!TriggerbotEnabled) return;
        if (!Myself->IsCombatReady()) return;

        if (WeaponList.find(Myself->WeaponIndex) == WeaponList.end()) return;

        for (int i = 0; i < Players->size(); i++) {
            Player* player = Players->at(i);
            if (!player->IsCombatReady()) continue;
            if (!player->IsHostile) continue;
            if (!player->IsAimedAt) continue;
            if (player->DistanceToLocalPlayer < Conversion::ToGameUnits(TriggerbotRange) && X11Display->KeyDown(XK_Shift_L)) {
                X11Display->MouseClickLeft();
                break;
            }
        }
    }
};
