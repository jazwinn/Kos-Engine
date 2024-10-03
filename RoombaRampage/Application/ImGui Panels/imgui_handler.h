/********************************************************************/
/*!
\file      imgui_handler.h
\author    Chiu Jun Jie, junjie.c , 2301524
\par       junjie.c@digipen.edu
\date      Oct 02, 2024
\brief     This header file declares functions that manage ImGui operations
           - Initialize: Sets up ImGui with GLFW and OpenGL.
           - NewFrame: Prepares ImGui for rendering.
           - Render: Draws ImGui elements.
           - Shutdown: Cleans up ImGui resources.
           - DrawMainMenuBar: Renders the main menu bar UI.
           - DrawPerformanceWindow: Displays performance metrics like FPS.
           - DrawComponentWindow: Manages the ECS component user interface.
           - DrawInputWindow: Displays user input settings.
           - DrawLogsWindow: Displays log output.
           - DrawRenderScreenWindow: Sets up the window for rendering output.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#pragma once
#include<vector>
#include<string>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "../ECS/ECS.h"
#include "../Assets/Audio.h"

namespace gui {

    class ImGuiHandler
    {
    public:
        //CTOR & Destructor
        ImGuiHandler();
        ~ImGuiHandler();

        void m_Initialize(GLFWwindow* window, const char* glsl_version);
        void m_NewFrame();
        void m_Render();
        void m_Shutdown();

        void m_DrawMainMenuBar();
        void m_DrawPerformanceWindow(float fps);
        void m_DrawComponentWindow();
        void m_DrawTestWindow();
        void m_DrawInputWindow();
        unsigned int m_DrawHierachyWindow();
        void m_DrawLogsWindow();
        void m_DrawRenderScreenWindow(unsigned int windowWidth, unsigned int windowHeight);

        //Getter to acess the four imgui variables
        std::vector<std::string>& GetObjectTextEntries() { return m_objTextEntries; }
        std::vector<bool>& GetDeleteButtons() { return m_deleteButton; }
        std::vector<bool>& GetDuplicateButtons() { return m_duplicateButton; }
        std::vector<unsigned int>& GetObjectEntityIDs() { return m_objEntityId; }

        int m_clickedEntityId;

    private:

        bool m_objectNameBox = false;
        char m_charBuffer[50] = "";  // Char Buffer
        std::vector<std::string> m_objTextEntries;
        std::vector<bool> m_deleteButton;
        std::vector<bool> m_duplicateButton;
        std::vector<unsigned int> m_objEntityId;

        ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    };
}