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

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "implot_internal.h"
#include "../Dependencies/rapidjson/document.h"
#include "../Dependencies/rapidjson/writer.h"
#include "../Dependencies/rapidjson/stringbuffer.h"
#include "imgui_handler.h"
#include "../Application/Helper.h"
#include "../Editor/EditorCamera.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Inputs/Input.h"
#include "../Events/EventHandler.h"

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
			//create main menu bar
			m_DrawMainMenuBar();

			Helper::Helpers* help = Helper::Helpers::GetInstance();

			ImVec2 windowSize = ImGui::GetIO().DisplaySize;
			// only render when window is not minimize
			if (windowSize.x > 0 && windowSize.y > 0) {
				m_DrawPerformanceWindow(help->m_fps);
				m_DrawPlayPauseWindow();
				m_DrawHierachyWindow();
				m_DrawComponentWindow();
				m_DrawLogsWindow();
				m_DrawTestWindow();
				m_DrawLayerWindow();
				m_DrawInputWindow();
				m_DrawContentBrowser();
				m_DrawRenderScreenWindow(static_cast<unsigned int>(Helper::Helpers::GetInstance()->m_windowWidth), static_cast<unsigned int>(Helper::Helpers::GetInstance()->m_windowHeight));
				m_DrawGameSceneWindow();
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
}

