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
#include "../Events/EventHandler.h"
#include "../Application/Helper.h"
#include "../Debugging/Performance.h"
#include "../Asset Manager/Prefab.h"

namespace gui {

	ImGuiHandler::ImGuiHandler() {
		REGISTER_BUTTON_LISTENER(events::ButtonEvents::EVENTBUTTONPRESS, ImGuiHandler::m_OnButtonPress, this)
		REGISTER_BUTTON_LISTENER(events::ButtonEvents::EVENTAUDIOFROMIMGUI, ImGuiHandler::m_OnButtonPress, this)
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


		
	}

	void ImGuiHandler::m_NewFrame()
	{
		// Start a new ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiHandler::m_Render()
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
				m_DrawTestWindow();
				m_DrawLayerWindow();
				m_DrawInputWindow();
				m_DrawContentBrowser();
				m_DrawRenderScreenWindow(static_cast<unsigned int>(Helper::Helpers::GetInstance()->m_windowWidth), static_cast<unsigned int>(Helper::Helpers::GetInstance()->m_windowHeight));
				m_DrawGameSceneWindow();
				auto end = std::chrono::steady_clock::now();
				duration = end - start;
				performancetracker::Performance::m_UpdateTotalSystemTime(duration.count());
				performancetracker::Performance::m_UpdateSystemTime(ecs::TYPEIMGUISYSTEM, duration.count());
				m_DrawPerformanceWindow(help->m_fps);
			}


			ImGuiIO& io = ImGui::GetIO();

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


	void ImGuiHandler::m_OnButtonPress(const events::BaseEvent<events::ButtonEvents>& givenEvent) {
		/***************************************************TO REMOVE, FOR TESTING ONLY*****************************************************************/
		if (givenEvent.m_ToType<events::ButtonPressEvent>().m_GetButton() == 1) {
			assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();
			assetManager->m_audioManager.m_soundMap.find("mindstorm.wav")->second->m_PlaySound();
		}else if (givenEvent.m_ToType<events::ButtonPressEvent>().m_GetButton() == 2) {
			assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();
			assetManager->m_audioManager.m_soundMap.find("zwing.wav")->second->m_PlaySound();
		}
		else if (givenEvent.m_ToType<events::ButtonPressEvent>().m_GetButton() == 3) {
			assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();
			assetManager->m_audioManager.m_soundMap.find("cleaver01.wav")->second->m_PlaySound();
		}
		else if (givenEvent.m_ToType<events::ButtonPressEvent>().m_GetButton() == 4) {
			assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();
			assetManager->m_audioManager.m_soundMap.find("gunshot01.wav")->second->m_PlaySound();
		}
		else if (givenEvent.m_ToType<events::ButtonPressEvent>().m_GetButton() == 5) {
			assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();
			assetManager->m_audioManager.m_soundMap.find("mindstorm.wav")->second->m_StopSound();
			assetManager->m_audioManager.m_soundMap.find("zwing.wav")->second->m_StopSound();
			assetManager->m_audioManager.m_soundMap.find("cleaver01.wav")->second->m_StopSound();
			assetManager->m_audioManager.m_soundMap.find("gunshot01.wav")->second->m_StopSound();
		}
		else if (givenEvent.m_ToType<events::AudioFromImgui>().m_GetEventType() == events::ButtonEvents::EVENTAUDIOFROMIMGUI) {\
			std::string fileToBePlayed = givenEvent.m_ToType<events::AudioFromImgui>().m_GetFile();
			assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();
			if (assetManager->m_audioManager.m_soundMap.find(fileToBePlayed)->second->m_IsPlaying()) {
				assetManager->m_audioManager.m_soundMap.find(fileToBePlayed)->second->m_StopSound();
			}
			else {
				assetManager->m_audioManager.m_soundMap.find(fileToBePlayed)->second->m_PlaySound();
			}
			
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

