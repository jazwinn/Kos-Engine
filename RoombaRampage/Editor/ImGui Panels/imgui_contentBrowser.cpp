#include "imgui_handler.h"

#include <filesystem>
#include <string>
#include "../Asset Manager/AssetManager.h"

namespace gui {

	static std::filesystem::path assetDirectory = "Assets";
	static std::filesystem::path currentDirectory = assetDirectory;
	static const char* fileIcon = "folder.png";

	void ImGuiHandler::m_DrawContentBrowser() {

		ImGui::Begin("Content Browser");
		
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();

		if (currentDirectory != assetDirectory && ImGui::Button("Back")) {

			currentDirectory = currentDirectory.parent_path();

		}
		else {
			ImGui::NewLine();
		}

		//TODO create imgui config file
		static float padding = 20.f;
		static float thumbnail = 64.f;
		float cellsize = padding + thumbnail;

		float panelwidth = ImGui::GetContentRegionAvail().x;
		int columns = (int)(panelwidth / cellsize);
		if (columns <= 0) {
			columns = 8;
		}
		ImGui::Columns(columns, 0, false);

		for (auto& directoryPath : std::filesystem::directory_iterator(currentDirectory)) {
			std::string directoryString = directoryPath.path().filename().string();
			if (directoryPath.is_directory()) {
				// if a folder
				ImGui::ImageButton(directoryString.c_str(), (ImTextureID)assetmanager->m_imageManager.m_imageMap.find(fileIcon)->second.textureID, { thumbnail ,thumbnail }, { 0,1 }, { 1,0 }, {0,0,0,0});
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					currentDirectory /= directoryPath.path().filename();
					std::cout << currentDirectory << std::endl;

				}
			}
			else {
				//is a texture file
				if (directoryPath.path().filename().extension().string() == ".png") {
					std::string fileName = directoryPath.path().filename().string();
					ImGui::ImageButton(directoryString.c_str(), (ImTextureID)assetmanager->m_imageManager.m_imageMap.find(fileName)->second.textureID, { thumbnail ,thumbnail }, { 0,1 }, { 1,0 }, { 0,0,0,0 });
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {


					}
				}
				else if (ImGui::Button(directoryString.c_str(), { thumbnail ,thumbnail })) {


				}
			}

			ImGui::SetWindowFontScale(1.f);
			ImGui::Text(directoryString.c_str());
			ImGui::SetWindowFontScale(1.f);
			ImGui::NextColumn();
		}



		ImGui::Columns(1);
		ImGui::End();
	}

}