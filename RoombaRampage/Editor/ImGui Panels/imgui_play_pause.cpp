#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"
#include "../Application/Helper.h"
#include "../Asset Manager/AssetManager.h"
#include "../Asset Manager/SceneManager.h"

namespace gui {
	
	void ImGuiHandler::m_DrawPlayPauseWindow() {
		static bool pause = false;
		bool open = true;
		//Helper::Helpers* help = Helper::Helpers::GetInstance();
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		ImGui::Begin("Play & Pause", &open);
		if (ImGui::Button("Play")) {
			pause = false;
			ecs->m_pause = true;

			assetmanager::AssetManager::m_funcGetInstance()->m_scriptManager.m_CompileAllCsharpFile();
			//help->m_fixedDeltaTime = 1.0 / 60.0;

			//assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();

			//compile all .cs file
			//assetmanager->m_scriptManager.m_CompileAllCsharpFile();


			//std::cout << help->m_fixedDeltaTime << std::endl;
		}
		ImGui::SameLine();
		if (ImGui::Button("Pause")) {
			pause = true;
			ecs->m_pause = false;
			//help->m_fixedDeltaTime = 0;

			//m_clickedEntityId = -1;
			//scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();
			//scenemanager->m_ReloadScene();








		}
		ImGui::End();
	}

}