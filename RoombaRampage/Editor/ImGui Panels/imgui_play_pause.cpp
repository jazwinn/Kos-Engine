/*!
\file      imgui_play_pause.cpp
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Sept 28, 2024
\brief     This file holds the imgui panel that has buttons for starting/pausing/stopping the editor

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"
#include "../Application/Helper.h"
#include "../Asset Manager/AssetManager.h"
#include "../Asset Manager/SceneManager.h"

namespace gui {
	
    void ImGuiHandler::m_DrawPlayPauseBar() {
        static bool pause = true;
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        if (ImGui::BeginMenuBar()) {
            if (pause && ImGui::Button("Play")) {
                pause = false;
                ecs->m_nextState = (ecs->m_getState() == ecs::STOP) ? ecs::START : ecs::RUNNING;

                if (ecs->m_nextState == ecs::START) {

                    //save all active scenes
                    scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();
                    scenemanager->m_SaveAllActiveScenes();


                    assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
                    assetmanager->m_scriptManager.m_HotReloadCompileAllCsharpFile();
                    assetmanager->m_scriptManager.m_ReloadAllDLL();
                }
            }
            else if (!pause && ImGui::Button("Pause")) {

                pause = true;
                ecs->m_nextState = ecs::WAIT;





            }

            if (ImGui::Button("stop")) {
                if (ecs->m_getState() != ecs::STOP) {
                    ecs->m_nextState = ecs::STOP;
                    pause = true;

                    //TODO load back scene
                    scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();
                    scenemanager->m_ReloadScene();
                    m_clickedEntityId = -1;
                }

            }

        }
        ImGui::EndMenuBar();
    }
}