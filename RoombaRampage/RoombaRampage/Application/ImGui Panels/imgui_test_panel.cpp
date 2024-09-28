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



void ImGuiHandler::DrawTestWindow() {
	
	bool clicked = false;
	static int maxTime = 60;
	static int currTime = 0;
	
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

	ImGui::End();
}
