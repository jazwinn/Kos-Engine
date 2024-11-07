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
#include "../Config/pch.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"
#include "imgui_stdlib.h"
#include "../ECS/ECS.h"
#include "../Asset Manager/AssetManager.h"
#include "../Graphics/GraphicsPipe.h"
#include "../ECS/Layers.h"

#include "ScriptVariable.h"

#pragma warning(push)
#pragma warning(disable : 26495)  // Disable uninitialized variable warning
#include "../Dependencies/rapidjson/document.h"
#include "../Dependencies/rapidjson/writer.h"
#include "../Dependencies/rapidjson/filereadstream.h"
#pragma warning(pop)

#include<vector>
#include<string>
#include <iostream>
#include <filesystem>


static const float slider_start_pos_x = 100.0f; //Padding for the slider
static const float slidersize = 50.f;

template <typename T>
struct DrawComponents {

    T m_Array;
    int count{};
    


    void operator()(float& _args) {
        
        ImGui::AlignTextToFramePadding();
        ImGui::Text(m_Array[count].c_str());
        ImGui::SameLine(slider_start_pos_x);
        ImGui::SetNextItemWidth(100.0f);
        std::string title = "##" + m_Array[count];
        ImGui::PushItemWidth(slidersize);
        ImGui::DragFloat(title.c_str(), &_args, 0.1f, -1000.0f, 1000.f, "%.2f");
        ImGui::PopItemWidth();
        count++;
    }


    void operator()(const std::vector<std::string>& _args) {
        for (auto& arg : _args) {
            ImGui::Text(m_Array[count].c_str());
            ImGui::SameLine(slider_start_pos_x);
            std::string title = "##" + m_Array[count];
           // ImGui::InputText(title.c_str(), arg.c_str());
        }

        count++;
    }

    void operator()(physicspipe::EntityType& _args)
    {
        const char* shapeName = (_args == physicspipe::EntityType::CIRCLE) ? "CIRCLE" : "RECTANGLE";
        if (ImGui::BeginCombo("Shape Types", shapeName))
        {
            if (ImGui::Selectable("RECTANGLE")) 
            {
                _args = physicspipe::EntityType::RECTANGLE;
            }
            if (ImGui::Selectable("CIRCLE")) 
            {
                _args = physicspipe::EntityType::CIRCLE;
            }
            ImGui::EndCombo();
        }
        count++;
    }

    void operator()(int& _args) {
        
        ImGui::AlignTextToFramePadding();
        ImGui::Text(m_Array[count].c_str());
        ImGui::SameLine(slider_start_pos_x);
        ImGui::SetNextItemWidth(100.0f);
        std::string title = "##" + m_Array[count];
        ImGui::PushItemWidth(slidersize);
        ImGui::DragInt(title.c_str(), &_args, 1.0f, -100, 100);
        ImGui::PopItemWidth();
        count++;
    }

    void operator()(vector2::Vec2& _args) {
        
        ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
        ImGui::Text(m_Array[count].c_str());
        ImGui::SameLine(slider_start_pos_x);
        ImGui::SetNextItemWidth(100.0f);
        ImGui::PushItemWidth(slidersize);
        std::string title = "X##" + m_Array[count];
        ImGui::DragFloat(title.c_str(), &_args.m_x, 0.02f, -50.f, 50.f, "%.2f");

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100.0f);
        title = "Y##" + m_Array[count];
        ImGui::PushItemWidth(slidersize);
        ImGui::DragFloat(title.c_str(), &_args.m_y, 0.02f, -50.0f, 50.0f, "%.2f");
        
        ImGui::PopItemWidth();
        ImGui::PopItemWidth();

