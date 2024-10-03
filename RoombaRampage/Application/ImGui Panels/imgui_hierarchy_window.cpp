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

unsigned int gui::ImGuiHandler::m_DrawHierachyWindow()
{
    //fetch ecs
    ecs::ECS* ecs = ecs::ECS::m_GetInstance();

    static bool hasLoaded = false;
    if (!hasLoaded) {
        // Iterate through all loaded entities and add them to the hierarchy
        for (const auto& entityPair : ecs->m_ECS_EntityMap) {
            this->m_objEntityId.push_back(entityPair.first);
            m_objTextEntries.push_back(static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(entityPair.first))->m_entityName);
            m_deleteButton.push_back(false);
            m_duplicateButton.push_back(false);
        }
        hasLoaded = true;
    }

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

                        m_objEntityId.push_back(static_cast<ecs::EntityID>(id));
                        m_objTextEntries.push_back(prefab.first);
                        m_deleteButton.push_back(false);
                        m_duplicateButton.push_back(false);
                        m_clickedEntityId = id;
                    }
                }
            }
          
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    std::string ObjectCountStr = "Oject Count: " + std::to_string(m_objEntityId.size());
    ImGui::Text(ObjectCountStr.c_str());

    if (ImGui::Button("+ Add GameObject"))
        ImGuiHandler::m_objectNameBox ? ImGuiHandler::m_objectNameBox = false : m_objectNameBox = true;
		
    if (m_objectNameBox)
    {
        if (ImGui::InputText("##", m_charBuffer, IM_ARRAYSIZE(m_charBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
        {

            //create ID then push into vector
            ecs::EntityID newEntityID = ecs->m_CreateEntity();
            m_objEntityId.push_back(newEntityID);

            //set new ID to be clicked
            m_clickedEntityId = newEntityID;
            //Add the string into the vector
            m_objTextEntries.push_back(std::string(m_charBuffer));
            // add string to name component
            static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(newEntityID))->m_entityName = std::string(m_charBuffer);

            //Set to false as no button showing first
            //Used to track and maintain sync between objtextentries and deletebutton vector
            m_deleteButton.push_back(false);
            m_duplicateButton.push_back(false);

            m_charBuffer[0] = '\0';
            m_objectNameBox = false;

            //Serialization::Serialize::SaveComponentsJson("../RoombaRampage/Json", Ecs::ECS::GetInstance()->ECS_EntityMap, obj_text_entries, obj_entity_id);
        }
    }

    //For loop to display all the gameobj text as button 
    for (size_t i = 0; i < m_objTextEntries.size(); i++)
    {
        // '##' let IMGui set an internal unique ID to widget without visible label!
        std::string buttonName = m_objTextEntries[i] +"##"+ std::to_string(i);

        if (ImGui::Button(buttonName.c_str()))
        {
            std::fill(m_deleteButton.begin(), m_deleteButton.end(), false);
            std::fill(m_duplicateButton.begin(), m_duplicateButton.end(), false);

            m_deleteButton[i] ? m_deleteButton[i] = false : m_deleteButton[i] = true;
            m_duplicateButton[i] ? m_duplicateButton[i] = false : m_duplicateButton[i] = true;

            m_clickedEntityId = m_objEntityId[i];
            std::cout << "Entity ID clicked: " << m_clickedEntityId << std::endl; //For debug purposes, remove later
        }

        //Render delete button
        if (m_deleteButton[i])
        {
            //Use _button,_buttonhover_buttonactive
            //To change the button color
            //Dont forget to pop
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.02f, 0.2f, 1.0f));  // Red 
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));  // Lighter red
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.1f, 0.1f, 1.0f));  // Darker red

            //Use this to make the button side by side on the same line
            ImGui::SameLine();
            std::string deleteButtonLabel = "Delete ##" + std::to_string(i);

            if (ImGui::Button(deleteButtonLabel.c_str()))
            {
                //Delete entity from ecs               
                ecs::ECS::m_GetInstance()->m_DeleteEntity(m_objEntityId[i]);

                //remove the entries 
                m_objTextEntries.erase(m_objTextEntries.begin() + i);
                m_objEntityId.erase(m_objEntityId.begin() + i);
                m_deleteButton.erase(m_deleteButton.begin() + i);
                m_duplicateButton.erase(m_duplicateButton.begin() + i);
                
                if (m_objEntityId.size() > 0) {
                    m_clickedEntityId = m_objEntityId[0];//set to 
                }
                


                i--;

                ImGui::PopStyleColor(3);  // Pop the 3 style colors (button, hovered, and active)
                continue;
            }

            ImGui::PopStyleColor(3);  // Pop the 3 style colors (button, hovered, and active)
        }

        //Render Duplicate button
        if (m_duplicateButton[i])
        {
            //Use _button,_buttonhover_buttonactive
            //To change the button color
            //Dont forget to pop
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.5f, 0.1f, 1.0f));  // green
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.8f, 0.1f, 1.0f));  // Lighter red
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 1.f, 0.1f, 1.0f));  // Darker red

            //Use this to make the button side by side on the same line
            ImGui::SameLine();
            std::string DuplicateButtonLabel = "Duplicate ##" + std::to_string(i);

            if (ImGui::Button(DuplicateButtonLabel.c_str()))
            {

                //create ID then push into vector
                ecs::EntityID newEntityID = ecs->m_DuplicateEntity(m_objEntityId[i]);
                m_objEntityId.push_back(newEntityID);

                //Add the string into the vector
                m_objTextEntries.push_back(std::string(m_objTextEntries[i]));

                //Set to false as no button showing first
                //Used to track and maintain sync between objtextentries and deletebutton vector
                m_deleteButton.push_back(false);
                m_duplicateButton.push_back(false);

                m_charBuffer[0] = '\0';
                m_objectNameBox = false;

                ImGui::PopStyleColor(3);  // Pop the 3 style colors (button, hovered, and active)
                continue;
            }

            ImGui::PopStyleColor(3);  // Pop the 3 style colors (button, hovered, and active)
        }
    }

    ImGui::End();

    return m_clickedEntityId;
}
