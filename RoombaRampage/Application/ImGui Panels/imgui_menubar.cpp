/******************************************************************/
/*!
\file      Imgui_menubar.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the defination of the draw mainmenu bar window
           function.



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"
#include "imgui_internal.h"

#include "../../De&Serialization/json_handler.h"
#include "../ECS/ECS.h"

void gui::ImGuiHandler::DrawMainMenuBar() {

    ImGuiIO& io = ImGui::GetIO();  // Get input/output data
    //If CTRL + S press, save
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S))) {
        Serialization::Serialize::m_SaveComponentsJson("../RoombaRampage/Json", ecs::ECS::m_GetInstance()->m_ECS_EntityMap, obj_text_entries, obj_entity_id);
        std::cout << "Saving data..." << std::endl;
    }

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save")) {


                Serialization::Serialize::m_SaveComponentsJson("../RoombaRampage/Json", ecs::ECS::m_GetInstance()->m_ECS_EntityMap, obj_text_entries, obj_entity_id);

                
            }
            
            if (ImGui::BeginMenu("Open - Work In Progress")) {


                if (ImGui::MenuItem("Components.json")) {
                    //Serialization::Serialize::LoadComponentsJson("../RoombaRampage/Json/components.json", Ecs::ECS::GetInstance(), obj_text_entries);
                }

                ImGui::EndMenu();
            }

                

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window"))
        {

            ImGui::EndMenu();
        }


        ImGui::EndMainMenuBar();
    }

}