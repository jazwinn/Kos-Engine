#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"
#include "../Application/Helper.h"

namespace gui {
	
	void ImGuiHandler::m_DrawPlayPauseWindow() {
		static bool pause = false;
		bool open = true;
		Helper::Helpers* help = Helper::Helpers::GetInstance();
		ImGui::Begin("Play & Pause", &open);
		if (ImGui::Button("Play")) {
			pause = false;
			help->m_fixedDeltaTime = 1.0 / 60.0;
			//std::cout << help->m_fixedDeltaTime << std::endl;
		}
		ImGui::SameLine();
		if (ImGui::Button("Pause")) {
			pause = true;
			help->m_fixedDeltaTime = 0;
		}
		ImGui::End();
	}

}