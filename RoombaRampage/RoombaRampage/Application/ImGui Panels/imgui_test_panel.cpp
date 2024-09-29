#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"
#include "../ECS/ECS.h"
#include "../Helper.h"
#include "../../De&Serialization/json_handler.h"
#include "../Application.h"

#include<vector>
#include<string>
#include <iostream>
#include <random> 



void ImGuiHandler::DrawTestWindow() {
	
	bool clicked = false;
	static int maxTime = 60;
	static int currTime = 0;

	Ecs::ECS *ecs = Ecs::ECS::GetInstance();
	
	bool open = true;
	ImGui::Begin("Test Window", &open);
	if (ImGui::Button("Vacuum")) {
		Application::Application::audio.playSound();
	}
	ImGui::SameLine();
	if (ImGui::Button("Sound")) {
		//to add other sound
	}
	ImGui::NewLine();
	if (ImGui::Button("Crash")) {
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
			 Ecs::EntityID id = ecs->CreateEntity();
			 Ecs::TransformComponent* tc = (Ecs::TransformComponent*)ecs->ECS_CombinedComponentPool[Ecs::TypeTransformComponent]->GetEntityComponent(id);
			 ecs->AddComponent(Ecs::TypeSpriteComponent, id);

			 tc->position.y = height(gen);

		}
	}


	ImGui::End();
}
