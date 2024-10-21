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
#include "../Assets/AssetManager.h"
#include "../Debugging/Logging.h"

#include<vector>
#include<string>
#include <iostream>

//Testing purposes! this are the three files needed!
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

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


        for (std::unordered_map<ecs::EntityID, ecs::compSignature>::iterator entity = ecs->m_ECS_EntityMap.begin(); entity != ecs->m_ECS_EntityMap.end(); entity++ ) {

            //draw parent entity node
            //draw entity with no parents hahaha
            if (!ecs::Hierachy::m_GetParent(entity->first).has_value()) {
                if (m_DrawEntityNode(entity->first) == false) {
                    //delete is called
                    break;
                }
            }
          

           

        }

        ImGui::End();

        return m_clickedEntityId;
    }


    bool ImGuiHandler::m_DrawEntityNode(ecs::EntityID id) {

        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        ecs::TransformComponent* transCom =  static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id));
        if (transCom == NULL) return false;

        ImGuiTreeNodeFlags flag = ((m_clickedEntityId == id) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        if (transCom->m_childID.size() <= 0) {
            flag |= ImGuiTreeNodeFlags_Leaf;
        }
        
        bool open = ImGui::TreeNodeEx(std::to_string(id).c_str(), flag, static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id))->m_entityName.c_str());
        if (ImGui::IsItemClicked())
        {
            m_clickedEntityId = id;


        }

        //draw context window
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Delete Entity")) {
                ecs->m_DeleteEntity(id);
                m_clickedEntityId = -1;
                ImGui::EndPopup();
                if(open)ImGui::TreePop();
                return false;
            }

            if (ImGui::MenuItem("Duplicate Entity")) {
                ecs->m_DuplicateEntity(m_clickedEntityId);
            }

            ImGui::EndPopup();
        }

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
            ecs::EntityID index = id;
            ImGui::SetDragDropPayload("Entity", &index, sizeof(ecs::EntityID));
            ImGui::Text(static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id))->m_entityName.c_str());
            //ImGui::Text(std::to_string((int)index).c_str());
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity"))
            {
                IM_ASSERT(payload->DataSize == sizeof(ecs::EntityID));
                ecs::EntityID childId = *static_cast<ecs::EntityID*>(payload->Data);


                ecs::Hierachy::m_SetParent(id, childId);
                std::cout << "Set Parent:" << id << " Child: " << childId << std::endl;
                



            }
            ImGui::EndDragDropTarget();
        }
        
        if (open) {
            //recursion
            if (transCom->m_childID.size() > 0) {
                for (auto& id : transCom->m_childID) {
                    m_DrawEntityNode(id);
                }
            }
           // m_DrawEntityNode(1);
            ImGui::TreePop();
        }


        return true;
    }




}

