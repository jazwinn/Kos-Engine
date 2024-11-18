#include "../Config/pch.h"
#include "Editor.h"

#include <filesystem>
#include <string>
#include "../Asset Manager/AssetManager.h"
#include "../Asset Manager/SceneManager.h"
#include "../Inputs/Input.h"
#include <imgui_internal.h>

namespace gui {

	static std::filesystem::path assetDirectory = "Assets";
	static std::filesystem::path currentDirectory  = assetDirectory;
	static const char* fileIcon = "folder.png";

	void ImGuiHandler::m_DrawContentBrowser() {

		ImGui::Begin("Content Browser");
		
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();

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
		if (m_tilePickerMode && ecs->m_ECS_CombinedComponentPool[ecs::TYPETILEMAPCOMPONENT]->m_HasComponent(m_clickedEntityId))
		{
			m_DrawTilePicker();
		}
		else
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
			static float thumbnail = 80.f;
			float cellsize = padding + thumbnail;

			float panelwidth = ImGui::GetContentRegionAvail().x;
			int columns = (int)(panelwidth / cellsize);
			if (columns <= 0) {
				columns = 1;
			}
			ImGui::Columns(columns, 0, false);

			auto textorimage = [](std::string directoryString, std::string fileName) {
				assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
				if (assetmanager->m_imageManager.m_imageMap.find(fileName) != assetmanager->m_imageManager.m_imageMap.end()) {
					ImGui::ImageButton(directoryString.c_str(), (ImTextureID)(uintptr_t)assetmanager->m_imageManager.m_imageMap.find(fileName)->second.textureID, { thumbnail ,thumbnail }, { 0,1 }, { 1,0 }, { 0,0,0,0 });
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
							//TODO play sound
							events::AudioFromImgui audioDblClick(directoryPath.path().filename().string());
							DISPATCH_BUTTON_EVENT(audioDblClick);
						}
					}
					else if (directoryPath.path().filename().extension().string() == ".cs") {
						std::string script = "ScriptIcon.png";
						textorimage(directoryString, script);

						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
							std::string command = "code \"" + directoryPath.path().string() + "\"";

							// Execute the command to open the file
							int result = std::system(command.c_str());

							if (result != 0) {
								LOGGING_ERROR("Fail to open .cs script");
							}

						}
					}
					else if (directoryPath.path().filename().extension().string() == ".prefab") {
						std::string prefab = "";
						textorimage(std::string(directoryPath.path().filename().extension().string() + "##" + directoryPath.path().filename().string()).c_str(), prefab);

						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
							//skip if active scene is filename
							if (m_activeScene == directoryPath.path().filename())continue;

							const auto& prefabscene = ecs->m_ECS_SceneMap.find(directoryPath.path().filename().string());
							if (prefabscene == ecs->m_ECS_SceneMap.end()) {
								LOGGING_ERROR("Prefab not loaded");
								continue;
							}
							else {
								m_prefabSceneMode = true;
							}

							//skip if prefab mode alraedy true
							if (!m_prefabSceneMode) {
								m_savedSceneState.clear();
								for (auto& scene : ecs->m_ECS_SceneMap) {
									if (scene.second.m_isPrefab == false) {
										//save all scenes active state
										m_savedSceneState[scene.first] = scene.second.m_isActive;
									}
								}

							}

							

							// clear save scene state

							// unload all regular scenes
							for (auto& scene : ecs->m_ECS_SceneMap) {
								scene.second.m_isActive = false;
							}

							// set prefab to active
							prefabscene->second.m_isActive = true;

							//set prefab as active scene
							m_activeScene = directoryPath.path().filename().string();

							m_clickedEntityId = -1;
						}
					}
					else if (directoryPath.path().filename().extension().string() == ".json") {
						std::string script;
						textorimage(directoryString, script);

						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {

							scenemanager->m_ClearAllScene();
							scenemanager->m_LoadScene(directoryPath.path());
							if (!m_prefabSceneMode) {
								m_activeScene = directoryPath.path().filename().string();
							}
							else {
								ecs::ECS::m_GetInstance()->m_ECS_SceneMap.find(directoryPath.path().filename().string())->second.m_isActive = false;
								m_savedSceneState[directoryPath.path().filename().string()] = true;
							}

							m_clickedEntityId = -1;

						}
					}
					else if (ImGui::Button(std::string(directoryPath.path().filename().extension().string() + "##" + directoryPath.path().filename().string()).c_str(), {thumbnail ,thumbnail})) {

						
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
				static bool _delete = false;
				static std::filesystem::path selectedfile{};
				if (ImGui::BeginPopupContextItem()) {
					if (ImGui::MenuItem("Rename")) {
						rename = true;
						selectedfile = directoryString;
					}
					if (ImGui::MenuItem("Delete")) {
						std::filesystem::remove_all(directoryPath.path());
						_delete = true;
						selectedfile = directoryString;
					}
					ImGui::EndPopup();
				}


				if (rename && (selectedfile.string() == directoryString)) {
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
						selectedfile.clear();

						//TODO edge cases,
						//Update assets if any of them are renamed
					}
				}
				else {
					ImGui::SetWindowFontScale(0.8f);
					ImGui::Text(directoryPath.path().filename().stem().string().c_str());
					ImGui::SetWindowFontScale(1.f);
				}

				if (_delete){
					
					if (selectedfile.filename().extension().string() == ".cs") {

						//erease script from scriptmanager
					
						//assetmanager->m_scriptManager.m_CSScripts.erase(std::find(assetmanager->m_scriptManager.m_CSScripts.begin(), assetmanager->m_scriptManager.m_CSScripts.end(), selectedfile.filename().stem().string()));

					}
					

					_delete = false;
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

							if (std::filesystem::exists(destination/source.filename())) {
								LOGGING_WARN("Directory already in folder");
								continue;
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

			//if (ImGui::BeginDrapDropTargetWindow("Entity"))
			//{


			//	ImGui::EndDragDropTarget();
			//}


			ImGui::EndChild();
		}


		ImGui::End();
	}

}