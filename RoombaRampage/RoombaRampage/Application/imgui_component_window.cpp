#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"
#include "../ECS/ECS.h"
#include<vector>
#include<string>
#include <iostream>

void ImGuiHandler::DrawComponentWindow(ECS* ecs, EntityID entityID, bool& windowOpen, const std::string& windowTitle)
{
    // Retrieve the TransformComponent
    TransformComponent* tc = static_cast<TransformComponent*>(ecs->ECS_CombinedComponentPool[TypeTransformComponent]
                            ->GetEntityComponent(entityID));

    // Check if the TransformComponent exists for the entity
    if (tc != nullptr)
    {
        ImGui::Begin(windowTitle.c_str(), &windowOpen);

        const float slider_start_pos_x = 100.0f; //Padding for the sliders

        // Display and edit the Transform component
        ImGui::Text("Transform Component");

        //Display Position
        ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
        ImGui::Text("Position");
        ImGui::SameLine(slider_start_pos_x);
        ImGui::SetNextItemWidth(100.0f);
        ImGui::DragFloat("X##", &tc->position.x, 0.02f, -1.0f,1.0f, "%.2f");

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100.0f);
        ImGui::DragFloat("Y##PosY", &tc->position.y, 0.02f, -1.0f, 1.0f, "%.2f");;

        //Display Rotation
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Rotation");
        ImGui::SameLine(slider_start_pos_x);
        ImGui::SetNextItemWidth(100.0f);
        ImGui::DragFloat("##Rot", &tc->rotation, 1.0f, -360.0f, 360.0f, "%.2f");

        //Display Scale
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Scale");
        ImGui::SameLine(slider_start_pos_x);
        ImGui::SetNextItemWidth(100.0f);
        ImGui::DragFloat("##Scale", &tc->scale, 0.02f, 0.0f, 2.0f, "%.2f");

        ImGui::End();
    }
    else
    {
        ImGui::Begin(windowTitle.c_str(), &windowOpen);
        ImGui::Text("Add a component?");
        ImGui::End();
    }
}