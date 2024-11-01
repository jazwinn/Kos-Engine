#include "imgui_handler.h"

#include <filesystem>
#include <string>
#include "../Asset Manager/AssetManager.h"
#include "../Inputs/Input.h"

namespace gui {

	static std::filesystem::path assetDirectory = "Assets";
	static std::filesystem::path currentDirectory  = assetDirectory;
	static const char* fileIcon = "folder.png";

	void ImGuiHandler::m_DrawContentBrowser() {

		ImGui::Begin("Content Browser");
		
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();

		//if (currentDirectory != assetDirectory && ImGui::Button("Back")) {

		//	currentDirectory = currentDirectory.parent_path();

		//}
		//else {
		//	ImGui::NewLine();
		//}
		{
			ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x * 0.2f, ImGui::GetContentRegionAvail().y), ImGuiChildFlags_Border);
			static bool isSelected{ false };
			for (auto& directoryPath : std::filesystem::directory_iterator(assetDirectory)) {
				std::string directoryString = directoryPath.path().filename().string();
				if (ImGui::Selectable (directoryString.c_str())) {

					currentDirectory = assetDirectory/directoryPath.path().filename();


				}

			}
			ImGui::EndChild();
		}
		ImGui::SameLine();
		{


			ImGui::BeginChild("ChildLa", ImVec2(0, ImGui::GetContentRegionAvail().y));

			//back button
			if (currentDirectory != assetDirectory && ImGui::Button("Back")) {
				currentDirectory = currentDirectory.parent_path();
			}
			else {
				ImGui::NewLine();
			}

			static float padding = 20.f;
			static float thumbnail = 64.f;
			float cellsize = padding + thumbnail;

			float panelwidth = ImGui::GetContentRegionAvail().x;
			int columns = (int)(panelwidth / cellsize);
			if (columns <= 0) {
				columns = 8;
			}
			ImGui::Columns(columns, 0, false);

			auto textorimage = [](std::string directoryString, std::string fileName) {
				assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
				if (assetmanager->m_imageManager.m_imageMap.find(fileName) != assetmanager->m_imageManager.m_imageMap.end()) {
					ImGui::ImageButton(directoryString.c_str(), (ImTextureID)assetmanager->m_imageManager.m_imageMap.find(fileName)->second.textureID, { thumbnail ,thumbnail }, { 0,1 }, { 1,0 }, { 0,0,0,0 });
				}
				else {
					ImGui::Button(directoryString.c_str(), { thumbnail ,thumbnail });
				}
				};


			for (auto& directoryPath : std::filesystem::directory_iterator(currentDirectory)) {
				std::string directoryString = directoryPath.path().filename().string();

				if (directoryPath.is_directory()) {
					// if a folder
					textorimage(directoryString, fileIcon);
					//ImGui::ImageButton(directoryString.c_str(), (ImTextureID)assetmanager->m_imageManager.m_imageMap.find(fileIcon)->second.textureID, { thumbnail ,thumbnail }, { 0,1 }, { 1,0 }, { 0,0,0,0 });
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
						currentDirectory /= directoryPath.path().filename();
					}
				}
				else {
					if (directoryPath.path().filename().extension().string() == ".png") {
						std::string fileName = directoryPath.path().filename().string();
						textorimage(directoryString, fileName);

						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {


						}
					}
					else if (directoryPath.path().filename().extension().string() == ".wav") {
						std::string wavicon = "Audiowav.png";
						textorimage(directoryString, wavicon);

						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {


						}
					}
					else if (ImGui::Button(directoryString.c_str(), { thumbnail ,thumbnail })) {


					}

					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
						static std::filesystem::path filepath;
						filepath = directoryPath.path();
						ImGui::SetDragDropPayload("file", &filepath, sizeof(std::filesystem::path));
						ImGui::Text(filepath.string().c_str());
						ImGui::EndDragDropSource();
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
						std::filesystem::remove(directoryPath);
					}
					ImGui::EndPopup();
				}


				if (rename && (selectedfile == directoryString)) {
					if (ImGui::InputText("##rename", m_charBuffer, IM_ARRAYSIZE(m_charBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
						//TODO check if file has extension, keep the extension
						std::string extension{};
						if (!directoryPath.is_directory()) {
							extension = directoryPath.path().filename().extension().string();
							ImGui::SameLine();
							ImGui::Text(extension.c_str());
						}

						std::filesystem::path path = std::filesystem::current_path();
						std::string newpath = path.string() + "\\" + currentDirectory.string() + "\\" + m_charBuffer + extension;
						std::string oldpath = path.string() + "\\" + currentDirectory.string() + "\\" + directoryString;


						assetmanager->m_RenameAsset(oldpath, newpath);

						rename = false;
						selectedfile = {};

						//TODO edge cases,
						//Update assets if any of them are renamed
					}
				}
				else {
					ImGui::SetWindowFontScale(0.8f);
					ImGui::Text(directoryString.c_str());
					ImGui::SetWindowFontScale(1.f);
				}

				ImGui::NextColumn();
			}

			//allow drag and drop on child directory
			if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows)) {
				if (!Input::InputSystem::m_droppedFiles.empty()) {
					for (const auto& file : Input::InputSystem::m_droppedFiles) {
						std::filesystem::path source = file;
						std::filesystem::path destination = currentDirectory;

						if (!std::filesystem::exists(source)) {
							LOGGING_WARN("Source directory does not exist ");
						}
						else {
							if (std::filesystem::is_directory(source)) {
								destination /= source.filename();
								if (std::filesystem::exists(destination)) {
									LOGGING_WARN("Directory already exists");
									continue;
								}
							}

							// Copy directory and all contents recursively
							std::filesystem::copy(source, destination, std::filesystem::copy_options::recursive);

							//load new asset
							assetmanager->m_LoadAsset(destination/source.filename());
						}

					}
					// Clear the vector if you want to reset after displaying
					Input::InputSystem::m_droppedFiles.clear();
				}
			}




			ImGui::EndChild();
		}


		ImGui::End();
	}

}