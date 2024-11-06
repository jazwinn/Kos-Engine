/********************************************************************/
/*!
\file      imgui_hierarchy_window.cpp
\author    Chiu Jun Jie, junjie.c , 2301524
\par       junjie.c@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the implementation of the ImGui Hierarchy Window for rendering and managing game objects.
           - Displays and updates a list of game objects (entities) in the ECS framework.
           - Provides functionality for adding, deleting, and duplicating game objects.
           - Uses ImGui to create a hierarchy view, with interaction buttons for each game object.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"

#include "../ECS/ECS.h"
#include "../ECS/Hierachy.h"
#include "../../De&Serialization/json_handler.h"
#include "../Asset Manager/AssetManager.h"
#include "../Debugging/Logging.h"
#include "../Asset Manager/SceneManager.h"
#include "../Asset Manager/Prefab.h"

#include<vector>
#include<string>
#include <iostream>
#include <filesystem>

//Testing purposes! this are the three files needed!
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace gui {
    unsigned int ImGuiHandler::m_DrawHierachyWindow()
    {
        //fetch ecs
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();
        assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
        // Custom window with example widgets
        bool open = true;
        ImGui::Begin("Hierachy Window", &open);

        //if (ImGui::BeginMenuBar())
        //{

        //    ImGui::EndMenuBar();
        //}

        std::string ObjectCountStr = "Oject Count: " + std::to_string(ecs->m_ECS_EntityMap.size());
        ImGui::Text(ObjectCountStr.c_str());

        if (ImGui::Button("+ Add GameObject"))
            ImGuiHandler::m_objectNameBox ? ImGuiHandler::m_objectNameBox = false : m_objectNameBox = true;

        //create ID then push into vector
        // assign to the top most scene



        if (m_objectNameBox)
        {
            if (ImGui::InputText("##", m_charBuffer, IM_ARRAYSIZE(m_charBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {

                ecs::EntityID newEntityID = ecs->m_CreateEntity(m_activeScene);

                //if in prefab mode, assign entity to upmost parent
                if (m_prefabSceneMode) {
                    ecs::EntityID id = ecs->m_ECS_SceneMap.find(m_activeScene)->second.m_prefabID;
                    ecs::Hierachy::m_SetParent(std::move(id), newEntityID);
                }


                //set new ID to be clicked
                m_clickedEntityId = newEntityID;
                //Add the string into the vector
                // add string to name component
                static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(newEntityID))->m_entityName = std::string(m_charBuffer);


                m_charBuffer[0] = '\0';
                m_objectNameBox = false;

                //Serialization::Serialize::SaveComponentsJson("../RoombaRampage/Json", Ecs::ECS::GetInstance()->ECS_EntityMap, obj_text_entries, obj_entity_id);
            }
        }


        for (auto& sceneentity : ecs->m_ECS_SceneMap) {

            //when prefab mode is on, skip non prefabs, and non active prefabs
            if (m_prefabSceneMode) {

                if (sceneentity.second.m_isPrefab == false || sceneentity.second.m_isActive == false)continue;

                //create seperator
                ImGui::SeparatorText("Prefab");

                if (ImGui::Button("Back")){
                    //save "prefab"
                    scenemanager->m_SaveScene(m_activeScene);
                    //set current prefab back to inactive
                    ecs->m_ECS_SceneMap.find(m_activeScene)->second.m_isActive = false;
                    
                    //set back scene's active state
                    for (auto& scene : ecs->m_ECS_SceneMap) {
                        if(scene.second.m_isPrefab == false)
                        scene.second.m_isActive = m_savedSceneState.find(scene.first)->second;
                        
                    }

                    //set back active scene
                    for (auto& scene : ecs->m_ECS_SceneMap) {
                        if (!scene.second.m_isPrefab) {
                            m_activeScene = scene.first;
                            break;
                        }
                    }

                    m_prefabSceneMode = false;
                    m_clickedEntityId = -1;
                }



            }
            
            //skip if prefab is not active 
            if (sceneentity.second.m_isPrefab == true && sceneentity.second.m_isActive == false) continue;

            std::string headerstr = sceneentity.first.substr(0, sceneentity.first.find_last_of('.'));
            //collapsing header for scene
            bool opens{};
            if (sceneentity.second.m_isActive == false) {
                headerstr += " (Unloaded)";
                ImGui::CollapsingHeader(headerstr.c_str());
            }
            else {
                if (sceneentity.first == m_activeScene) {
                    headerstr += " (Active)";
                }
                opens = ImGui::CollapsingHeader(headerstr.c_str());
            }





            if (ImGui::BeginPopupContextItem()) {
                if ((sceneentity.first != m_activeScene) && ImGui::MenuItem("Remove Scene")) {
                    scenemanager->m_ClearScene(sceneentity.first);

                    //break loop
                    ImGui::EndPopup();
                    break;
                }

                if ((sceneentity.first != m_activeScene) && (sceneentity.second.m_isActive == true) && ImGui::MenuItem("Unload Scene")) {
                    sceneentity.second.m_isActive = false;
                    m_clickedEntityId = -1;

                    if (!m_prefabSceneMode) {
                        //change scene if current active scene is unloaded
                        if (ecs->m_ECS_SceneMap.find(m_activeScene)->second.m_isActive == false) {
                            //set first loaded scene as active
                            for (auto& scene : ecs->m_ECS_SceneMap) {
                                if (scene.second.m_isActive == true && scene.second.m_isPrefab == false) {
                                    m_activeScene = ecs->m_ECS_SceneMap.begin()->first;
                                }

                            }
                        }
                       

                    }

                    ImGui::EndPopup();
                    break;
                }

                if ((sceneentity.second.m_isActive == false) && ImGui::MenuItem("load Scene")) {
                    sceneentity.second.m_isActive = true;

                    ImGui::EndPopup();
                    break;
                }

                if (ImGui::MenuItem("Save Scene")) {
                    scenemanager->m_SaveScene(sceneentity.first);

                }

                if ((sceneentity.first != m_activeScene) && ImGui::MenuItem("Set Active")) {
                    m_activeScene = sceneentity.first;
                }

                ImGui::EndPopup();
            }
            if (ImGui::BeginDragDropTarget())
            {

                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity"))
                {
                    IM_ASSERT(payload->DataSize == sizeof(ecs::EntityID));
                    ecs::EntityID Id = *static_cast<ecs::EntityID*>(payload->Data);
                    const auto& scene = scenemanager->GetSceneByEntityID(Id);

                    if (scene.has_value()) {
                        scenemanager->m_SwapScenes(scene.value(), sceneentity.first, Id);
                    }

                    //if entity is a child, break from parent
                    const auto& parent = ecs::Hierachy::m_GetParent(Id);
                    if (parent.has_value()) {
                        ecs::Hierachy::m_RemoveParent(Id);
                    }

                    ecs::Hierachy::m_UpdateChildScene(Id);
                    

                }

                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("file"))
                {
                    IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
                    std::filesystem::path* filename = static_cast<std::filesystem::path*>(payload->Data);

                    if (filename->filename().extension().string() == ".prefab") {
                        
                        prefab::Prefab::m_CreatePrefab(filename->filename().string(), sceneentity.first);

                    }

                }
                ImGui::EndDragDropTarget();
            }

            if (opens) {


                for (auto entity : sceneentity.second.m_sceneIDs) {
                    //draw parent entity node
                    //draw entity with no parents hahaha
                    if (!ecs::Hierachy::m_GetParent(entity).has_value()) {
                        if (m_DrawEntityNode(entity) == false) {
                            //delete is called
                            break;
                        }
                    }
                }
            }      
        }


           
        if (ImGui::GetContentRegionAvail().x > 0 && ImGui::GetContentRegionAvail().y > 0) {
            //std::cout << "x: " << ImGui::GetContentRegionAvail().x << std::endl;
            //std::cout << "y: " << ImGui::GetContentRegionAvail().y << std::endl;


            ImGui::InvisibleButton("#invbut", ImVec2{ ImGui::GetContentRegionAvail().x,ImGui::GetContentRegionAvail().y });
            if (ImGui::BeginDragDropTarget())
            {

                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity"))
                {
                    IM_ASSERT(payload->DataSize == sizeof(ecs::EntityID));
                    ecs::EntityID Id = *static_cast<ecs::EntityID*>(payload->Data);
                    ecs::Hierachy::m_RemoveParent(Id);


                }

                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("file"))
                {
                    IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
                    std::filesystem::path* filename = static_cast<std::filesystem::path*>(payload->Data);


                    if (filename->filename().extension().string() == ".json") {
                        scenemanager->m_LoadScene(*filename);
                    }

                    if (filename->filename().extension().string() == ".prefab") {

                        prefab::Prefab::m_CreatePrefab(filename->filename().string(), m_activeScene);

                    }




                }
                ImGui::EndDragDropTarget();
            }

        }

        ImGui::End();

        return m_clickedEntityId;
    }


    bool ImGuiHandler::m_DrawEntityNode(ecs::EntityID id) {

        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        

        ecs::TransformComponent* transCom =  static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id));
        if (transCom == NULL) return false;

        ImGuiTreeNodeFlags flag = ((static_cast<unsigned int>(m_clickedEntityId) == id) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        if (transCom->m_childID.size() <= 0) {
            flag |= ImGuiTreeNodeFlags_Leaf;
        }
        
        ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id));

        //create color if prefab
        if (nc->m_isPrefab) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 1.0f, 1.0f));
        bool open = ImGui::TreeNodeEx(std::to_string(id).c_str(), flag, nc->m_entityName.c_str());
        if (nc->m_isPrefab) ImGui::PopStyleColor();

        if (ImGui::IsItemClicked())
        {
            m_clickedEntityId = id;
        }
       

        //draw context window
        if (ImGui::BeginPopupContextItem()) {

            //disable if the upmost prefab
            if (m_prefabSceneMode && (id == ecs->m_ECS_SceneMap.find(m_activeScene)->second.m_prefabID)) {
              

            }
            else {
                if (ImGui::MenuItem("Delete Entity")) {
                    ecs->m_DeleteEntity(id);
                    m_clickedEntityId = -1;
                    ImGui::EndPopup();
                    if (open)ImGui::TreePop();
                    return false;
                }
            }
           

            if (ImGui::MenuItem("Duplicate Entity")) {
                ecs::EntityID newid = ecs->m_DuplicateEntity(id);

                if (m_prefabSceneMode && (!ecs::Hierachy::m_GetParent(id).has_value())) {
                   ecs::Hierachy::m_SetParent(id, newid);
                }



                ImGui::EndPopup();
                if (open)ImGui::TreePop();
                return false;
                //return false to reset the loop
            }

            if (ImGui::MenuItem("Save Prefab")) {
                prefab::Prefab::m_SaveEntitytoPrefab(id);
            }

            ImGui::EndPopup();
        }

        //no reordering of child prefabs
        if (!transCom->m_haveParent || !static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(transCom->m_parentID))->m_isPrefab) {
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                //might undefine behaviour
                ecs::EntityID index = id;
                ImGui::SetDragDropPayload("Entity", &index, sizeof(ecs::EntityID));
                ImGui::Text(nc->m_entityName.c_str());
                //ImGui::Text(std::to_string((int)index).c_str());
                ImGui::EndDragDropSource();
            }
        }
        else {

        }


        if (ImGui::BeginDragDropTarget())
        {
            
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity"))
            {
                IM_ASSERT(payload->DataSize == sizeof(ecs::EntityID));
                ecs::EntityID childId = *static_cast<ecs::EntityID*>(payload->Data);


                ecs::Hierachy::m_SetParent(id, childId);
                std::cout << "Set Parent:" << id << " Child: " << childId << std::endl;

                // update child's scene
                ecs::Hierachy::m_UpdateChildScene(id);

                //return
                ImGui::EndDragDropTarget();
                if (open)ImGui::TreePop();
                return false;


            }
            ImGui::EndDragDropTarget();
        }
        

        

        if (open) {
            //recursion
            if (transCom->m_childID.size() > 0) {
                for (auto& ids : transCom->m_childID) {
                    if (!m_DrawEntityNode(ids)) {

                        ImGui::TreePop();
                        return false;
                    }
                }
            }
           // m_DrawEntityNode(1);
            ImGui::TreePop();
        }


        return true;
    }




}

