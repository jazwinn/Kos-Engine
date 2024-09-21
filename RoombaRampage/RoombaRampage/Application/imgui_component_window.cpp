#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"
#include "../ECS/ECS.h"

#pragma warning(push)
#pragma warning(disable : 26495)  // Disable uninitialized variable warning
#include "../Dependencies/rapidjson/document.h"
#include "../Dependencies/rapidjson/writer.h"
#include "../Dependencies/rapidjson/filereadstream.h"
#pragma warning(pop)

#include<vector>
#include<string>
#include <iostream>
#include "json_handler.h"

void ImGuiHandler::DrawComponentWindow(Ecs::ECS* ecs, Ecs::EntityID entityID, bool& windowOpen, const std::string& windowTitle)
{
    // Retrieve the TransformComponent
    Ecs::TransformComponent* tc = static_cast<Ecs::TransformComponent*>(ecs->ECS_CombinedComponentPool[Ecs::TypeTransformComponent]
                            ->GetEntityComponent(entityID));

    // Retrieve the TransformComponent
    Ecs::MovementComponent* mc = static_cast<Ecs::MovementComponent*>(ecs->ECS_CombinedComponentPool[Ecs::TypeMovemmentComponent]
                            ->GetEntityComponent(entityID));

    ImGui::Begin(windowTitle.c_str(), &windowOpen);

    const float slider_start_pos_x = 100.0f; //Padding for the slider

    bool isModified = false;  //Track if the user modifies the component values

    // Check if the TransformComponent exists for the entity
    if (tc != nullptr)
    {
        // Display and edit the Transform component
        ImGui::Text("Transform Component");

        //Display Position
        ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
        ImGui::Text("Position");
        ImGui::SameLine(slider_start_pos_x);
        ImGui::SetNextItemWidth(100.0f);
        if (ImGui::DragFloat("X##", &tc->position.x, 0.02f, -1.0f, 1.0f, "%.2f"))
            isModified = true;

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100.0f);
        if(ImGui::DragFloat("Y##PosY", &tc->position.y, 0.02f, -1.0f, 1.0f, "%.2f"))
            isModified = true;

        //Display Rotation
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Rotation");
        ImGui::SameLine(slider_start_pos_x);
        ImGui::SetNextItemWidth(100.0f);
        if(ImGui::DragFloat("##Rot", &tc->rotation, 1.0f, -360.0f, 360.0f, "%.2f"))
            isModified = true;

        //Display Scale
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Scale");
        ImGui::SameLine(slider_start_pos_x);
        ImGui::SetNextItemWidth(100.0f);
        if(ImGui::DragFloat("X", &tc->scale.x, 0.02f, 0.1f, 2.0f, "%.2f"))
            isModified = true;

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100.0f);
        if (ImGui::DragFloat("Y", &tc->scale.y, 0.02f, 0.1f, 2.0f, "%.2f"))
            isModified = true;

    }
    if (mc != nullptr)
    {
        ImGui::Text("Movement Component");

        // Display Speed
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Speed");
        ImGui::SameLine(slider_start_pos_x);
        ImGui::SetNextItemWidth(100.0f);
        if (ImGui::DragFloat("##Speed", &mc->Speed, 0.01f, -10.0f, 10.0f, "%.2f"))
            isModified = true;

        // Display Velocity
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Direction");
        ImGui::SameLine(slider_start_pos_x);
        ImGui::SetNextItemWidth(100.0f);
        if(ImGui::DragFloat("X##VelX", &mc->Direction.x, 0.02f, -1.0f, 1.0f, "%.2f"))
            isModified = true;

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100.0f);
        if (ImGui::DragFloat("Y##VelY", &mc->Direction.y, 0.02f, -1.0f, 1.0f, "%.2f"))
            isModified = true;

        //// Display Resistance
        //ImGui::AlignTextToFramePadding();
        //ImGui::Text("Resistance");
        //ImGui::SameLine(slider_start_pos_x);
        //ImGui::SetNextItemWidth(100.0f);
        //ImGui::DragFloat("##Res", &mc->resistance, 0.01f, 0.0f, 5.0f, "%.2f");

    }
    else
    {
        ImGui::Text("Add a component?");
        ImGui::End();
    }
    ImGui::End();


    // If any component was modified, save the updated values to JSON
    if (isModified)
        SaveComponentsJson("../RoombaRampage/Json Texts/components.json", tc, mc);
}