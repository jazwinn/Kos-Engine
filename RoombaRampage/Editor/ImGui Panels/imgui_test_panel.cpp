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
#include "../Application/Helper.h"
#include "../../De&Serialization/json_handler.h"
#include "../Debugging/Logging.h"
#include "../Asset Manager/AssetManager.h"
#include "../Events/BaseMessage.h"
#include "../Events/MessageSystem.h"
#include "../Events/Listeners.h"
#include "../Events/EventHandler.h"

#include<vector>
#include<string>
#include <iostream>
#include <random> 



void gui::ImGuiHandler::m_DrawTestWindow() {
	
	//bool clicked = false;
	static int maxTime = 60;
	static int currTime = 0;

	//ecs::ECS *ecs = ecs::ECS::m_GetInstance();
	assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();
	logging::Logger log;

	messaging::MessageSystem MsgSys;
	MsgSys.m_AddListener(messaging::MessageType::AUDIOPLAY, messaging::SoundPlayed);
	bool open = true;
	ImGui::Begin("Test Window", &open);
	if (ImGui::Button("BGM")) {
		// abit hard coded might cause crash
		//assetManager->m_audioContainer[0]->m_playSound();
		/*messaging::Message vacuumBtnClick(messaging::MessageType::AUDIOPLAY, "Test Panel");
		MsgSys.m_SendMessage(vacuumBtnClick);*/
		events::ButtonPressEvent temp(1);
		DISPATCH_BUTTON_EVENT(temp);
		//Honestly unsure how to test unregistering cuz idk when it would be used
		//REMOVE_BUTTON_LISTENER(m_buttonID);
	}
	ImGui::SameLine();
	if (ImGui::Button("Sound")) {
		assetManager->m_audioManager.m_soundMap.find("mindstorm.wav")->second->m_PlaySound();
	}
	if (ImGui::Button("Stop Sound")) {
		assetManager->m_audioManager.m_soundMap.find("mindstorm.wav")->second->m_StopSound();
		assetManager->m_audioManager.m_soundMap.find("zwing.wav")->second->m_StopSound();
	}
	ImGui::NewLine();
	ImGui::SeparatorText("##########################################");
	ImGui::NewLine();
	if (ImGui::Button("Crash")) {
		LOGGING_INFO("About to trigger abort");
		abort();
	}
	ImGui::SeparatorText("##########################################");
	ImGui::NewLine();
	if (ImGui::Button("Test Log")) {
		log.m_GetInstance().m_TestingLog();
	}
	ImGui::SeparatorText("##########################################");
	ImGui::NewLine();

	static bool spawn = false;
	static std::vector <ecs::EntityID> list_Of_Entities;
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
				
				

			}
			//spawn = true;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete 2500")) {
		if (spawn) {
			for (auto& i : list_Of_Entities) {
				ecs::ECS::m_GetInstance()->m_DeleteEntity(i);
			}
		}
	}
	
	ImGui::SeparatorText("##########################################");
	ImGui::NewLine();
	static bool collision_Flag = false;
	static bool delete_Flag = false;
	static ecs::EntityID id_1;
	static ecs::EntityID id_2;
	if (ImGui::Button("Collision Test")) {
		//if (!collision_Flag) {
		//	//create player 
		//	id_1 = ecs->m_CreateEntity();
		//	ecs::TransformComponent* tc = (ecs::TransformComponent*)ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id_1);
		//	ecs::SpriteComponent* sc = static_cast<ecs::SpriteComponent*>(ecs->m_AddComponent(ecs::TYPESPRITECOMPONENT, id_1));
		//	ecs::ColliderComponent* cc = static_cast<ecs::ColliderComponent*>(ecs->m_AddComponent(ecs::TYPECOLLIDERCOMPONENT, id_1));
		//	ecs::RigidBodyComponent* rc = static_cast<ecs::RigidBodyComponent*>(ecs->m_AddComponent(ecs::TYPERIGIDBODYCOMPONENT, id_1));

		//	tc->m_scale = { 1.f,1.f };
		//	tc->m_position.m_x = static_cast<float>(1.0f);
		//	tc->m_position.m_y = static_cast<float>(0);
		//	cc->m_Size = { 0.5f,0.5f };

		//	id_2 = ecs->m_CreateEntity();
		//	tc = (ecs::TransformComponent*)ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id_2);
		//	sc = static_cast<ecs::SpriteComponent*>(ecs->m_AddComponent(ecs::TYPESPRITECOMPONENT, id_2));
		//	cc = static_cast<ecs::ColliderComponent*>(ecs->m_AddComponent(ecs::TYPECOLLIDERCOMPONENT, id_2));
		//	rc = static_cast<ecs::RigidBodyComponent*>(ecs->m_AddComponent(ecs::TYPERIGIDBODYCOMPONENT, id_2));


		//	tc->m_scale = { 1.f,1.f };
		//	tc->m_position.m_x = static_cast<float>(-1.0f);
		//	tc->m_position.m_y = static_cast<float>(0);
		//	cc->m_Size = { 0.5f,0.5f };

		//}
		//collision_Flag = true;
		//delete_Flag = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete")) {
		if (!delete_Flag) {
			ecs::ECS::m_GetInstance()->m_DeleteEntity(id_1);
			ecs::ECS::m_GetInstance()->m_DeleteEntity(id_2);
			collision_Flag = false;
			delete_Flag = true;
		}
	}


	ImGui::SeparatorText("##########################################");
	ImGui::NewLine();
	static bool font_Flag = false;
	static bool font_delete_Flag = true;
	static ecs::EntityID fontid_1;
	static ecs::EntityID fontid_2;
	if (ImGui::Button("Double Font Test") && !font_Flag)
	{
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();

		fontid_1 = ecs->m_CreateEntity(m_activeScene);
		ecs->m_AddComponent(ecs::TYPETEXTCOMPONENT, fontid_1);
		static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(fontid_1))->m_fileName = std::string("lazy_dog.ttf");
		static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(fontid_1))->m_text = std::string("Hello");
		static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(fontid_1))->m_entityName = std::string("Test Font 1");
		static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(fontid_1))->m_position = { -1.f,1.f };
		static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(fontid_1))->m_fontSize = { 10.f };


		fontid_2 = ecs->m_CreateEntity(m_activeScene);
		ecs->m_AddComponent(ecs::TYPETEXTCOMPONENT, fontid_2);
		static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(fontid_2))->m_fileName = std::string("RubikGlitch-Regular.ttf");
		static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(fontid_2))->m_text = std::string("World");
		static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(fontid_2))->m_entityName = std::string("Test Font 2");
		static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(fontid_2))->m_position = { -1.f,-1.f };
		static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(fontid_2))->m_fontSize = { 10.f };
		font_Flag = true;
		font_delete_Flag = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete Font")) 
	{
		if (!font_delete_Flag) 
		{
			ecs::ECS* ecs = ecs::ECS::m_GetInstance();
			if (ecs->m_ECS_EntityMap.find(fontid_1) != ecs->m_ECS_EntityMap.end())
			{
				ecs::ECS::m_GetInstance()->m_DeleteEntity(fontid_1);
			}
			if (ecs->m_ECS_EntityMap.find(fontid_2) != ecs->m_ECS_EntityMap.end())
			{
				ecs::ECS::m_GetInstance()->m_DeleteEntity(fontid_2);
			}
			font_Flag = false;
			font_delete_Flag = true;
		}
	}


	ImGui::End();
}
