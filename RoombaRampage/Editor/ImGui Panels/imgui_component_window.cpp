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
#include "Editor.h"
#include "imgui_stdlib.h"
#include "../ECS/ECS.h"
#include "../Asset Manager/AssetManager.h"
#include "../Graphics/GraphicsPipe.h"
#include "../ECS/Layers.h"
#include "../Editor/TilemapCalculations.h"
#include "../Events/DragFloat.h"

#include "ScriptVariable.h"
#include "../Editor/WindowFile.h"
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
#include <type_traits>
#include <typeinfo>


static const float slider_start_pos_x = 100.0f; //Padding for the slider
static const float slidersize = 50.f;

template <typename T>
struct DrawComponents {

    T m_Array;
    int count{};
    


    bool operator()(float& _args) {
        
        ImGui::AlignTextToFramePadding();
        ImGui::Text(m_Array[count].c_str());
        ImGui::SameLine(slider_start_pos_x);
        ImGui::SetNextItemWidth(100.0f);
        std::string title = "##" + m_Array[count];
        ImGui::PushItemWidth(slidersize);
        bool changed = false;
        ImGui::DragFloat(title.c_str(), &_args, 0.1f, -1000.0f, 1000.f, "%.2f");
        if (ImGui::IsItemActivated()) {
            changed = true;
        }
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            changed = true;
        }
        ImGui::PopItemWidth();
        count++;
        return changed;
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

