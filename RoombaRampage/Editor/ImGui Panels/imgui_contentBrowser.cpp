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
				if (assetmanager->m_imageManager.m_imageMap.find(fileIcon) != assetmanager->m_imageManager.m_imageMap.end()) {
					ImGui::ImageButton(directoryString.c_str(), (ImTextureID)assetmanager->m_imageManager.m_imageMap.find(fileIcon)->second.textureID, { thumbnail ,thumbnail }, { 0,1 }, { 1,0 }, { 0,0,0,0 });
				}
				else {
					ImGui::Button(directoryString.c_str(), { thumbnail ,thumbnail });
				}

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					currentDirectory /= directoryPath.path().filename();

				}
			}
			else {
				//is a texture file
				if (directoryPath.path().filename().extension().string() == ".png") {
					std::string fileName = directoryPath.path().filename().string();
					if (assetmanager->m_imageManager.m_imageMap.find(fileName) != assetmanager->m_imageManager.m_imageMap.end()) {
						ImGui::ImageButton(directoryString.c_str(), (ImTextureID)assetmanager->m_imageManager.m_imageMap.find(fileName)->second.textureID, { thumbnail ,thumbnail }, { 0,1 }, { 1,0 }, { 0,0,0,0 });
					}
					else {
						ImGui::Button(directoryString.c_str(), { thumbnail ,thumbnail });
					}
					
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {


					}
				}
				else if (directoryPath.path().filename().extension().string() == ".wav") {
					std::string fileName = "Audiowav.png";
					if (assetmanager->m_imageManager.m_imageMap.find(fileName) != assetmanager->m_imageManager.m_imageMap.end()) {
						ImGui::ImageButton(directoryString.c_str(), (ImTextureID)assetmanager->m_imageManager.m_imageMap.find(fileName)->second.textureID, { thumbnail ,thumbnail }, { 0,1 }, { 1,0 }, { 0,0,0,0 });
					}
					else {
						ImGui::Button(directoryString.c_str(), { thumbnail ,thumbnail });
					}

					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {


					}
				}
				else if (ImGui::Button(directoryString.c_str(), { thumbnail ,thumbnail })) {


				}
			}

			//create context window
			static bool rename = false;
			static std::string selectedfile{};
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::MenuItem("Rename")) {
					rename = true;
					selectedfile = directoryString;
				}
				if (ImGui::MenuItem("Delete")) {

				}
				ImGui::EndPopup();
			}


			if (rename && (selectedfile == directoryString)) {
				if (ImGui::InputText("##rename", m_charBuffer, IM_ARRAYSIZE(m_charBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
					//TODO check if file has extension, keep the extension
					std::filesystem::path path = std::filesystem::current_path();
					std::string newpath = path.string() + "\\" + currentDirectory.string() + "\\" + m_charBuffer;
					std::string oldpath = path.string() + "\\" + currentDirectory.string() + "\\" + directoryString;

					std::filesystem::rename(oldpath.c_str(), newpath.c_str());
					

					rename = false;
					selectedfile = {};

					//TODO edge cases,
					//Update assets if any of them are renamed
				}
			}
			else {
				//ImGui::SetWindowFontScale(1.f);
				ImGui::Text(directoryString.c_str());
				//ImGui::SetWindowFontScale(1.f);
			}

			
			
			
			ImGui::NextColumn();
		}

		ImGui::Columns(1);
		ImGui::End();
	}

}