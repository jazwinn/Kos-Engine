#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"
#include "../Application/Helper.h"
#include "../Asset Manager/AssetManager.h"
#include "../Asset Manager/SceneManager.h"

namespace gui {
	
	void ImGuiHandler::m_DrawPlayPauseWindow() {
		static bool pause = true;
		bool open = true;
		//Helper::Helpers* help = Helper::Helpers::GetInstance();
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		ImGui::Begin("Play & Pause", &open);
		if (pause && ImGui::Button("Play")) {
			pause = false;
			ecs->m_nextState = (ecs->m_getState() == ecs::STOP) ? ecs::START : ecs::RUNNING;
	
			if (ecs->m_nextState == ecs::START) {
				assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
				assetmanager->m_scriptManager.m_HotReloadCompileAllCsharpFile();
				assetmanager->m_scriptManager.m_ReloadAllDLL();
			}


		}
		if (!pause && ImGui::Button("Pause")) {
			pause = true;
			ecs->m_nextState = ecs::WAIT;
			//help->m_fixedDeltaTime = 0;

			//m_clickedEntityId = -1;
			//scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();
			//scenemanager->m_ReloadScene();
		}
		
		if (ecs->m_getState() != ecs::STOP) {
			ImGui::SameLine();
			if (ImGui::Button("Stop")) {
				ecs->m_nextState = ecs::STOP;
				pause = true;
			}
		}

		ImGui::End();
	}

}