        count++;
    }

    void operator()( vector3::Vec3& _args) {
        ImGui::PushItemWidth(slidersize);
        ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
        ImGui::Text(m_Array[count].c_str());
        ImGui::SameLine(slider_start_pos_x);
        ImGui::SetNextItemWidth(100.0f);
        std::string title = "X##" + m_Array[count];
        ImGui::PushItemWidth(slidersize);
        ImGui::DragFloat(title.c_str(), &_args.m_x, 0.02f, -50.f, 50.f, "%.2f");

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100.0f);
        title = "Y##" + m_Array[count];
        ImGui::PushItemWidth(slidersize);
        ImGui::DragFloat(title.c_str(), &_args.m_y, 0.02f, -50.0f, 50.0f, "%.2f");

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100.0f);
        title = "Z##" + m_Array[count];
        ImGui::PushItemWidth(slidersize);
        ImGui::DragFloat(title.c_str(), &_args.m_y, 0.02f, -50.0f, 50.0f, "%.2f");

        ImGui::PopItemWidth();
        ImGui::PopItemWidth();
        ImGui::PopItemWidth();
        count++;
    }


    void operator()(bool& _args) {
        ImGui::Text(m_Array[count].c_str());
        ImGui::SameLine(slider_start_pos_x);
        std::string title = "##" + m_Array[count];
        ImGui::Checkbox(title.c_str(), &_args);

        count++;
    }

    void operator()(std::string& _args) {
        ImGui::Text(m_Array[count].c_str());
        ImGui::SameLine(slider_start_pos_x);
        std::string title = "##" + m_Array[count];
        ImGui::InputText(title.c_str(), &_args);

        count++;
    }
};




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
        "Add Components", "Collider Component", "Sprite Component", "Player Component", "Rigid Body Component", "Text Component", "Animation Component", "Camera Component" , "Button Component" , "Script Component"
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
            if (ComponentType == 8) {
                ecs->m_AddComponent(ecs::TYPEBUTTONCOMPONENT, entityID);
                ComponentType = 0;
            }
            if (ComponentType == 9) {
                ecs->m_AddComponent(ecs::TYPESCRIPTCOMPONENT, entityID);
                ComponentType = 0;
            }
        }

        auto deletecontext = [](ecs::ComponentType Type, ecs::EntityID ID) {
            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Delete Component")) {
                    ecs::ECS::m_GetInstance()->m_RemoveComponent(Type, ID);
                }
                ImGui::EndPopup();
            }
        };

        static std::map<int, std::pair<ecs::EntityID, bool>> layerMap; // Bool represents whether it's a sprite or a text
        layerMap.clear();

        for (const auto& scene : ecs->m_ECS_SceneMap)
        {
            if (scene.second.m_isActive)
            {
                for (const auto& id : scene.second.m_sceneIDs)
                {
                    if (ecs->m_ECS_EntityMap[id].test(ecs::TYPESPRITECOMPONENT))
                    {
                        ecs::SpriteComponent* sprite = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(id));
                        const int maxLayer = 99;
                        int layer = sprite->m_layer;
                        if (layer >= maxLayer)
                        {
                            layer = maxLayer;
                            while (layerMap.find(--layer) != layerMap.end());
                        }
                        else if (layerMap.find(sprite->m_layer) != layerMap.end())
                        {
                            while (layerMap.find(++layer) != layerMap.end())
                            {

                            }
                        }
                        sprite->m_layer = layer;
                        layerMap[layer] = { id , true };
                    }
                    if (ecs->m_ECS_EntityMap[id].test(ecs::TYPETEXTCOMPONENT))
                    {
                        ecs::TextComponent* text = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(id));
                        const int maxLayer = 99;
                        int layer = text->m_fontLayer;
                        if (layer >= maxLayer)
                        {
                            layer = maxLayer;
                            while (layerMap.find(--layer) != layerMap.end());
                        }
                        else if (layerMap.find(text->m_fontLayer) != layerMap.end())
                        {
                            while (layerMap.find(++layer) != layerMap.end())
                            {

                            }
                        }
                        text->m_fontLayer = layer;
                        layerMap[layer] = { id , false };
                    }
                }
            }


        }


        ecs::compSignature EntitySignature = ecs->m_ECS_EntityMap[entityID];
        assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();
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
            const char* layers[] = { ecs->m_layersStack.m_layerMap[layer::DEFAULT].first.c_str(), ecs->m_layersStack.m_layerMap[layer::LAYER1].first.c_str(),ecs->m_layersStack.m_layerMap[layer::LAYER2].first.c_str(),
                                  ecs->m_layersStack.m_layerMap[layer::LAYER3].first.c_str(), ecs->m_layersStack.m_layerMap[layer::LAYER4].first.c_str(), ecs->m_layersStack.m_layerMap[layer::LAYER5].first.c_str(),
                                  ecs->m_layersStack.m_layerMap[layer::LAYER6].first.c_str(), ecs->m_layersStack.m_layerMap[layer::LAYER7].first.c_str(), ecs->m_layersStack.m_layerMap[layer::LAYER8].first.c_str() };
            int layer_current = nc->m_Layer;
            if (ImGui::Combo("Layers", &layer_current, layers, IM_ARRAYSIZE(layers))) {
                ecs->m_layersStack.m_SwapEntityLayer((layer::LAYERS)layer_current, nc->m_Layer, entityID);

            }

            //create overwrite button for prefab
            if (nc->m_isPrefab && !m_prefabSceneMode) {
                auto* tc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entityID));
                if (!tc->m_haveParent || !static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(tc->m_parentID))->m_isPrefab) {
                    if (ImGui::Button("Overwrite")) {

                        prefab::Prefab::m_OverWriteScenePrafab(entityID);

                    }
                    ImGui::SameLine();

                    if (ImGui::Checkbox("Sync", &nc->m_syncPrefab)) {

                        prefab::Prefab::m_UpdateAllPrefabEntity(nc->m_prefabName);
                    }
                }
                
                static const char* buf = nc->m_prefabName.c_str();
                ImGui::InputText("##readonlytext", (char*)buf, strlen(buf), ImGuiInputTextFlags_ReadOnly);
            }



            bool open;

            //TODO find better way to implement
            if (EntitySignature.test(ecs::TYPETRANSFORMCOMPONENT)) {

                if (ImGui::CollapsingHeader("Transform Component")) {
                    auto* rbc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entityID));
                    rbc->ApplyFunction(DrawComponents(rbc->Names()));
                    
                }


            }
            if (EntitySignature.test(ecs::TYPESPRITECOMPONENT)){

                open = ImGui::CollapsingHeader("Sprite Component");

                deletecontext(ecs::TYPESPRITECOMPONENT, entityID);

                if (open) {
                   
                    assetmanager::AssetManager* Asset = assetmanager::AssetManager::m_funcGetInstance();
                    auto* sc = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(entityID));

                   
                        
                    if (ImGui::BeginCombo("Images", sc->m_imageFile.c_str()))
                    {
                        for (const auto& image : Asset->m_imageManager.m_imageMap) {

                            if (ImGui::Selectable(image.first.c_str())) {
                                sc->m_imageFile = image.first.c_str();
                            }
                        }
                        ImGui::EndCombo();
                    }

                    if (ImGui::BeginDragDropTarget())
                    {

                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("file"))
                        {
                            IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
                            std::filesystem::path* filename = static_cast<std::filesystem::path*>(payload->Data);
                            if (filename->filename().extension().string() == ".png") {
                                if (Asset->m_imageManager.m_imageMap.find(filename->filename().string()) == Asset->m_imageManager.m_imageMap.end()) {
                                    LOGGING_WARN("File not loaded, please reload content browser");
                                }
                                else {
                                    sc->m_imageFile = filename->filename().string();
                                }
                            }
                            else {
                                LOGGING_WARN("Wrong File Type");
                            }



                        }
                        ImGui::EndDragDropTarget();
                    }

                    ImVec4 color = ImVec4(sc->m_color.m_x, sc->m_color.m_y, sc->m_color.m_z, sc->m_alpha);

                    ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
                    ImGui::Text("Color");
                    ImGui::SameLine();
                    if (ImGui::ColorEdit3("##MyColor2", (float*)&color, ImGuiColorEditFlags_DisplayRGB))
                    {
                        sc->m_color.m_x = color.x;
                        sc->m_color.m_y = color.y;
                        sc->m_color.m_z = color.z;
                    }


                    if (open) {
                        auto* rbc = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(entityID));
                        rbc->ApplyFunction(DrawComponents(rbc->Names()));
                    }

               
                    

                    if (ImGui::TreeNode("Image Layers"))
                    {
                        //int count = 0;
                        for (auto it = layerMap.begin(); it != layerMap.end(); ++it) 
                        {
                            ecs::NameComponent* namec = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(it->second.first));
                            std::string entityIDS = std::to_string(it->second.first);
                            if (it->second.second)
                            {
                                entityIDS += " (Sprite)";
                            }
                            else
                            {
                                entityIDS += " (Font)";
                            }

                            std::string selectable = namec->m_entityName + "_ID" + entityIDS;
                            if (it->second.first == static_cast<unsigned>(m_clickedEntityId))
                            {
                                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 0.8f, 0.f, 1.0f));
                                selectable += "(Active)";
                                ImGui::Selectable(selectable.c_str());
                                ImGui::PopStyleColor();
                            }
                            else
                            {
                                ImGui::Selectable(selectable.c_str());
                            }
                            
                          /*  std::ostringstream display;
                            display << selectable << std::setw(30 - selectable.length()) << std::right << "Layer" + std::to_string(spriteLayer);*/
                            

                            if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
                            {
                                decltype(it) n_next = (ImGui::GetMouseDragDelta(0).y < 0.f ? std::prev(it) : std::next(it));
                                if (n_next != layerMap.end())
                                {
                                    if (n_next->second.second && it->second.second)
                                    {
                                        ecs::SpriteComponent* sprite1 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(it->second.first));
                                        ecs::SpriteComponent* sprite2 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(n_next->second.first));
                                        int layer = sprite1->m_layer;
                                        sprite1->m_layer = sprite2->m_layer;
                                        sprite2->m_layer = layer;
                                    }
                                    else if (!n_next->second.second && !it->second.second)
                                    {
                                        ecs::TextComponent* text1 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(it->second.first));
                                        ecs::TextComponent* text2 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(n_next->second.first));
                                        int layer = text1->m_fontLayer;
                                        text1->m_fontLayer = text2->m_fontLayer;
                                        text2->m_fontLayer = layer;
                                    }
                                    else if (n_next->second.second && !it->second.second)
                                    {
                                        ecs::SpriteComponent* sprite1 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(n_next->second.first));
                                        ecs::TextComponent* text1 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(it->second.first));
                                        int layer = text1->m_fontLayer;
                                        text1->m_fontLayer = sprite1->m_layer;
                                        sprite1->m_layer = layer;
                                    }
                                    else
                                    {
                                        ecs::SpriteComponent* sprite1 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(it->second.first));
                                        ecs::TextComponent* text1 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(n_next->second.first));
                                        int layer = text1->m_fontLayer;
                                        text1->m_fontLayer = sprite1->m_layer;
                                        sprite1->m_layer = layer;
                                    }
                                   
                                   
                                    ImGui::ResetMouseDragDelta();
                                }
                            }
                           
                        }
                        ImGui::TreePop();
                    }
                  
                }


            }
            if (EntitySignature.test(ecs::TYPECOLLIDERCOMPONENT)) {

                open = ImGui::CollapsingHeader("Collider Component");

                deletecontext(ecs::TYPECOLLIDERCOMPONENT, entityID);

                if (open) {
                    auto* rbc = static_cast<ecs::ColliderComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(entityID));
                    rbc->ApplyFunction(DrawComponents(rbc->Names()));
                }


            }
            if (EntitySignature.test(ecs::TYPERIGIDBODYCOMPONENT)) {

                open = ImGui::CollapsingHeader("RigidBody Component");

                deletecontext(ecs::TYPERIGIDBODYCOMPONENT, entityID);

                if (open) {
                    auto* rbc = static_cast<ecs::RigidBodyComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(entityID));
                    rbc->ApplyFunction(DrawComponents(rbc->Names()));
                }


            }
            if (EntitySignature.test(ecs::TYPEPLAYERCOMPONENT)) {

                open = ImGui::CollapsingHeader("Player Component");

                deletecontext(ecs::TYPEPLAYERCOMPONENT, entityID);

                if (open) {
                    auto* rbc = static_cast<ecs::PlayerComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEPLAYERCOMPONENT]->m_GetEntityComponent(entityID));
                    rbc->ApplyFunction(DrawComponents(rbc->Names()));
                }


            }
            if (EntitySignature.test(ecs::TYPETEXTCOMPONENT)) {

                open = ImGui::CollapsingHeader("Text Component");

                deletecontext(ecs::TYPETEXTCOMPONENT, entityID);


                if (open) {

                    assetmanager::AssetManager* Asset = assetmanager::AssetManager::m_funcGetInstance();
                    auto* tc = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(entityID));

                    ImVec4 color = ImVec4(tc->m_color.m_x, tc->m_color.m_y, tc->m_color.m_z, 255.0f / 255.0f);

                    ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
                    ImGui::Text("Text: ");
                    ImGui::SameLine(slider_start_pos_x);
                    ImGui::SetNextItemWidth(100.0f);
                    ImGui::InputText("##TEXT##", &tc->m_text);
                    ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
                    ImGui::Text("Size");
                    ImGui::SameLine(slider_start_pos_x);
                    ImGui::SetNextItemWidth(100.0f);
                    ImGui::DragFloat("###TEXTXXX", &tc->m_fontSize, 0.05f, 1.f, 1000.0f, "%.2f");
                    ImGui::Text("Color");
                    ImGui::SameLine();
                    if (ImGui::ColorEdit3("##MyColor1", (float*)&color, ImGuiColorEditFlags_DisplayRGB)) {
                        tc->m_color.m_x = color.x;
                        tc->m_color.m_y = color.y;
                        tc->m_color.m_z = color.z;
                    }
                    if (ImGui::BeginCombo("Fonts", tc->m_fileName.c_str()))
                    {
                        for (const auto& font : Asset->m_fontManager.m_fonts) {
                            if (font.first.empty())continue;
                            if (ImGui::Selectable(font.first.c_str())) {
                                tc->m_fileName = font.first.c_str();
                            }
                        }
                        ImGui::EndCombo();
                    }
                    if (ImGui::BeginDragDropTarget())
                    {

                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("file"))
                        {
                            IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
                            std::filesystem::path* filename = static_cast<std::filesystem::path*>(payload->Data);
                            if (filename->filename().extension().string() == ".ttf") {
                                if (Asset->m_fontManager.m_fonts.find(filename->filename().string()) == Asset->m_fontManager.m_fonts.end()) {
                                    LOGGING_WARN("File not loaded, please reload content browser");
                                }
                                else {
                                    tc->m_fileName = filename->filename().string();
                                }
                            }
                            else {
                                LOGGING_WARN("Wrong File Type");
                            }



                        }
                        ImGui::EndDragDropTarget();
                    }

                    if (ImGui::TreeNode("Font Layers"))
                    {
                        //int count = 0;
                        for (auto it = layerMap.begin(); it != layerMap.end(); ++it)
                        {
                            ecs::NameComponent* namec = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(it->second.first));
                            std::string entityIDS = std::to_string(it->second.first);
                            if (it->second.second)
                            {
                                entityIDS += " (Sprite)";
                            }
                            else
                            {
                                entityIDS += " (Font)";
                            }

                            std::string selectable = namec->m_entityName + "_ID" + entityIDS;
                            if (it->second.first == static_cast<unsigned>(m_clickedEntityId))
                            {
                                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.f, 0.3f, 0.8f, 1.0f));
                                selectable += "(Active)";
                                ImGui::Selectable(selectable.c_str());
                                ImGui::PopStyleColor();
                            }
                            else
                            {
                                ImGui::Selectable(selectable.c_str());
                            }

                            /*  std::ostringstream display;
                              display << selectable << std::setw(30 - selectable.length()) << std::right << "Layer" + std::to_string(spriteLayer);*/


                            if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
                            {
                                decltype(it) n_next = (ImGui::GetMouseDragDelta(0).y < 0.f ? std::prev(it) : std::next(it));
                                if (n_next != layerMap.end())
                                {
                                    if (n_next->second.second && it->second.second)
                                    {
                                        ecs::SpriteComponent* sprite1 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(it->second.first));
                                        ecs::SpriteComponent* sprite2 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(n_next->second.first));
                                        int layer = sprite1->m_layer;
                                        sprite1->m_layer = sprite2->m_layer;
                                        sprite2->m_layer = layer;
                                    }
                                    else if (!n_next->second.second && !it->second.second)
                                    {
                                        ecs::TextComponent* text1 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(it->second.first));
                                        ecs::TextComponent* text2 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(n_next->second.first));
                                        int layer = text1->m_fontLayer;
                                        text1->m_fontLayer = text2->m_fontLayer;
                                        text2->m_fontLayer = layer;
                                    }
                                    else if (n_next->second.second && !it->second.second)
                                    {
                                        ecs::SpriteComponent* sprite1 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(n_next->second.first));
                                        ecs::TextComponent* text1 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(it->second.first));
                                        int layer = text1->m_fontLayer;
                                        text1->m_fontLayer = sprite1->m_layer;
                                        sprite1->m_layer = layer;
                                    }
                                    else
                                    {
                                        ecs::SpriteComponent* sprite1 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(it->second.first));
                                        ecs::TextComponent* text1 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(n_next->second.first));
                                        int layer = text1->m_fontLayer;
                                        text1->m_fontLayer = sprite1->m_layer;
                                        sprite1->m_layer = layer;
                                    }


                                    ImGui::ResetMouseDragDelta();
                                }
                            }

                        }
                        ImGui::TreePop();
                    }
                }

                if (open) {
                    auto* rbc = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(entityID));
                    rbc->ApplyFunction(DrawComponents(rbc->Names()));
                }

            }
            if (EntitySignature.test(ecs::TYPEANIMATIONCOMPONENT)) {

                open = ImGui::CollapsingHeader("Animation Component");

                deletecontext(ecs::TYPEANIMATIONCOMPONENT, entityID);

                if (open) {
                    auto* rbc = static_cast<ecs::AnimationComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEANIMATIONCOMPONENT]->m_GetEntityComponent(entityID));
                    rbc->ApplyFunction(DrawComponents(rbc->Names()));
                }


            }
            if (EntitySignature.test(ecs::TYPECAMERACOMPONENT)) {

                open = ImGui::CollapsingHeader("Camera Component");

                deletecontext(ecs::TYPECAMERACOMPONENT, entityID);

                if (open) {
                    auto* rbc = static_cast<ecs::CameraComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPECAMERACOMPONENT]->m_GetEntityComponent(entityID));
                    rbc->ApplyFunction(DrawComponents(rbc->Names()));
                }
                

            }
            if (EntitySignature.test(ecs::TYPESCRIPTCOMPONENT)) {

                open = ImGui::CollapsingHeader("Script Component");

                deletecontext(ecs::TYPESCRIPTCOMPONENT, entityID);

                if (open) {
                    auto* sc = static_cast<ecs::ScriptComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESCRIPTCOMPONENT]->m_GetEntityComponent(entityID));

                    for (const auto& scriptname : sc->m_scripts)
                    {
                        //print out varaibles
                        scripteditor::ScriptEditor::DisplayScriptComponents(scriptname, entityID);
                    }

                    
                    if (ImGui::BeginListBox("Scripts"))
                    {
                        int n{};
                        for (const auto& scriptname : sc->m_scripts)
                        {


                            ImGui::Selectable(scriptname.c_str());
                            if (ImGui::BeginPopupContextItem()) {
                                if (ImGui::MenuItem("Delete Component")) {
                                    sc->m_scripts.erase(std::find(sc->m_scripts.begin(), sc->m_scripts.end(), scriptname));
                                    ImGui::EndPopup();


                                    break;
                                }
                                ImGui::EndPopup();
                            }


                            if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
                            {
                                int n_next = n + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
                                if (n_next >= 0 && n_next < sc->m_scripts.size())
                                {

                                    std::swap(sc->m_scripts[n], sc->m_scripts[n_next]);
                                    ImGui::ResetMouseDragDelta();
                                }
                            }


                            n++;
                        }
                        ImGui::EndListBox();
                    }
                   
                    std::string preview = "Add Scripts";
                    if (ImGui::BeginCombo("####add scrip", preview.c_str()))
                    {
                        for (const auto& scriptname : assetManager->m_scriptManager.m_CSScripts) {



                            const bool is_selected{};
                            if (ImGui::Selectable(scriptname.c_str(), is_selected)) {
                                //TODO for now push back
                                if (std::find(sc->m_scripts.begin(), sc->m_scripts.end(), scriptname) == sc->m_scripts.end()) {
                                    sc->m_scripts.push_back(scriptname);
                                }
                                else {
                                    LOGGING_WARN("Script is already inside Object");
                                }
                                

                            }

                            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }

                        ImGui::EndCombo();
                    }


                }


            }

            if (EntitySignature.test(ecs::TYPEBUTTONCOMPONENT)) {

                open = ImGui::CollapsingHeader("Button Component");

                deletecontext(ecs::TYPEBUTTONCOMPONENT, entityID);

                if (open) {
                    //auto* rbc = static_cast<ecs::ButtonComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEBUTTONCOMPONENT]->m_GetEntityComponent(entityID));
                    //rbc->ApplyFunction(DrawComponents(rbc->Names()));
                }


            }

            //draw invinsible box
            if (ImGui::GetContentRegionAvail().x > 0 && ImGui::GetContentRegionAvail().y > 0){
                
                ImGui::InvisibleButton("##Invinsible", ImVec2{ ImGui::GetContentRegionAvail().x,ImGui::GetContentRegionAvail().y });

                if (ImGui::BeginDragDropTarget())
                {

                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("file"))
                    {
                        IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
                        std::filesystem::path* filename = static_cast<std::filesystem::path*>(payload->Data);

                        if (filename->filename().extension().string() == ".png") {

                            if (!EntitySignature.test(ecs::TYPESPRITECOMPONENT)) {// does not have sprite component, create one
                                ecs::SpriteComponent* com = static_cast<ecs::SpriteComponent*>(ecs->m_AddComponent(ecs::TYPESPRITECOMPONENT, entityID));
                                com->m_imageFile = filename->filename().string();
                            }
                            else {
                                auto* sc = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(entityID));
                                sc->m_imageFile = filename->filename().string();
                            }
                        }

                        if (filename->filename().extension().string() == ".ttf") {

                            if (!EntitySignature.test(ecs::TYPETEXTCOMPONENT)) {// does not have sprite component, create one
                                ecs::TextComponent* com = static_cast<ecs::TextComponent*>(ecs->m_AddComponent(ecs::TYPETEXTCOMPONENT, entityID));
                                com->m_fileName = filename->filename().string();
                            }
                            else {
                                auto* sc = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(entityID));
                                sc->m_fileName = filename->filename().string();
                            }
                        }

                    }
                    ImGui::EndDragDropTarget();
                }
            }
        }

     }
    ImGui::End();
}   
