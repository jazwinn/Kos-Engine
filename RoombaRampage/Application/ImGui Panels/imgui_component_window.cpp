#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"
#include "imgui_stdlib.h"
#include "../ECS/ECS.h"
#include "../Assets/AssetManager.h"
#include "../Graphics/GraphicsPipe.h"

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
    ecs::ECS* ecs = ecs::ECS::m_GetInstance();

    std::string windowTitle = Title;

    ImGui::Begin(windowTitle.c_str(), &windowOpen);

    //Add Component Window
    const char* ComponentNames[] =
    {
        "Add Components","Movement Component", "Collider Component", "Sprite Component", "Player Component", "Rigid Body Component", "Text Component", "Animation Component"
    };
    static int ComponentType = 0;





    if (obj_text_entries.size() > 0) {

        ecs::EntityID entityID = clicked_entity_id;

        ImGui::Combo("##ADDCOMPONENT", &ComponentType, ComponentNames, IM_ARRAYSIZE(ComponentNames), IM_ARRAYSIZE(ComponentNames));
        if (ComponentType == 1) {
            ecs->m_AddComponent(ecs::TYPEMOVEMENTCOMPONENT, entityID);
            ComponentType = 0;
        }
        if (ComponentType == 2) {
            ecs->m_AddComponent(ecs::TYPECOLLIDERCOMPONENT, entityID);
            ComponentType = 0;
        }
        if (ComponentType == 3) {
            ecs->m_AddComponent(ecs::TYPESPRITECOMPONENT, entityID);
            ComponentType = 0;
        }
        if (ComponentType == 4) {
            ecs->m_AddComponent(ecs::TYPEPLAYERCOMPONENT, entityID);
            ComponentType = 0;
        }
        if (ComponentType == 5) {
            ecs->m_AddComponent(ecs::TYPERIGIDBODYCOMPONENT, entityID);
            ComponentType = 0;
        }
        if (ComponentType == 6) {
            ecs->m_AddComponent(ecs::TYPETEXTCOMPONENT, entityID);
            ComponentType = 0;
        }
        if (ComponentType == 7) {
            ecs->m_AddComponent(ecs::TYPEANIMATIONCOMPONENT, entityID);
            ComponentType = 0;
        }


        const float slider_start_pos_x = 100.0f; //Padding for the slider


        // Check if the TransformComponent exists for the entity

        ecs::compSignature EntitySignature = ecs->m_ECS_EntityMap[entityID];

        ImGui::SeparatorText("Components");

        if (EntitySignature.test(ecs::TYPENAMECOMPONENT))
        {



            // Retrieve the TransformComponent
            ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]
                ->m_GetEntityComponent(entityID));

            //Display Position
            ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
            ImGui::Text("Object Name: ");
            ImGui::SameLine(slider_start_pos_x);
            ImGui::SetNextItemWidth(100.0f);
            if (ImGui::InputText("##NAMETEXT##", &nc->m_entityName)) {
                for (int n{}; n < obj_entity_id.size(); n++) {
                    if (obj_entity_id[n] == entityID) {
                        obj_text_entries[n] = nc->m_entityName;
                    }
                }
            }

            
        }
            
        if (EntitySignature.test(ecs::TYPETRANSFORMCOMPONENT))
        {
                
                
            if (ImGui::CollapsingHeader("Transform Component"))
            {
                // Retrieve the TransformComponent
                ecs::TransformComponent* tc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]
                    ->m_GetEntityComponent(entityID));

                //Display Position
                ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
                ImGui::Text("Position");
                ImGui::SameLine(slider_start_pos_x);
                ImGui::SetNextItemWidth(100.0f);
                ImGui::DragFloat("X##", &tc->m_position.m_x, 0.02f, -2.f, 2.f, "%.2f");

                ImGui::SameLine();
                ImGui::SetNextItemWidth(100.0f);
                ImGui::DragFloat("Y##PosY", &tc->m_position.m_y, 0.02f, -1.0f, 1.0f, "%.2f");

                //Display Rotation
                ImGui::AlignTextToFramePadding();
                ImGui::Text("Rotation");
                ImGui::SameLine(slider_start_pos_x);
                ImGui::SetNextItemWidth(100.0f);
                ImGui::DragFloat("##Rot", &tc->m_rotation, 1.0f, -360.0f, 360.0f, "%.2f");

                //Display Scale
                ImGui::AlignTextToFramePadding();
                ImGui::Text("Scale");
                ImGui::SameLine(slider_start_pos_x);
                ImGui::SetNextItemWidth(100.0f);
                ImGui::DragFloat("X", &tc->m_scale.m_x, 0.02f, 0.1f, 2.0f, "%.2f");

                ImGui::SameLine();
                ImGui::SetNextItemWidth(100.0f);
                ImGui::DragFloat("Y", &tc->m_scale.m_y, 0.02f, 0.1f, 2.0f, "%.2f");
            }
        }
        if (EntitySignature.test(ecs::TYPEMOVEMENTCOMPONENT))
        {
            if (ImGui::CollapsingHeader("Movement Component"))
            {
                // retrieve movement component
                ecs::MovementComponent* mc = static_cast<ecs::MovementComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEMOVEMENTCOMPONENT]
                    ->m_GetEntityComponent(entityID));

                // Display Speed
                ImGui::AlignTextToFramePadding();
                ImGui::Text("Speed");
                ImGui::SameLine(slider_start_pos_x);
                ImGui::SetNextItemWidth(100.0f);
                ImGui::DragFloat("##Speed", &mc->m_Speed, 0.01f, -10.0f, 10.0f, "%.2f");

                // Display Velocity
                ImGui::AlignTextToFramePadding();
                ImGui::Text("Direction");
                ImGui::SameLine(slider_start_pos_x);
                ImGui::SetNextItemWidth(100.0f);
                ImGui::DragFloat("X##VelX", &mc->m_Direction.m_x, 0.02f, -1.0f, 1.0f, "%.2f");

                ImGui::SameLine();
                ImGui::SetNextItemWidth(100.0f);
                ImGui::DragFloat("Y##VelY", &mc->m_Direction.m_y, 0.02f, -1.0f, 1.0f, "%.2f");


                //// Display Resistance
                //ImGui::AlignTextToFramePadding();
                //ImGui::Text("Auto Move");
                //ImGui::SameLine(slider_start_pos_x);
                //ImGui::Checkbox("##", &mc->m_Move);

            }

               

        }
        if (EntitySignature.test(ecs::TYPECOLLIDERCOMPONENT))
        {

            if (ImGui::CollapsingHeader("Collider Component")) {

                //retrieve collision collider
                ecs::ColliderComponent* cc = static_cast<ecs::ColliderComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]
                    ->m_GetEntityComponent(entityID));

                //Display size
                ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
                ImGui::Text("Size");
                ImGui::SameLine(slider_start_pos_x);
                ImGui::SetNextItemWidth(100.0f);
                ImGui::DragFloat("X###", &cc->m_Size.m_x, 0.02f, 0.f, 2.0f, "%.2f");


                ImGui::SameLine();
                ImGui::SetNextItemWidth(100.0f);
                ImGui::DragFloat("Y###PosY", &cc->m_Size.m_y, 0.02f, 0.f, 2.0f, "%.2f");
   

                ImGui::AlignTextToFramePadding();
                ImGui::Text("Display Collision");
                ImGui::SameLine(slider_start_pos_x + 40);
                ImGui::Checkbox("####xx", &cc->m_drawDebug);
            

                ImGui::AlignTextToFramePadding();
                ImGui::Text("Offset");
                ImGui::SameLine(slider_start_pos_x);
                ImGui::SetNextItemWidth(100.0f);
                ImGui::DragFloat("XX##VelX", &cc->m_OffSet.m_x, 0.02f, -1.f, 1.0f, "%.2f");
                  

                ImGui::SameLine();
                ImGui::SetNextItemWidth(100.0f);
                ImGui::DragFloat("YY##VelY", &cc->m_OffSet.m_y, 0.02f, -1.f, 1.f, "%.2f");
                   
            }

                
        }
        if (EntitySignature.test(ecs::TYPESPRITECOMPONENT))
        {
            //retrieve sprite component
            ecs::SpriteComponent* sc = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]
                ->m_GetEntityComponent(entityID));

            if (ImGui::CollapsingHeader("Sprite Component")) {
                assetmanager::AssetManager* images = assetmanager::AssetManager::m_funcGetInstance();

                const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE" };
                static int item_selected_idx = 0; // Here we store our selected data as an index.

                static bool item_highlight = false;
                int item_highlighted_idx = -1; // Here we store our highlighted data as an index.
                //ImGui::Checkbox("Highlight hovered item in second listbox", &item_highlight);

                if (ImGui::BeginListBox("Images"))
                {
                    item_selected_idx = sc->m_imageID;
                    for (unsigned int n = 0; n < images->m_imageContainer.size(); n++)
                    {
                        bool is_selected = (item_selected_idx == n);
                        if (ImGui::Selectable(const_cast<char*>(images->m_imageContainer[n].m_spriteName.c_str()), is_selected))
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
                            sc->m_imageID = n;
                        }

                    }
                    ImGui::EndListBox();
                }
            }


        }
        if (EntitySignature.test(ecs::TYPEPLAYERCOMPONENT)) {
            ecs::PlayerComponent* pc = static_cast<ecs::PlayerComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEPLAYERCOMPONENT]
                ->m_GetEntityComponent(entityID));
            if (ImGui::CollapsingHeader("Player Component")) {
                
                ImGui::Checkbox("Player Control", &pc->m_Control);
                
                
            }
        }

        if (EntitySignature.test(ecs::TYPERIGIDBODYCOMPONENT)) {
            //ecs::RigidBodyComponent* rc = static_cast<ecs::RigidBodyComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPERIGIDBODYCOMPONENT]
            //    ->m_GetEntityComponent(entityID));
            if (ImGui::CollapsingHeader("RigidBody Component")) {

                ImGui::Text("Work In Progress");


            }
        }
        
        if (EntitySignature.test(ecs::TYPETEXTCOMPONENT))
        {


            if (ImGui::CollapsingHeader("Text Component"))
            {
                

            }
        }
        if (EntitySignature.test(ecs::TYPEANIMATIONCOMPONENT))
        {
            //retrieve sprite component
            ecs::AnimationComponent* ac = static_cast<ecs::AnimationComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEANIMATIONCOMPONENT]
                ->m_GetEntityComponent(entityID));
            graphicpipe::GraphicsPipe* graphicsPipe = graphicpipe::GraphicsPipe::m_funcGetInstance();

            if (ImGui::CollapsingHeader("Animation Component")) {


                // Retrieve the Animation Component
                ecs::AnimationComponent* ac = static_cast<ecs::AnimationComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEANIMATIONCOMPONENT]
                    ->m_GetEntityComponent(entityID));

                ImGui::AlignTextToFramePadding();
                ImGui::Text("Toggle Animation");
                ImGui::SameLine(slider_start_pos_x + 40);
                ImGui::Checkbox("####xx##", &ac->m_isAnimating);

            
               ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
               ImGui::Text("Seconds Per Frame: ");
               ImGui::SameLine(slider_start_pos_x + 100);
               ImGui::SetNextItemWidth(100.0f);
               ImGui::DragFloat("##SPF##", &graphicsPipe->m_frameTime, 0.01f, 0.04f, 2.f, "%.2f");


            }
        }
        


     }

    ImGui::End();
}   