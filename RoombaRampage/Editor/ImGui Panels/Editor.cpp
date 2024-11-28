/********************************************************************/
/*!
\file      imgui_handler.cpp
\author    Chiu Jun Jie, junjie.c , 2301524
\par       junjie.c@digipen.edu
\date      Oct 02, 2024
\brief     This file defines functions that handle ImGui operations
		   - Initialize: Sets up ImGui with GLFW and OpenGL.
		   - NewFrame: Prepares and starts a new ImGui frame.
		   - Render: Renders ImGui elements and windows.
		   - Shutdown: Cleans up ImGui resources.
		   - DrawMainMenuBar: Renders the main menu bar UI.
		   - DrawPerformanceWindow: Displays FPS and performance metrics.
		   - DrawComponentWindow: Manages the ECS component interface.
		   - DrawLogsWindow: Displays logs.
		   - DrawInputWindow: Handles input settings.
		   - DrawRenderScreenWindow: Sets up the window for render output.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../Config/pch.h"


#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "implot_internal.h"
#include "../Dependencies/rapidjson/document.h"
#include "../Dependencies/rapidjson/writer.h"
#include "../Dependencies/rapidjson/stringbuffer.h"
#include "Editor.h"
#include "../Application/Helper.h"
#include "../Editor/EditorCamera.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Inputs/Input.h"
#include "../Events/EventsEventHandler.h"
#include "../Application/Helper.h"
#include "../Debugging/Performance.h"
#include "../Asset Manager/Prefab.h"
#include "../Actions/ActionManager.h"
#include "../Actions/ModifyAction.h"
#include "../Inputs/Input.h"
#include "../Editor/WindowFile.h"

namespace gui {

	ImGuiHandler::ImGuiHandler() {
		REGISTER_BUTTON_LISTENER(events::ButtonEvents::EVENTBUTTONPRESS, ImGuiHandler::m_OnButtonPress, this)
		REGISTER_BUTTON_LISTENER(events::ButtonEvents::EVENTAUDIOFROMIMGUI, ImGuiHandler::m_OnButtonPress, this)
		REGISTER_ACTION_LISTENER(events::Actions::BASEACTION, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::TRANSFORMCOMP, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::UNDO, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::REDO, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::ADDCOMP, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::REMOVECOMP, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::ADDENT, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::DELENT, ImGuiHandler::m_OnAction, this)
	} //CTORdoing 

	ImGuiHandler::~ImGuiHandler() {} //Destructor

	void ImGuiHandler::m_Initialize(GLFWwindow* window, const char* glsl_version)
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;// Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;// Enable Multi-Viewport / Platform Windows

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		// General window settings
		style.WindowRounding = 5.0f;
		style.FrameRounding = 5.0f;
		style.ScrollbarRounding = 5.0f;
		style.GrabRounding = 5.0f;
		style.TabRounding = 5.0f;
		style.WindowBorderSize = 1.0f;
		style.FrameBorderSize = 1.0f;
		style.PopupBorderSize = 1.0f;
		style.PopupRounding = 5.0f;

		// Setting the colors (Light version)
		colors[ImGuiCol_Text] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f); // Light background
		colors[ImGuiCol_ChildBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f); // Light frame background
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);

		// Accent colors with a soft pastel gray-green
		colors[ImGuiCol_CheckMark] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f); // Soft gray-green for check marks
		colors[ImGuiCol_SliderGrab] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.60f, 0.70f, 0.60f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f); // Light button background
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f); // Accent color for resize grips
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.60f, 0.70f, 0.60f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.65f, 0.75f, 0.65f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f); // Tabs background
		colors[ImGuiCol_TabHovered] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f); // Docking preview in gray-green
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);

		// Additional styles
		style.FramePadding = ImVec2(8.0f, 4.0f);
		style.ItemSpacing = ImVec2(8.0f, 4.0f);
		style.IndentSpacing = 20.0f;
		style.ScrollbarSize = 16.0f;

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		//set first active scene
		ecs::ECS* ec = ecs::ECS::m_GetInstance();
		for (auto& scene : ec->m_ECS_SceneMap) {
			if (!scene.second.m_isPrefab) {
				m_activeScene = scene.first;
				break;
			}
		}

		m_tags = file::FileWindow::readEditorConfig("./Editor/editorConfig.txt");
		
	}

	void ImGuiHandler::m_NewFrame()
	{
		// Start a new ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiHandler::m_update()
	{
		if (Input::InputSystem::m_isKeyTriggered(keys::F11))
		{
			//std::cout << Input::InputSystem::m_isKeyTriggered(keys::F11) << std::endl;
			//Input::InputSystem::m_keyStateF11 = GLFW_RELEASE;
			graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
			EditorCamera::m_editorMode = (EditorCamera::m_editorMode) ? false : true;
			pipe->m_gameMode = (pipe->m_gameMode) ? false : true;
		}

		if (EditorCamera::m_editorMode)
		{
			// Render ImGui
			m_NewFrame();
			//for gizmo - todo once camera is done
			//ImGuizmo::SetOrthographic(true);
			
			//viewport docking
			ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

			//update while prefabmode is on
			m_UpdateOnPrefabMode();
			
			Helper::Helpers* help = Helper::Helpers::GetInstance();
			ecs::ECS* ecs = ecs::ECS::m_GetInstance();

			//check if "m_activeScene", if not find first active scene
			const auto& scene = ecs->m_ECS_SceneMap.find(m_activeScene);
			if (scene == ecs->m_ECS_SceneMap.end()) {
				for (auto& _scene : ecs->m_ECS_SceneMap) {
					if (!_scene.second.m_isPrefab) {
						m_activeScene = _scene.first;
						_scene.second.m_isActive = true;
					}
				}
			}

			//check if selected entityid is loaded
			if (ecs->m_ECS_EntityMap.find(m_clickedEntityId) == ecs->m_ECS_EntityMap.end()) {
				m_clickedEntityId = -1;
			}


			ImVec2 windowSize = ImGui::GetIO().DisplaySize;
			// only render when window is not minimize
			if (windowSize.x > 0 && windowSize.y > 0) {
				std::chrono::duration<float> duration{};
				auto start = std::chrono::steady_clock::now();
				m_DrawMainMenuBar();
				//m_DrawPlayPauseWindow();
				m_DrawHierachyWindow();
				m_DrawComponentWindow();
				m_DrawLogsWindow();
				//m_DrawTestWindow();
				m_DrawLayerWindow();
				m_DrawInputWindow();
				m_DrawContentBrowser();
				m_DrawRenderScreenWindow(static_cast<unsigned int>(Helper::Helpers::GetInstance()->m_windowWidth), static_cast<unsigned int>(Helper::Helpers::GetInstance()->m_windowHeight));
				m_DrawGameSceneWindow();
				auto end = std::chrono::steady_clock::now();
				duration = end - start;
				performancetracker::Performance::m_UpdateTotalSystemTime(duration.count());
				performancetracker::Performance::m_UpdateSystemTime(ecs::TOTALTYPESYSTEM, duration.count());
				m_DrawPerformanceWindow(help->m_fps);
			}

			
			if (Input::InputSystem::m_isKeyPressed(keys::LeftControl) && Input::InputSystem::m_isKeyTriggered(keys::Z)) {
				events::UndoLatest temp;
				DISPATCH_ACTION_EVENT(temp);
			}

			if (Input::InputSystem::m_isKeyPressed(keys::LeftControl) && Input::InputSystem::m_isKeyTriggered(keys::Y)) {
				events::RedoPrevious temp;
				DISPATCH_ACTION_EVENT(temp);
			}

			
		}
	
	}

	void ImGuiHandler::m_render()
	{
		ImGuiIO& io = ImGui::GetIO();

		if (EditorCamera::m_editorMode)
		{
			ImGui::Render();

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
	}




	void ImGuiHandler::m_Shutdown()
	{
		// Shutdown ImGui
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}


	void ImGuiHandler::m_OnButtonPress([[maybe_unused]]const events::BaseEvent<events::ButtonEvents>& givenEvent) {
		assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();

		if (!assetManager) {
			std::cerr << "AssetManager instance is null.\n";
			return;
		}

		//auto& soundMap = assetManager->m_audioManager.getSoundMap();

		/*auto playSound = [&](const std::string& fileName) {
			auto it = soundMap.find(fileName);
			if (it != soundMap.end()) {
				it->second->m_PlaySound();
			}
			else {
				std::cerr << "Sound file not found: " << fileName << "\n";
			}
			};*/

		/*auto stopSound = [&](const std::string& fileName) {
			auto it = soundMap.find(fileName);
			if (it != soundMap.end()) {
				it->second->m_StopSound();
			}
			else {
				std::cerr << "Sound file not found: " << fileName << "\n";
			}
			};*/

		//int button = givenEvent.m_ToType<events::ButtonPressEvent>().m_GetButton();

		/*if (button == 1) {
			playSound("mindstorm.wav");
		}
		else if (button == 2) {
			playSound("zwing.wav");
		}
		else if (button == 3) {
			playSound("cleaver01.wav");
		}
		else if (button == 4) {
			playSound("gunshot01.wav");
		}
		else if (button == 5) {
			stopSound("mindstorm.wav");
			stopSound("zwing.wav");
			stopSound("cleaver01.wav");
			stopSound("gunshot01.wav");
		}
		else if (givenEvent.m_ToType<events::AudioFromImgui>().m_GetEventType() == events::ButtonEvents::EVENTAUDIOFROMIMGUI) {
			std::string fileToBePlayed = givenEvent.m_ToType<events::AudioFromImgui>().m_GetFile();
			auto it = soundMap.find(fileToBePlayed);

			if (it != soundMap.end()) {
				if (it->second->m_IsPlaying()) {
					it->second->m_StopSound();
				}
				else {
					it->second->m_PlaySound();
				}
			}
			else {
				std::cerr << "Sound file not found: " << fileToBePlayed << "\n";
			}
		}*/
	}


	void ImGuiHandler::m_OnAction(const events::BaseEvent<events::Actions>& givenEvent) {
		if (givenEvent.m_GetEventType() == events::Actions::TRANSFORMCOMP) {
			auto* newAct = new actions::ModifyTransformAction(givenEvent.m_ToType<events::TransformComponentChanged>().m_GetID(), givenEvent.m_ToType<events::TransformComponentChanged>().m_GetComp(),
															  givenEvent.m_ToType<events::TransformComponentChanged>().m_GetOldPos(), givenEvent.m_ToType<events::TransformComponentChanged>().m_GetOldRot(),
															  givenEvent.m_ToType<events::TransformComponentChanged>().m_GetOldScale(), givenEvent.m_ToType<events::TransformComponentChanged>().m_GetOldTrans());
			//ModifyTransformAction(ecs::EntityID inID, ecs::TransformComponent* inComp, vector2::Vec2 inOldPos, float inOldRot, vector2::Vec2 inOldScale, mat3x3::Mat3x3 inOldTrans)
			actions::ActionManager::m_GetManagerInstance()->m_DoAction(newAct);
		}else if (givenEvent.m_GetEventType() == events::Actions::ADDCOMP) {
			auto* newAct = new actions::AddComponentAction(givenEvent.m_ToType<events::AddComponent>().m_GetID(), givenEvent.m_ToType<events::AddComponent>().m_GetComponentType());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}
		else if (givenEvent.m_GetEventType() == events::Actions::REMOVECOMP) {
			auto* newAct = new actions::RemoveComponentAction(givenEvent.m_ToType<events::RemoveComponent>().m_GetID(), givenEvent.m_ToType<events::RemoveComponent>().m_GetComponentType());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}
		else if (givenEvent.m_GetEventType() == events::Actions::ADDENT) {
			auto* newAct = new actions::AddEntityAction(givenEvent.m_ToType<events::AddEntity>().m_GetID());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}
		else if (givenEvent.m_GetEventType() == events::Actions::DELENT) {
			auto* newAct = new actions::RemoveEntityAction(givenEvent.m_ToType<events::RemoveEntity>().m_GetID());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}
		else if (givenEvent.m_GetEventType() == events::Actions::UNDO) {
			actions::ActionManager::m_GetManagerInstance()->m_Undo();
		}
		else if (givenEvent.m_GetEventType() == events::Actions::REDO) {
			actions::ActionManager::m_GetManagerInstance()->m_Redo();

		}
		
	}

	void ImGuiHandler::m_UpdateOnPrefabMode()
	{
		Helper::Helpers* help = Helper::Helpers::GetInstance();
		help->m_colour = { 0.86f, 0.86f, 0.86f };
		//return if not on prefab mode
		if (m_prefabSceneMode == false) return;

		help->m_colour = { 0.133f, 0.23f, 0.32f };

		//update all entity with prefab
		prefab::Prefab::m_UpdateAllPrefabEntity(m_activeScene);



	}
}

