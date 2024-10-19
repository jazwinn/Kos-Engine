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

#include "../Assets/SceneManager.h"
#include "../ECS/ECS.h"

void gui::ImGuiHandler::m_DrawMainMenuBar() {

    ImGuiIO& io = ImGui::GetIO();  // Get input/output data
    scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();
    //If CTRL + S press, save
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S))) {
        scenemanager->m_SaveActiveScene();
        std::cout << "Saving data..." << std::endl;
    }

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save")) {


                scenemanager->m_SaveActiveScene();

                
            }
            
            if (ImGui::BeginMenu("Open - Work In Progress")) {

                for (auto& scene : scenemanager->m_availableScenes) {
                    if (ImGui::MenuItem(scene.c_str())) {
                        scenemanager->m_LoadScene(scene);
                    }
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