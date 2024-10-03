#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"

#include"../Inputs/Input.h"

void gui::ImGuiHandler::m_DrawInputWindow() {

	bool open = true;
	ImGui::Begin("Inputs", &open);

	std::string mousePos = "Mouse Position: (" + std::to_string(static_cast<int>(Input::InputSystem::MousePosition.m_x)) + " , " + std::to_string(static_cast<int>(Input::InputSystem::MousePosition.m_y)) + ")";
	ImGui::Text(mousePos.c_str());
	
	std::string keyStatus = "Key Status: " + Input::InputSystem::m_keyString;
	ImGui::Text(keyStatus.c_str());

	std::string mouseStatus = "MouseButton Status: " + Input::InputSystem::m_mouseString;
	ImGui::Text(mouseStatus.c_str());

	ImGui::End();
}
