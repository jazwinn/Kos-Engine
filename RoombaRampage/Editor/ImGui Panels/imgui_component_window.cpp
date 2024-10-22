/******************************************************************/
/*!
\file      imgui_component_window.cpp
\author    Chiu Jun Jie, junjie.c , 2301524
\par       junjie.c@digipen.edu
\date      Oct 02, 2024
\brief     This file implements the ImGui component window for handling
           user interactions with ECS components in the application. It
           enables the addition, display, and modification of various
           entity components within the ECS framework.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"
#include "imgui_stdlib.h"
#include "../ECS/ECS.h"
#include "../Assets/AssetManager.h"
#include "../Graphics/GraphicsPipe.h"
#include "../ECS/Layers.h"

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

void gui::ImGuiHandler::m_DrawComponentWindow()
{
    bool windowOpen = true;
    std::string Title = "Component Window";
    ecs::ECS* ecs = ecs::ECS::m_GetInstance();

    std::string windowTitle = Title;

    ImGui::Begin(windowTitle.c_str(), &windowOpen);

    //Add Component Window
    const char* ComponentNames[] =
    {
        "Add Components", "Collider Component", "Sprite Component", "Player Component", "Rigid Body Component", "Text Component", "Animation Component", "Camera Component"
    };
    static int ComponentType = 0;



    if (ecs->m_ECS_EntityMap.size() > 0 && m_clickedEntityId >= 0) {

        ecs::EntityID entityID = m_clickedEntityId;

 

        if (ImGui::Combo("##ADDCOMPONENT", &ComponentType, ComponentNames, IM_ARRAYSIZE(ComponentNames), IM_ARRAYSIZE(ComponentNames))) {
            if (ComponentType == 1) {
                ecs->m_AddComponent(ecs::TYPECOLLIDERCOMPONENT, entityID);
                ComponentType = 0;
            }
            if (ComponentType == 2) {
                ecs->m_AddComponent(ecs::TYPESPRITECOMPONENT, entityID);
                ComponentType = 0;
            }
            if (ComponentType == 3) {
                ecs->m_AddComponent(ecs::TYPEPLAYERCOMPONENT, entityID);
                ComponentType = 0;
            }
            if (ComponentType == 4) {
                ecs->m_AddComponent(ecs::TYPERIGIDBODYCOMPONENT, entityID);
                ComponentType = 0;
            }
            if (ComponentType == 5) {
                ecs->m_AddComponent(ecs::TYPETEXTCOMPONENT, entityID);
                ComponentType = 0;
            }
            if (ComponentType == 6) {
                ecs->m_AddComponent(ecs::TYPEANIMATIONCOMPONENT, entityID);
                ComponentType = 0;
            }
            if (ComponentType == 7) {
                ecs->m_AddComponent(ecs::TYPECAMERACOMPONENT, entityID);
                ComponentType = 0;
            }
        }
        

        const float slider_start_pos_x = 100.0f; //Padding for the slider




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
            ImGui::InputText("##NAMETEXT##", &nc->m_entityName);

            //layer selector
            const char* layers[] = {ecs->m_layersStack.m_layerMap[layer::DEFAULT].first.c_str(), ecs->m_layersStack.m_layerMap[layer::LAYER1].first.c_str(),ecs->m_layersStack.m_layerMap[layer::LAYER2].first.c_str(),
                                  ecs->m_layersStack.m_layerMap[layer::LAYER3].first.c_str(), ecs->m_layersStack.m_layerMap[layer::LAYER4].first.c_str(), ecs->m_layersStack.m_layerMap[layer::LAYER5].first.c_str(), 
                                  ecs->m_layersStack.m_layerMap[layer::LAYER6].first.c_str(), ecs->m_layersStack.m_layerMap[layer::LAYER7].first.c_str(), ecs->m_layersStack.m_layerMap[layer::LAYER8].first.c_str()};
            int layer_current = nc->m_Layer;
            if (ImGui::Combo("Layers", &layer_current, layers, IM_ARRAYSIZE(layers))) {
                switch (layer_current) {
                case 0:
                    ecs->m_layersStack.m_SwapEntityLayer(layer::DEFAULT, nc->m_Layer, entityID);
                    break;
                case 1:
                    ecs->m_layersStack.m_SwapEntityLayer(layer::LAYER1, nc->m_Layer, entityID);
                    break;
                case 2:
                    ecs->m_layersStack.m_SwapEntityLayer(layer::LAYER2, nc->m_Layer, entityID);
                    break;
                case 3:
                    ecs->m_layersStack.m_SwapEntityLayer(layer::LAYER3, nc->m_Layer, entityID);
                    break;
                case 4:
                    ecs->m_layersStack.m_SwapEntityLayer(layer::LAYER4, nc->m_Layer, entityID);
                    break;
                case 5:
                    ecs->m_layersStack.m_SwapEntityLayer(layer::LAYER5, nc->m_Layer, entityID);
                    break;
                case 6:
                    ecs->m_layersStack.m_SwapEntityLayer(layer::LAYER6, nc->m_Layer, entityID);
                    break;
                case 7:
                    ecs->m_layersStack.m_SwapEntityLayer(layer::LAYER7, nc->m_Layer, entityID);
                    break;
                case 8:
                    ecs->m_layersStack.m_SwapEntityLayer(layer::LAYER8, nc->m_Layer, entityID);
                    break;

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
                ImGui::Text("Translation");
                ImGui::SameLine(slider_start_pos_x);
                ImGui::SetNextItemWidth(100.0f);
                ImGui::DragFloat("X##", &tc->m_position.m_x, 0.02f, -50.f, 50.f, "%.2f");

                ImGui::SameLine();
                ImGui::SetNextItemWidth(100.0f);
                ImGui::DragFloat("Y##PosY", &tc->m_position.m_y, 0.02f, -50.0f, 50.0f, "%.2f");

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
                ImGui::DragFloat("X", &tc->m_scale.m_x, 0.02f, 0.1f, 10.0f, "%.2f");

                ImGui::SameLine();
                ImGui::SetNextItemWidth(100.0f);
                ImGui::DragFloat("Y", &tc->m_scale.m_y, 0.02f, 0.1f, 10.0f, "%.2f");

            }

        }
       


        if (EntitySignature.test(ecs::TYPECOLLIDERCOMPONENT))
        {
            bool open = ImGui::CollapsingHeader("Collider Component");
            //retrieve collision collider
            ecs::ColliderComponent* cc = static_cast<ecs::ColliderComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]
                ->m_GetEntityComponent(entityID));


            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Delete Component")) {
                    ecs->m_RemoveComponent(ecs::TYPECOLLIDERCOMPONENT, m_clickedEntityId);
                }
                ImGui::EndPopup();
            }

            if (open) {

                //Display size
                ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
                ImGui::Text("Size");
                ImGui::SameLine(slider_start_pos_x);
                ImGui::SetNextItemWidth(100.0f);
                ImGui::DragFloat("X###", &cc->m_Size.m_x, 0.02f, 0.f, 10.0f, "%.2f");


                ImGui::SameLine();
                ImGui::SetNextItemWidth(100.0f);
                ImGui::DragFloat("Y###PosY", &cc->m_Size.m_y, 0.02f, 0.f, 10.0f, "%.2f");
   

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

            bool open = ImGui::CollapsingHeader("Sprite Component");

            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Delete Component")) {
                    ecs->m_RemoveComponent(ecs::TYPESPRITECOMPONENT, m_clickedEntityId);
                }
                ImGui::EndPopup();
            }

            if (open) {

                //retrieve sprite component
                ecs::SpriteComponent* sc = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]
                    ->m_GetEntityComponent(entityID));

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
                        bool is_selected = (item_selected_idx == static_cast<int>(n));
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

            bool open = ImGui::CollapsingHeader("Player Component");

            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Delete Component")) {
                    ecs->m_RemoveComponent(ecs::TYPEPLAYERCOMPONENT, m_clickedEntityId);
                }
                ImGui::EndPopup();
            }
            
            if (open) {

                ecs::PlayerComponent* pc = static_cast<ecs::PlayerComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEPLAYERCOMPONENT]
                    ->m_GetEntityComponent(entityID));

                ImGui::Checkbox("Player Control", &pc->m_Control);
            }
        }

        if (EntitySignature.test(ecs::TYPERIGIDBODYCOMPONENT)) {

            bool open = ImGui::CollapsingHeader("RigidBody Component");

            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Delete Component")) {
                    ecs->m_RemoveComponent(ecs::TYPERIGIDBODYCOMPONENT, m_clickedEntityId);
                }
                ImGui::EndPopup();
            }

            if (open) {

                ImGui::Text("Work In Progress");
            }
        }
        if (EntitySignature.test(ecs::TYPETEXTCOMPONENT))
        {
            bool open = ImGui::CollapsingHeader("Text Component");

            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Delete Component")) {
                    ecs->m_RemoveComponent(ecs::TYPETEXTCOMPONENT, m_clickedEntityId);
                }
                ImGui::EndPopup();
            }

            if (open)
            {
                // Retrieve the TransformComponent
                ecs::TextComponent* tc = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]
                    ->m_GetEntityComponent(entityID));


                ImVec4 color = ImVec4(tc->m_red, tc->m_green, tc->m_blue, 255.0f / 255.0f);
                // ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

                 //Display Position
                ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
                ImGui::Text("Text: ");
                ImGui::SameLine(slider_start_pos_x);
                ImGui::SetNextItemWidth(100.0f);
                ImGui::InputText("##TEXT##", &tc->m_text);

                ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
                ImGui::Text("Size");
                ImGui::SameLine(slider_start_pos_x);
                ImGui::SetNextItemWidth(100.0f);
                ImGui::DragFloat("###TEXTXXX", &tc->m_fontSize, 0.02f, 0.f, 10.0f, "%.2f");

                ImGui::Text("Color");
                ImGui::SameLine();
                if (ImGui::ColorEdit3("##MyColor1", (float*)&color, ImGuiColorEditFlags_DisplayRGB)) {
                    tc->m_red = color.x;
                    tc->m_green = color.y;
                    tc->m_blue = color.z;
                }
            }
        }
        
        if (EntitySignature.test(ecs::TYPEANIMATIONCOMPONENT))
        {
            bool open = ImGui::CollapsingHeader("Animation Component");

            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Delete Component")) {
                    ecs->m_RemoveComponent(ecs::TYPEANIMATIONCOMPONENT, m_clickedEntityId);
                }
                ImGui::EndPopup();
            }
            //retrieve sprite component


            if (open) {

                ecs::AnimationComponent* ac = static_cast<ecs::AnimationComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEANIMATIONCOMPONENT]
                    ->m_GetEntityComponent(entityID));
                graphicpipe::GraphicsPipe* graphicsPipe = graphicpipe::GraphicsPipe::m_funcGetInstance();

                ImGui::AlignTextToFramePadding();
                ImGui::Text("Toggle Animation");
                ImGui::SameLine(slider_start_pos_x + 40);
                ImGui::Checkbox("####xx##", &ac->m_isAnimating);

            
               ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
               ImGui::Text("Frames Per Second: ");
               ImGui::SameLine(slider_start_pos_x + 100);
               ImGui::SetNextItemWidth(100.0f);
               ImGui::DragInt("##SPF##", &ac->m_framesPerSecond, 1, 1, 60, "%d");

               ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
               ImGui::Text("Frame Timer");
               ImGui::SameLine(slider_start_pos_x);
               ImGui::SetNextItemWidth(100.0f);
               ImGui::DragFloat("###TEXTXXX", &ac->m_frameTimer, 0.02f, 0.f, 1.f, "%.2f");
            }
        }
        if (EntitySignature.test(ecs::TYPECAMERACOMPONENT))
        {
            bool open = ImGui::CollapsingHeader("Camera Component");

            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Delete Component")) {
                    ecs->m_RemoveComponent(ecs::TYPECAMERACOMPONENT, m_clickedEntityId);
                }
                ImGui::EndPopup();
            }

            if (open)
            {
                // Retrieve the TransformComponent
                ecs::CameraComponent* cc = static_cast<ecs::CameraComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPECAMERACOMPONENT]
                    ->m_GetEntityComponent(entityID));




            }

        }
     }
    ImGui::End();
}   