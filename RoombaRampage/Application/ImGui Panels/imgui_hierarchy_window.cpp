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
#include "../../De&Serialization/json_handler.h"
#include "../Assets/AssetManager.h"

#include<vector>
#include<string>
#include <iostream>

namespace gui {
    unsigned int ImGuiHandler::m_DrawHierachyWindow()
    {
        //fetch ecs
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();

        // Custom window with example widgets
        bool open = true;
        ImGui::Begin("Hierachy Window", &open, ImGuiWindowFlags_MenuBar);

        if (ImGui::BeginMenuBar())
        {

            if (ImGui::BeginMenu("Prefabs"))
            {
                assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
                if (assetmanager->m_prefabs.size() > 0) {
                    for (auto prefab : assetmanager->m_prefabs) {

                        if (ImGui::MenuItem(prefab.first.c_str())) {

                            int id = prefab::Prefab::m_CreateEntityFromPrefab(prefab.first);
                            if (id == -1) continue;

                            m_clickedEntityId = id;
                        }
                    }
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        std::string ObjectCountStr = "Oject Count: " + std::to_string(ecs->m_ECS_EntityMap.size());
        ImGui::Text(ObjectCountStr.c_str());

        if (ImGui::Button("+ Add GameObject"))
            ImGuiHandler::m_objectNameBox ? ImGuiHandler::m_objectNameBox = false : m_objectNameBox = true;




        if (m_objectNameBox)
        {
            if (ImGui::InputText("##", m_charBuffer, IM_ARRAYSIZE(m_charBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {

                //create ID then push into vector
                ecs::EntityID newEntityID = ecs->m_CreateEntity();

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


        for (auto& entity : ecs->m_ECS_EntityMap) {


            m_DrawEntityNode(entity.first);

            
            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Delete Entity")) {
                    ecs->m_DeleteEntity(m_clickedEntityId);
                    m_clickedEntityId = 0;
                    //break to reinitialize
                    ImGui::EndPopup();
                    break;
                }

                if (ImGui::MenuItem("Duplicate Entity")) {
                    ecs->m_DuplicateEntity(m_clickedEntityId);
                }

                ImGui::EndPopup();
            }

            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                ImGui::SetDragDropPayload("Entity", &entity.first, sizeof(ecs::EntityID));
                ImGui::Text(static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(entity.first))->m_entityName.c_str());
                ImGui::EndDragDropSource();
            }

            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity"))
                {
                    

                }
                ImGui::EndDragDropTarget();
            }

        }

        ImGui::End();

        return m_clickedEntityId;
    }


    void ImGuiHandler::m_DrawEntityNode(ecs::EntityID id) {

        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        ImGuiTreeNodeFlags flag = ((m_clickedEntityId == id) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        bool open = ImGui::TreeNodeEx(std::to_string(id).c_str(), flag, static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id))->m_entityName.c_str());
        if (ImGui::IsItemClicked())
        {
            m_clickedEntityId = id;


        }
        
        if (open) {
            //recursion
           // m_DrawEntityNode(1);
            ImGui::TreePop();
        }



    }




}