    bool operator()(vector2::Vec2& _args) {
        
        ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
        ImGui::Text(m_Array[count].c_str());
        ImGui::SameLine(slider_start_pos_x);
        ImGui::SetNextItemWidth(100.0f);
        ImGui::PushItemWidth(slidersize);
        std::string title = "X##" + m_Array[count];
        bool changed = false;
        ImGui::DragFloat(title.c_str(), &_args.m_x, 0.02f, -50.f, 50.f, "%.2f");
        if(ImGui::IsItemActivated()){
            changed = true;
        }
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            changed = true;
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100.0f);
        title = "Y##" + m_Array[count];
        ImGui::PushItemWidth(slidersize);
        ImGui::DragFloat(title.c_str(), &_args.m_y, 0.02f, -50.0f, 50.0f, "%.2f");
        if (ImGui::IsItemActivated() && !changed) {
            changed = true;
        }
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            changed = true;
        }
        ImGui::PopItemWidth();
        ImGui::PopItemWidth();



        count++;
        return changed;
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
        "Add Components", "Collider Component", "Sprite Component", "Player Component", "Rigid Body Component", "Text Component", 
        "Animation Component", "Camera Component" , "Button Component" , "Script Component", "Tilemap Component", "Audio Component"
    };
    static int ComponentType = 0;



    if (ecs->m_ECS_EntityMap.size() > 0 && m_clickedEntityId >= 0) {

        ecs::EntityID entityID = m_clickedEntityId;
      


        if (ImGui::Combo("##ADDCOMPONENT", &ComponentType, ComponentNames, IM_ARRAYSIZE(ComponentNames), IM_ARRAYSIZE(ComponentNames))) {
            ecs::compSignature EntitySignature = ecs->m_ECS_EntityMap[entityID];
            if (ComponentType == 1) {
                ecs->m_AddComponent(ecs::TYPECOLLIDERCOMPONENT, entityID);
                if (ecs->m_ECS_EntityMap[entityID].test(ecs::TYPESPRITECOMPONENT))
                {
                    graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
                    ecs::ColliderComponent* colCom = static_cast<ecs::ColliderComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(entityID));
                    ecs::SpriteComponent* spriteCom = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(entityID));
                    if (!spriteCom->m_imageFile.empty())
                    {
                        assetmanager::AssetManager* assets = assetmanager::AssetManager::m_funcGetInstance();
                        colCom->m_Size.m_x = static_cast<float>(static_cast<float>(assets->m_imageManager.m_imageMap[spriteCom->m_imageFile].m_width) / static_cast<float>(pipe->m_unitWidth) / assets->m_imageManager.m_imageMap[spriteCom->m_imageFile].m_stripCount);
                        colCom->m_Size.m_y = static_cast<float>(assets->m_imageManager.m_imageMap[spriteCom->m_imageFile].m_height) / static_cast<float>(pipe->m_unitHeight);
                    }
                }
               
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPECOLLIDERCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPECOLLIDERCOMPONENT);
                    DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 2) {
                ecs->m_AddComponent(ecs::TYPESPRITECOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPESPRITECOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPESPRITECOMPONENT);
                    DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 3) {
                ecs->m_AddComponent(ecs::TYPEPLAYERCOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPEPLAYERCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPEPLAYERCOMPONENT);
                    DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 4) {
                ecs->m_AddComponent(ecs::TYPERIGIDBODYCOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPERIGIDBODYCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPERIGIDBODYCOMPONENT);
                    DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 5) {
                ecs->m_AddComponent(ecs::TYPETEXTCOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPETEXTCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPETEXTCOMPONENT);
                    DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 6) {
                ecs->m_AddComponent(ecs::TYPEANIMATIONCOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPEANIMATIONCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPEANIMATIONCOMPONENT);
                    DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 7) {
                ecs->m_AddComponent(ecs::TYPECAMERACOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPECAMERACOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPECAMERACOMPONENT);
                    DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 8) {
                ecs->m_AddComponent(ecs::TYPEBUTTONCOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPEBUTTONCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPEBUTTONCOMPONENT);
                    DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 9) {
                ecs->m_AddComponent(ecs::TYPESCRIPTCOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPESCRIPTCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPESCRIPTCOMPONENT);
                    DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 10) {
                ecs->m_AddComponent(ecs::TYPETILEMAPCOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPETILEMAPCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPETILEMAPCOMPONENT);
                    DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 11) {
                ecs->m_AddComponent(ecs::TYPEAUDIOCOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPEAUDIOCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPEAUDIOCOMPONENT);
                    DISPATCH_ACTION_EVENT(action);
                }
            }
        }

        auto CreateContext = [](ecs::ComponentType Type, ecs::EntityID ID) {
            if (ImGui::BeginPopupContextItem()) {
                if (Type != ecs::TYPETRANSFORMCOMPONENT && ImGui::MenuItem("Delete Component")) {
                    ecs::ECS::m_GetInstance()->m_RemoveComponent(Type, ID);
                }
                if (ImGui::MenuItem("Reset Component")) {
                    if (Type == ecs::TYPETRANSFORMCOMPONENT) {
                        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
                        const auto& tc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
                        tc->m_position = vector2::Vec2{0.f, 0.f};
                        tc->m_rotation = 0.f;
                        tc->m_scale = vector2::Vec2{1.f, 1.f};
                    }
                    else {
                        ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[Type]->m_ResetComponent(ID);
                    }
                    
                    
                }

                ImGui::EndPopup();
            }
        };

        static std::map<int, std::pair<ecs::EntityID, bool>> layerMap; // Bool represents whether it's a sprite or a text
        static bool layerChange{ true };
        static size_t entitySize = ecs->m_ECS_EntityMap.size();
        static std::string sceneChange = m_activeScene;

        if (layerChange || entitySize != ecs->m_ECS_EntityMap.size() || sceneChange != m_activeScene)
        {
            layerChange = false;
            entitySize = ecs->m_ECS_EntityMap.size();
            sceneChange = m_activeScene;
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
                            const int maxLayer = 999;
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

            // Convert vector to array of char* for ImGui
            std::vector<const char*> tag_Names(m_tags.size());
            std::transform(m_tags.begin(), m_tags.end(), tag_Names.begin(), [](const std::string& tag) {
                return tag.c_str();
                });

            int item{};
            const auto& it = std::find(tag_Names.begin(), tag_Names.end(), nc->m_entityTag);
            if (it != tag_Names.end()) {
                item = static_cast<int>(std::distance(tag_Names.begin(), it));
            }
            
            if (ImGui::Combo("Tag", &item, tag_Names.data(), static_cast<int>(tag_Names.size()))) {
                nc->m_entityTag = m_tags[item];
            }

           // std::cout << nc->m_entityTag << std::endl;
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



            static bool open;

            //TODO find better way to implement
            if (EntitySignature.test(ecs::TYPETRANSFORMCOMPONENT)) {
                
                open = ImGui::CollapsingHeader("Transform Component");

                CreateContext(ecs::TYPETRANSFORMCOMPONENT, entityID);

                if (open) {
                    auto* rbc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entityID));
                    DrawComponents toDraw(rbc->Names());
                    static ecs::TransformComponent oldVal = *rbc;
                    if (toDraw(rbc->m_position)) {
                        if ((DragFloat::dragFloatCheck::m_GetInstance()->m_getPrevMem() != DragFloat::Member::POS)) {
                            oldVal = *rbc;
                        }
                        if (DragFloat::dragFloatCheck::m_GetInstance()->m_click(DragFloat::Comp::TRANSFORM, DragFloat::Member::POS)) {
                            events::TransformComponentChanged action(ecs::TYPETRANSFORMCOMPONENT, entityID, rbc, oldVal);
                            DISPATCH_ACTION_EVENT(action);
                            oldVal = *rbc;
                        }
                        
                    }
                    if (toDraw(rbc->m_rotation)) {
                        if ((DragFloat::dragFloatCheck::m_GetInstance()->m_getPrevMem() != DragFloat::Member::ROT)) {
                            oldVal = *rbc;
                        }
                        if (DragFloat::dragFloatCheck::m_GetInstance()->m_click(DragFloat::Comp::TRANSFORM, DragFloat::Member::ROT)) {
                            events::TransformComponentChanged action(ecs::TYPETRANSFORMCOMPONENT, entityID, rbc, oldVal);
                            DISPATCH_ACTION_EVENT(action);
                            oldVal = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_scale)) {
                        if ((DragFloat::dragFloatCheck::m_GetInstance()->m_getPrevMem() != DragFloat::Member::SCALE)) {
                            oldVal = *rbc;
                        }
                        if (DragFloat::dragFloatCheck::m_GetInstance()->m_click(DragFloat::Comp::TRANSFORM, DragFloat::Member::SCALE)) {
                            events::TransformComponentChanged action(ecs::TYPETRANSFORMCOMPONENT, entityID, rbc, oldVal);
                            DISPATCH_ACTION_EVENT(action);
                            oldVal = *rbc;
                        }
                    }
                    
                }


            }
            if (EntitySignature.test(ecs::TYPESPRITECOMPONENT)){

                open = ImGui::CollapsingHeader("Sprite Component");

                CreateContext(ecs::TYPESPRITECOMPONENT, entityID);

                if (open) {
                   
                    assetmanager::AssetManager* Asset = assetmanager::AssetManager::m_funcGetInstance();
                    auto* sc = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(entityID));

                   
                        
                    if (ImGui::BeginCombo("Images", sc->m_imageFile.c_str()))
                    {
                        for (const auto& image : Asset->m_imageManager.m_imageMap) {

                            if (ImGui::Selectable(image.first.c_str())) {
                                sc->m_imageFile = image.first.c_str();
                                if (!ecs->m_ECS_EntityMap[entityID].test(ecs::TYPEANIMATIONCOMPONENT))
                                {
                                    assetmanager::AssetManager* assets = assetmanager::AssetManager::m_funcGetInstance();
                                    //graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
                                    if (assets->m_imageManager.m_imageMap[sc->m_imageFile].m_stripCount > 1)
                                    {
                                       
                                        ecs::AnimationComponent* com = static_cast<ecs::AnimationComponent*>(ecs->m_AddComponent(ecs::TYPEANIMATIONCOMPONENT, entityID));
                                        com->m_stripCount = assets->m_imageManager.m_imageMap[sc->m_imageFile].m_stripCount;
                                        
                                       
                                    }
                                }
                                else 
                                {
                                    assetmanager::AssetManager* assets = assetmanager::AssetManager::m_funcGetInstance();
                                    ecs::AnimationComponent* com = static_cast<ecs::AnimationComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEANIMATIONCOMPONENT]->m_GetEntityComponent(entityID));
                                    com->m_stripCount = assets->m_imageManager.m_imageMap[sc->m_imageFile].m_stripCount;
                                }
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
                            if (filename->filename().extension().string() == ".png" || filename->filename().extension().string() == ".jpg") {
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
                        for (auto it2 = layerMap.begin(); it2 != layerMap.end(); ++it2) 
                        {
                            ecs::NameComponent* namec = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(it2->second.first));
                            std::string entityIDS = std::to_string(it2->second.first);
                            if (it2->second.second)
                            {
                                entityIDS += " (Sprite)";
                            }
                            else
                            {
                                entityIDS += " (Font)";
                            }

                            std::string selectable = namec->m_entityName + "_ID" + entityIDS;
                            if (it2->second.first == static_cast<unsigned>(m_clickedEntityId))
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
                                decltype(it2) n_next = (ImGui::GetMouseDragDelta(0).y < 0.f ? std::prev(it2) : std::next(it2));
                                if (n_next != layerMap.end())
                                {
                                    if (n_next->second.second && it2->second.second)
                                    {
                                        ecs::SpriteComponent* sprite1 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(it2->second.first));
                                        ecs::SpriteComponent* sprite2 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(n_next->second.first));
                                        int layer = sprite1->m_layer;
                                        sprite1->m_layer = sprite2->m_layer;
                                        sprite2->m_layer = layer;
                                    }
                                    else if (!n_next->second.second && !it2->second.second)
                                    {
                                        ecs::TextComponent* text1 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(it2->second.first));
                                        ecs::TextComponent* text2 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(n_next->second.first));
                                        int layer = text1->m_fontLayer;
                                        text1->m_fontLayer = text2->m_fontLayer;
                                        text2->m_fontLayer = layer;
                                    }
                                    else if (n_next->second.second && !it2->second.second)
                                    {
                                        ecs::SpriteComponent* sprite1 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(n_next->second.first));
                                        ecs::TextComponent* text1 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(it2->second.first));
                                        int layer = text1->m_fontLayer;
                                        text1->m_fontLayer = sprite1->m_layer;
                                        sprite1->m_layer = layer;
                                    }
                                    else
                                    {
                                        ecs::SpriteComponent* sprite1 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(it2->second.first));
                                        ecs::TextComponent* text1 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(n_next->second.first));
                                        int layer = text1->m_fontLayer;
                                        text1->m_fontLayer = sprite1->m_layer;
                                        sprite1->m_layer = layer;
                                    }
                                    layerChange = true;
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

                CreateContext(ecs::TYPECOLLIDERCOMPONENT, entityID);

                if (open) {
                    auto* rbc = static_cast<ecs::ColliderComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(entityID));
                    rbc->ApplyFunction(DrawComponents(rbc->Names()));
                }


            }
            if (EntitySignature.test(ecs::TYPERIGIDBODYCOMPONENT)) {

                open = ImGui::CollapsingHeader("RigidBody Component");

                CreateContext(ecs::TYPERIGIDBODYCOMPONENT, entityID);

                if (open) {
                    auto* rbc = static_cast<ecs::RigidBodyComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(entityID));
                    rbc->ApplyFunction(DrawComponents(rbc->Names()));
                }


            }
            if (EntitySignature.test(ecs::TYPEPLAYERCOMPONENT)) {

                open = ImGui::CollapsingHeader("Player Component");

                CreateContext(ecs::TYPEPLAYERCOMPONENT, entityID);

                if (open) {
                    auto* rbc = static_cast<ecs::PlayerComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEPLAYERCOMPONENT]->m_GetEntityComponent(entityID));
                    rbc->ApplyFunction(DrawComponents(rbc->Names()));
                }


            }
            if (EntitySignature.test(ecs::TYPETEXTCOMPONENT)) {

                open = ImGui::CollapsingHeader("Text Component");

                CreateContext(ecs::TYPETEXTCOMPONENT, entityID);


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
                        for (auto it2 = layerMap.begin(); it2 != layerMap.end(); ++it2)
                        {
                            ecs::NameComponent* namec = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(it2->second.first));
                            std::string entityIDS = std::to_string(it2->second.first);
                            if (it2->second.second)
                            {
                                entityIDS += " (Sprite)";
                            }
                            else
                            {
                                entityIDS += " (Font)";
                            }

                            std::string selectable = namec->m_entityName + "_ID" + entityIDS;
                            if (it2->second.first == static_cast<unsigned>(m_clickedEntityId))
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
                                decltype(it2) n_next = (ImGui::GetMouseDragDelta(0).y < 0.f ? std::prev(it2) : std::next(it2));
                                if (n_next != layerMap.end())
                                {
                                    if (n_next->second.second && it2->second.second)
                                    {
                                        ecs::SpriteComponent* sprite1 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(it2->second.first));
                                        ecs::SpriteComponent* sprite2 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(n_next->second.first));
                                        int layer = sprite1->m_layer;
                                        sprite1->m_layer = sprite2->m_layer;
                                        sprite2->m_layer = layer;
                                    }
                                    else if (!n_next->second.second && !it2->second.second)
                                    {
                                        ecs::TextComponent* text1 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(it2->second.first));
                                        ecs::TextComponent* text2 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(n_next->second.first));
                                        int layer = text1->m_fontLayer;
                                        text1->m_fontLayer = text2->m_fontLayer;
                                        text2->m_fontLayer = layer;
                                    }
                                    else if (n_next->second.second && !it2->second.second)
                                    {
                                        ecs::SpriteComponent* sprite1 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(n_next->second.first));
                                        ecs::TextComponent* text1 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(it2->second.first));
                                        int layer = text1->m_fontLayer;
                                        text1->m_fontLayer = sprite1->m_layer;
                                        sprite1->m_layer = layer;
                                    }
                                    else
                                    {
                                        ecs::SpriteComponent* sprite1 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(it2->second.first));
                                        ecs::TextComponent* text1 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(n_next->second.first));
                                        int layer = text1->m_fontLayer;
                                        text1->m_fontLayer = sprite1->m_layer;
                                        sprite1->m_layer = layer;
                                    }
                                    layerChange = true;
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

                CreateContext(ecs::TYPEANIMATIONCOMPONENT, entityID);

                if (open) {
                    auto* rbc = static_cast<ecs::AnimationComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEANIMATIONCOMPONENT]->m_GetEntityComponent(entityID));
                    rbc->ApplyFunction(DrawComponents(rbc->Names()));
                }


            }
            if (EntitySignature.test(ecs::TYPECAMERACOMPONENT)) {

                open = ImGui::CollapsingHeader("Camera Component");

                CreateContext(ecs::TYPECAMERACOMPONENT, entityID);

                if (open) {
                    auto* rbc = static_cast<ecs::CameraComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPECAMERACOMPONENT]->m_GetEntityComponent(entityID));
                    rbc->ApplyFunction(DrawComponents(rbc->Names()));
                }
                

            }
            if (EntitySignature.test(ecs::TYPESCRIPTCOMPONENT)) {

                open = ImGui::CollapsingHeader("Script Component");

                CreateContext(ecs::TYPESCRIPTCOMPONENT, entityID);

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
                                if (ImGui::MenuItem("Delete Script")) {
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
                            if (ImGui::Selectable(scriptname.first.c_str(), is_selected)) {
                                //TODO for now push back
                                if (std::find(sc->m_scripts.begin(), sc->m_scripts.end(), scriptname.first) == sc->m_scripts.end()) {
                                    sc->m_scripts.push_back(scriptname.first);
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

                CreateContext(ecs::TYPEBUTTONCOMPONENT, entityID);

                if (open) {
                    auto* rbc = static_cast<ecs::ButtonComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEBUTTONCOMPONENT]->m_GetEntityComponent(entityID));
                    rbc->ApplyFunction(DrawComponents(rbc->Names()));
                }


            }

            if (EntitySignature.test(ecs::TYPETILEMAPCOMPONENT)) {

                open = ImGui::CollapsingHeader("Tilemap Component");

                CreateContext(ecs::TYPETILEMAPCOMPONENT, entityID);

                if (open) {

                    assetmanager::AssetManager* Asset = assetmanager::AssetManager::m_funcGetInstance();
                    auto* tmc = static_cast<ecs::TilemapComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETILEMAPCOMPONENT]->m_GetEntityComponent(entityID));
                    //auto* transform = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entityID));



                    if (ImGui::BeginCombo("Tilemaps", tmc->m_tilemapFile.c_str()))
                    {
                        for (const auto& image : Asset->m_imageManager.m_imageMap) {

                            if (ImGui::Selectable(image.first.c_str())) {
                                tmc->m_tilemapFile = image.first.c_str();
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
                            if (filename->filename().extension().string() == ".png" || filename->filename().extension().string() == ".jpg") {
                                if (Asset->m_imageManager.m_imageMap.find(filename->filename().string()) == Asset->m_imageManager.m_imageMap.end()) {
                                    LOGGING_WARN("File not loaded, please reload content browser");
                                }
                                else {
                                    tmc->m_tilemapFile = filename->filename().string();
                                }
                            }
                            else {
                                LOGGING_WARN("Wrong File Type");
                            }
                        }
                        ImGui::EndDragDropTarget();
                    }

                    ImVec4 color = ImVec4(tmc->m_color.m_x, tmc->m_color.m_y, tmc->m_color.m_z, tmc->m_alpha);

                    ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
                    ImGui::Text("Color");
                    ImGui::SameLine();
                    if (ImGui::ColorEdit3("##MyColor3", (float*)&color, ImGuiColorEditFlags_DisplayRGB))
                    {
                        tmc->m_color.m_x = color.x;
                        tmc->m_color.m_y = color.y;
                        tmc->m_color.m_z = color.z;
                    }


                    if (open) {
                        auto* rbc = static_cast<ecs::TilemapComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETILEMAPCOMPONENT]->m_GetEntityComponent(entityID));
                        rbc->ApplyFunction(DrawComponents(rbc->Names()));
                    }
                    Tilemap::resizeTiles(tmc, tmc->m_rowLength, tmc->m_columnLength);

                    if (ImGui::Button("Pick Tile"))
                    {
                        m_tilePickerMode = true;
                    }
                    //Tilemap::debugTileIndex(tmc);

                    //std::cout << EditorCamera::calculateWorldCoordinatesFromMouse(ImGui::GetMousePos().x, ImGui::GetMousePos().y).m_y << std::endl;

                }


            }
            if (EntitySignature.test(ecs::TYPEAUDIOCOMPONENT)) {
                open = ImGui::CollapsingHeader("Audio Component");

                CreateContext(ecs::TYPEAUDIOCOMPONENT, entityID);

                if (open) {
                    auto* ac = static_cast<ecs::AudioComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEAUDIOCOMPONENT]->m_GetEntityComponent(entityID));
                    //assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();

                    if (ac) {
                        int fileIndex = 0;
                        for (auto it2 = ac->m_AudioFiles.begin(); it2 != ac->m_AudioFiles.end();) {
                            ImGui::PushID(fileIndex);

                            bool removeFile = false;
                            

                            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
                            std::string headerName = "Audio File " + std::to_string(fileIndex + 1) + ": " + it2->m_Name;
                            bool nodeOpen = ImGui::TreeNodeEx(headerName.c_str(), flags);

                            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                                ImGui::OpenPopup("AudioContextMenu");
                            }

                            if (ImGui::BeginPopup("AudioContextMenu")) {
                                if (ImGui::MenuItem("Remove")) {
                                    removeFile = true;
                                }
                                ImGui::EndPopup();
                            }

                            if (nodeOpen) {
                                char buffer[256];
                                strncpy(buffer, it2->m_FilePath.c_str(), sizeof(buffer));


                                if (ImGui::BeginCombo("Sounds", it2->m_Name.c_str())) {
                                    for (const auto& sound : assetManager->m_audioManager.getSoundMap()) {
                                        if (ImGui::Selectable(sound.first.c_str(), sound.first == it->m_Name)) {
                                            if (sound.first != it->m_Name) {
                                                auto& audioManager = assetManager->m_audioManager;

                                                if (audioManager.m_IsPlayingForEntity(entityID, it->m_Name)) {
                                                    audioManager.m_StopAudioForEntity(entityID, it->m_Name);
                                                }

                                                if (audioManager.getSoundMap().find(sound.first) == audioManager.getSoundMap().end()) {
                                                    LOGGING_WARN("Selected sound not found in the audio manager.");
                                                    continue;
                                                }

                                                it->m_Name = sound.first;
                                            }
                                        }
                                    }
                                    ImGui::EndCombo();
                                }


                                if (ImGui::BeginDragDropTarget()) {
                                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("file")) {
                                        IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
                                        std::filesystem::path* filename = static_cast<std::filesystem::path*>(payload->Data);
                                        if (filename->filename().extension().string() == ".ogg" || ".wav") {
                                            if (assetManager->m_audioManager.getSoundMap().find(filename->filename().string()) == assetManager->m_audioManager.getSoundMap().end()) {
                                                LOGGING_WARN("File not loaded, please reload content browser");
                                            }
                                            else {
                                                assetManager->m_LoadAudio(filename->filename().string());
                                            }
                                        }
                                        else {
                                            LOGGING_WARN("Wrong File Type");
                                        }
                                    }
                                    ImGui::EndDragDropTarget();
                                }

                                ImGui::SliderFloat("Volume", &it->m_Volume, 0.0f, 1.0f);
                                assetManager->m_audioManager.m_SetVolumeForEntity(entityID, it->m_Name, it->m_Volume);


                                bool wasLooping = it2->m_Loop;

                                if (ImGui::Button("Play Sound")) {
                                    std::string key = it2->m_Name;
                                    auto& audioManager = assetManager->m_audioManager;
                                    if (!audioManager.m_IsPlayingForEntity(entityID, key)) {
                                        audioManager.m_PlayAudioForEntity(entityID, key, it->m_Volume);
                                    }
                                    else {
                                        audioManager.m_StopAudioForEntity(entityID, it->m_Name);
                                        audioManager.m_PlayAudioForEntity(entityID, key, it->m_Volume);
                                    }
                                }
    
                                if (ImGui::Checkbox("Play On Start", &it2->m_PlayOnStart)) {
                                    auto& audioManager = assetManager->m_audioManager;

                                    if (it2->m_PlayOnStart) {
                                        for (auto& audioFile : ac->m_AudioFiles) {
                                            if (&audioFile != &(*it2)) {
                                                audioFile.m_PlayOnStart = false;
                                            }
                                        }

                                        audioManager.m_StopAudioForEntity(entityID, it->m_Name);

                                    }
                                    else {
                                        audioManager.m_StopAudioForEntity(entityID, it->m_Name);
                                    }

                                    audioManager.m_SetPlayOnStartForEntity(entityID, it->m_Name, it->m_PlayOnStart);
                                }

                                if (ImGui::Checkbox("Loop", &it2->m_Loop)) {
                                    auto& audioManager = assetManager->m_audioManager;

                                    audioManager.m_SetLoopingForEntity(entityID, it->m_Name, it->m_Loop);

                                    if (it->m_Loop != wasLooping && audioManager.m_IsPlayingForEntity(entityID, it->m_Name)) {
                                        audioManager.m_StopAudioForEntity(entityID, it->m_Name);
                                        audioManager.m_PlayAudioForEntity(entityID, it->m_Name, it->m_Volume);
                                    }
                                }




                                bool isPaused = false;
                                auto& audioManager2 = assetManager->m_audioManager;
                                if (audioManager2.getSoundMap().find(it2->m_Name) != audioManager2.getSoundMap().end()) {
                                    auto& sound = audioManager2.getSoundMap()[it2->m_Name];

                                    if (sound->m_GetChannelForEntity(entityID)) {
                                        sound->m_GetChannelForEntity(entityID)->getPaused(&isPaused);
                                    }

                                    if (ImGui::Checkbox("Pause Sound", &isPaused)) {
                                        if (isPaused) {
                                            audioManager.m_PauseAudioForEntity(entityID, it->m_Name);
                                        }
                                        else {
                                            audioManager.m_UnpauseAudioForEntity(entityID, it->m_Name);
                                        }
                                    }
                                }

                                if (ImGui::Button("Stop Sound")) {
                                    std::string key = it->m_Name;
                                    auto& audioManager = assetManager->m_audioManager;
                                    audioManager.m_StopAudioForEntity(entityID, key);
                                }



                                ImGui::TreePop();
                            }

                            ImGui::PopID();

                            if (removeFile) {
                                std::string key = it2->m_Name;
                                auto& audioManager = assetManager->m_audioManager;
                                audioManager.m_StopAudioForEntity(entityID, key);
                                it = ac->m_AudioFiles.erase(it);
                            }
                            else {
                                ++it2;
                                ++fileIndex;
                            }
                        }

                        ImGui::Separator();

                        static char newAudioName[256] = "";
                        if (ImGui::Button("Add Audio File")) {
                            ac->m_AudioFiles.emplace_back();
                        }

                        if (ImGui::Button("Stop All Sounds"))
                        {
                            auto& audioManager = assetManager->m_audioManager;
                            audioManager.m_StopAllSounds();
                        }
                    }
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

                        if (filename->filename().extension().string() == ".png" || filename->filename().extension().string() == ".jpg") {

                            if (!EntitySignature.test(ecs::TYPESPRITECOMPONENT)) {// does not have sprite component, create one
                                ecs::SpriteComponent* com = static_cast<ecs::SpriteComponent*>(ecs->m_AddComponent(ecs::TYPESPRITECOMPONENT, entityID));
                                com->m_imageFile = filename->filename().string();
                                if (!ecs->m_ECS_EntityMap[entityID].test(ecs::TYPEANIMATIONCOMPONENT))
                                {
                                    assetmanager::AssetManager* assets = assetmanager::AssetManager::m_funcGetInstance();
                                    //graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
                                    if (assets->m_imageManager.m_imageMap[com->m_imageFile].m_stripCount > 1)
                                    {
                                        ecs::AnimationComponent* ani = static_cast<ecs::AnimationComponent*>(ecs->m_AddComponent(ecs::TYPEANIMATIONCOMPONENT, entityID));
                                        ani->m_stripCount = assets->m_imageManager.m_imageMap[com->m_imageFile].m_stripCount;
                                    }
                                }
                                else
                                {
                                    assetmanager::AssetManager* assets = assetmanager::AssetManager::m_funcGetInstance();
                                    ecs::AnimationComponent* ani = static_cast<ecs::AnimationComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEANIMATIONCOMPONENT]->m_GetEntityComponent(entityID));
                                    ani->m_stripCount = assets->m_imageManager.m_imageMap[com->m_imageFile].m_stripCount;
                                }
                            }
                            else {
                                auto* sc = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(entityID));
                                sc->m_imageFile = filename->filename().string();
                                if (!ecs->m_ECS_EntityMap[entityID].test(ecs::TYPEANIMATIONCOMPONENT))
                                {
                                    assetmanager::AssetManager* assets = assetmanager::AssetManager::m_funcGetInstance();
                                    //graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
                                    if (assets->m_imageManager.m_imageMap[sc->m_imageFile].m_stripCount > 1)
                                    {
                                        ecs::AnimationComponent* ani = static_cast<ecs::AnimationComponent*>(ecs->m_AddComponent(ecs::TYPEANIMATIONCOMPONENT, entityID));
                                        ani->m_stripCount = assets->m_imageManager.m_imageMap[sc->m_imageFile].m_stripCount;
                                    }
                                }
                                else
                                {
                                    assetmanager::AssetManager* assets = assetmanager::AssetManager::m_funcGetInstance();
                                    ecs::AnimationComponent* ani = static_cast<ecs::AnimationComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEANIMATIONCOMPONENT]->m_GetEntityComponent(entityID));
                                    ani->m_stripCount = assets->m_imageManager.m_imageMap[sc->m_imageFile].m_stripCount;
                                }
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
