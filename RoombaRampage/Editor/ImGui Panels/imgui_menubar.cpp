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

#include "../Editor/WindowFile.h"

void gui::ImGuiHandler::m_DrawMainMenuBar() {

    ImGuiIO& io = ImGui::GetIO();  // Get input/output data
    scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();
    //If CTRL + S press, save
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S))) {
        scenemanager->m_SaveActiveScene();
        std::cout << "Saving data..." << std::endl;
    }



    bool openNewFilepopup = false;;
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New")) {
                openNewFilepopup = true;


            }
           
            if (ImGui::MenuItem("Save")) {


                scenemanager->m_SaveActiveScene();

                
            }

            
            if (ImGui::BeginMenu("Open Recent")) {

                for (auto& scene : scenemanager->m_availableScenes) {
                    if (ImGui::MenuItem(scene.c_str())) {
                        scenemanager->m_LoadScene(scene);
                        m_clickedEntityId = -1;
                    }
                }

                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Open")) {
                char filePath[MAX_PATH];
                std::string path = file::FileWindow::m_OpenfileDialog(filePath);
                std::string scene = scenemanager->m_AddScene(path);
                scenemanager->m_LoadScene(scene);
                

            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window"))
        {

            ImGui::EndMenu();
        }

        if (openNewFilepopup) {
            ImGui::OpenPopup("New Scene");
        }


        if (ImGui::BeginPopupModal("New Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Enter Scene Name");
            ImGui::Separator();

            //static int unused_i = 0;
            //ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

            static char str1[128] = "";
            ImGui::InputTextWithHint(".json", "Enter scene name here", str1, IM_ARRAYSIZE(str1));

            if (ImGui::Button("Save", ImVec2(120, 0))) { 
                std::string scene = str1;
                if (!scene.empty()) {
                    if (auto result = scenemanager->m_CreateNewScene(scene); result.has_value()) {
                        std::string filepath = result.value();
                        scenemanager->m_AddScene(filepath);
                        scenemanager->m_activeScene = scene;
                        scenemanager->m_SaveActiveScene();
                    }
                }
                else {
                    LOGGING_WARN("Please Insert a Scene Name");
                }



                ImGui::CloseCurrentPopup(); 

            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }

        ImGui::EndMainMenuBar();
    }



    
}

