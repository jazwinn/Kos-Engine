/*!
\file      imgui_text_panel.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Sept 28, 2024
\brief     This file holds the imgui panel that has buttons that will be used for testing purposes

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
	assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();
	logging::Logger log;

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
		assetManager->m_audioContainer[1]->m_PlaySound();
	}
	if (ImGui::Button("Stop Sound")) {
		assetManager->m_audioContainer[0]->m_StopSound();
		assetManager->m_audioContainer[1]->m_StopSound();
	}
	ImGui::NewLine();
	if (ImGui::Button("Crash")) {
		LOGGING_INFO("About to trigger abort");
		abort();
	}
	ImGui::NewLine();
	if (ImGui::Button("Test Log")) {
		log.m_GetInstance().m_TestingLog();
	}
	ImGui::NewLine();

	static bool spawn = false;
	if (ImGui::Button("Spawn 2500")) {
		
		if (spawn) {
			LOGGING_WARN("SPAWNING MORE THAN SET MAX ENTITY WILL CAUSE ASSERTION");
		}
		else {
			int lowerBoundy = -1;
			int upperBoundy = 1;



			std::random_device rd;
			std::mt19937 gen(rd());

			std::uniform_real_distribution<float> height(static_cast<float>(lowerBoundy), static_cast<float>(upperBoundy));
			std::uniform_real_distribution<float> height2(-1.5, 1.5);

			for (int n{}; n < 2500; n++) {
				ecs::EntityID id = ecs->m_CreateEntity();
				ecs::TransformComponent* tc = (ecs::TransformComponent*)ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id);
				ecs::SpriteComponent* sc = static_cast<ecs::SpriteComponent*>(ecs->m_AddComponent(ecs::TYPESPRITECOMPONENT, id));
				sc->m_imageID = 3;
				
				tc->m_scale = { 1.f,1.f };
				tc->m_position.m_y = static_cast<float>(height(gen));
				tc->m_position.m_x = static_cast<float>(height2(gen));

			}

			spawn = true;
		}
		
		
		
	}


	ImGui::End();
}
