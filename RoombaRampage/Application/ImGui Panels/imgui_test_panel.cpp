#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"
#include "../ECS/ECS.h"
#include "../Helper.h"
#include "../../De&Serialization/json_handler.h"
#include "../Debugging/Logging.h"
#include "../Assets/AssetManager.h"
#include "../Events/BaseMessage.h"
#include "../Events/MessageSystem.h"
#include "../Events/Listeners.h"

#include<vector>
#include<string>
#include <iostream>
#include <random> 



void ImGuiHandler::DrawTestWindow() {
	
	//bool clicked = false;
	static int maxTime = 60;
	static int currTime = 0;

	ecs::ECS *ecs = ecs::ECS::m_GetInstance();
	assetmanager::AssetManager* assetManager = assetmanager::AssetManager::funcGetInstance();

	messaging::MessageSystem MsgSys;
	MsgSys.m_AddListener(messaging::MessageType::AUDIOPLAY, messaging::SoundPlayed);
	bool open = true;
	ImGui::Begin("Test Window", &open);
	if (ImGui::Button("Vacuum")) {
		// abit hard coded might cause crash
		//assetManager->m_audioContainer[0]->m_playSound();
		messaging::Message vacuumBtnClick(messaging::MessageType::AUDIOPLAY, "Test Panel");
		MsgSys.m_SendMessage(vacuumBtnClick);
	}
	ImGui::SameLine();
	if (ImGui::Button("Sound")) {
		assetManager->m_audioContainer[1]->m_playSound();
	}
	if (ImGui::Button("Stop Sound")) {
		assetManager->m_audioContainer[0]->m_stopSound();
		assetManager->m_audioContainer[1]->m_stopSound();
	}
	ImGui::NewLine();
	if (ImGui::Button("Crash")) {
		LOGGING_INFO("About to trigger abort");
		abort();
	}
	ImGui::NewLine();
	if (ImGui::Button("Spawn 2500")) {
		int lowerBoundy = -1;
		int upperBoundy = 1;



		std::random_device rd;
		std::mt19937 gen(rd());

		std::uniform_int_distribution<> height(lowerBoundy, upperBoundy);


		for (int n{}; n < 2500; n++) {
			 ecs::EntityID id = ecs->m_CreateEntity();
			 ecs::TransformComponent* tc = (ecs::TransformComponent*)ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id);
			 ecs->m_AddComponent(ecs::TYPESPRITECOMPONENT, id);

			 tc->m_position.m_y = static_cast<float>(height(gen));

		}
	}


	ImGui::End();
}
