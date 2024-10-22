#include "imgui_handler.h"

#include <filesystem>
#include <string>


namespace gui {

	 

	void ImGuiHandler::m_DrawContentBrowser() {

		ImGui::Begin("Content Browser");
		const char* directory = "Assets";

		for (auto& directoryPath : std::filesystem::directory_iterator(directory)) {

			std::string path = directoryPath.path().string();
			ImGui::Text(path.c_str());

		}




		ImGui::End();
	}

}