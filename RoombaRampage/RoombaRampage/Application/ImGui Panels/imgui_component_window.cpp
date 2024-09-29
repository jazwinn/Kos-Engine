#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"
#include "../ECS/ECS.h"
#include "../Assets/AssetManager.h"

#pragma warning(push)
#pragma warning(disable : 26495)  // Disable uninitialized variable warning
#include "../Dependencies/rapidjson/document.h"
#include "../Dependencies/rapidjson/writer.h"
#include "../Dependencies/rapidjson/filereadstream.h"
#pragma warning(pop)

#include<vector>
#include<string>
#include <iostream>
#include "../De&Serialization/json_handler.h"

void ImGuiHandler::DrawComponentWindow()
{
    bool windowOpen = true;
    std::string Title = "Component Window";
    Ecs::ECS* ecs = Ecs::ECS::GetInstance();

    std::string windowTitle = Title;

    ImGui::Begin(windowTitle.c_str(), &windowOpen);

    //Add Component Window
    const char* ComponentNames[] =
    {
        "Add Components","Movement Component", "Collider Component", "Sprite Component", "Player Component", "Rigid Body Component"
    };
    static int ComponentType = 0;

    for (size_t i = 0; i < obj_text_entries.size(); i++) {
        if (obj_component_window[i] && obj_entity_id[i] == clicked_entity_id)
        {
            Ecs::EntityID entityID = obj_entity_id[i];

            ImGui::Combo("##ADDCOMPONENT", &ComponentType, ComponentNames, IM_ARRAYSIZE(ComponentNames), IM_ARRAYSIZE(ComponentNames));
            if (ComponentType == 1) {
                ecs->AddComponent(Ecs::TypeMovemmentComponent, entityID);
                ComponentType = 0;
            }
            if (ComponentType == 2) {
                ecs->AddComponent(Ecs::TypeColliderComponent, entityID);
                ComponentType = 0;
            }
            if (ComponentType == 3) {
                ecs->AddComponent(Ecs::TypeSpriteComponent, entityID);
                ComponentType = 0;
            }
            if (ComponentType == 4) {
                ecs->AddComponent(Ecs::TypePlayerComponent, entityID);
                ComponentType = 0;
            }
            if (ComponentType == 5) {
                ecs->AddComponent(Ecs::TypeRigidBodyComponent, entityID);
                ComponentType = 0;
            }

            const float slider_start_pos_x = 100.0f; //Padding for the slider

            bool isModified = false;  //Track if the user modifies the component values
            bool isFirstSaved = true;

            // Check if the TransformComponent exists for the entity

            Ecs::compSignature EntitySignature = ecs->ECS_EntityMap[entityID];
            
            if (EntitySignature.test(Ecs::TypeTransformComponent))
            {
                
                
                if (ImGui::CollapsingHeader("Transform Component"))
                {
                    // Retrieve the TransformComponent
                    Ecs::TransformComponent* tc = static_cast<Ecs::TransformComponent*>(ecs->ECS_CombinedComponentPool[Ecs::TypeTransformComponent]
                        ->GetEntityComponent(entityID));

                    //Display Position
                    ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
                    ImGui::Text("Position");
                    ImGui::SameLine(slider_start_pos_x);
                    ImGui::SetNextItemWidth(100.0f);
                    if (ImGui::DragFloat("X##", &tc->position.x, 0.02f, -1.0f, 1.0f, "%.2f"))
                        isModified = true;

                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.0f);
                    if (ImGui::DragFloat("Y##PosY", &tc->position.y, 0.02f, -1.0f, 1.0f, "%.2f"))
                        isModified = true;

                    //Display Rotation
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Rotation");
                    ImGui::SameLine(slider_start_pos_x);
                    ImGui::SetNextItemWidth(100.0f);
                    if (ImGui::DragFloat("##Rot", &tc->rotation, 1.0f, -360.0f, 360.0f, "%.2f"))
                        isModified = true;

                    //Display Scale
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Scale");
                    ImGui::SameLine(slider_start_pos_x);
                    ImGui::SetNextItemWidth(100.0f);
                    if (ImGui::DragFloat("X", &tc->scale.x, 0.02f, 0.1f, 2.0f, "%.2f"))
                        isModified = true;

                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.0f);
                    if (ImGui::DragFloat("Y", &tc->scale.y, 0.02f, 0.1f, 2.0f, "%.2f"))
                        isModified = true;
                }
            }

            if (EntitySignature.test(Ecs::TypeMovemmentComponent))
            {
                if (ImGui::CollapsingHeader("Movement Component"))
                {
                    // retrieve movement component
                    Ecs::MovementComponent* mc = static_cast<Ecs::MovementComponent*>(ecs->ECS_CombinedComponentPool[Ecs::TypeMovemmentComponent]
                        ->GetEntityComponent(entityID));

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
                    if (ImGui::DragFloat("X##VelX", &mc->Direction.x, 0.02f, -1.0f, 1.0f, "%.2f"))
                        isModified = true;

                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.0f);
                    if (ImGui::DragFloat("Y##VelY", &mc->Direction.y, 0.02f, -1.0f, 1.0f, "%.2f"))
                        isModified = true;

                    // Display Resistance
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Auto Move");
                    ImGui::SameLine(slider_start_pos_x);
                    ImGui::Checkbox("##", &mc->Move);

                }
            }

            if (EntitySignature.test(Ecs::TypeColliderComponent))
            {

                if (ImGui::CollapsingHeader("Collider Component")) {

                    //retrieve collision collider
                    Ecs::ColliderComponent* cc = static_cast<Ecs::ColliderComponent*>(ecs->ECS_CombinedComponentPool[Ecs::TypeColliderComponent]
                        ->GetEntityComponent(entityID));

                    //Display Position
                    ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
                    ImGui::Text("Size");
                    ImGui::SameLine(slider_start_pos_x);
                    ImGui::SetNextItemWidth(100.0f);
                    if (ImGui::DragFloat("X###", &cc->Size.x, 0.02f, 0.f, 2.0f, "%.2f"))
                        isModified = true;

                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.0f);
                    if (ImGui::DragFloat("Y###PosY", &cc->Size.y, 0.02f, 0.f, 2.0f, "%.2f"))
                        isModified = true;

                    //Display Rotation
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Display Collision");
                    ImGui::SameLine(slider_start_pos_x + 40);
                    ImGui::Checkbox("####xx", &cc->drawDebug);
                    isModified = true;

                    //Display Scale
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Offset");
                    ImGui::SameLine(slider_start_pos_x);
                    ImGui::SetNextItemWidth(100.0f);
                    if (ImGui::DragFloat("XX##VelX", &cc->OffSet.x, 0.02f, -1.f, 1.0f, "%.2f"))
                        isModified = true;

                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.0f);
                    if (ImGui::DragFloat("YY##VelY", &cc->OffSet.y, 0.02f, -1.f, 1.f, "%.2f"))
                        isModified = true;
                }
            }

            if (EntitySignature.test(Ecs::TypeSpriteComponent))
            {
                //retrieve sprite component
                Ecs::SpriteComponent* sc = static_cast<Ecs::SpriteComponent*>(ecs->ECS_CombinedComponentPool[Ecs::TypeSpriteComponent]
                    ->GetEntityComponent(entityID));

                if (ImGui::CollapsingHeader("Sprite Component")) {
                    AssetManager* images = AssetManager::funcGetInstance();

                    const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE" };
                    static int item_selected_idx = 0; // Here we store our selected data as an index.

                    static bool item_highlight = false;
                    int item_highlighted_idx = -1; // Here we store our highlighted data as an index.
                    //ImGui::Checkbox("Highlight hovered item in second listbox", &item_highlight);

                    if (ImGui::BeginListBox("Images"))
                    {
                        item_selected_idx = sc->imageID;
                        for (unsigned int n = 0; n < images->imageContainer.size(); n++)
                        {
                            bool is_selected = (item_selected_idx == n);
                            if (ImGui::Selectable(const_cast<char*>(images->imageContainer[n].spriteName.c_str()), is_selected))
                            {
                                item_selected_idx = n;
                                is_selected = true;
                            }

                            if (item_highlight && ImGui::IsItemHovered())
                            {
                                item_highlighted_idx = n;
                            }

                            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                            if (is_selected)
                            {
                                ImGui::SetItemDefaultFocus();
                                sc->imageID = n;
                            }

                        }

                        ImGui::EndListBox();
                    }
                }

            }
            if (EntitySignature.test(Ecs::TypePlayerComponent)) {
                Ecs::PlayerComponent* pc = static_cast<Ecs::PlayerComponent*>(ecs->ECS_CombinedComponentPool[Ecs::TypePlayerComponent]
                    ->GetEntityComponent(entityID));
                if (ImGui::CollapsingHeader("Player Component")) {
                
                    ImGui::Checkbox("Player Control", &pc->Control);
                
                
                }
            }

            if (EntitySignature.test(Ecs::TypeRigidBodyComponent)) {
                Ecs::RigidBodyComponent* rc = static_cast<Ecs::RigidBodyComponent*>(ecs->ECS_CombinedComponentPool[Ecs::TypeRigidBodyComponent]
                    ->GetEntityComponent(entityID));
                if (ImGui::CollapsingHeader("RigidBody Component")) {

                    ImGui::Text("Work In Progress");


                }
            }

            // If any component was modified, save the updated values to JSON
            if (isFirstSaved || isModified)
            {
                Serialization::Serialize::SaveComponentsJson("../RoombaRampage/Json Texts", Ecs::ECS::GetInstance()->ECS_EntityMap, obj_text_entries);
                isFirstSaved = false; // Ensure this is only set once
            }

            obj_component_window[i] = windowOpen;
        }
    }

    ImGui::End();
}   