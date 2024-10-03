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

        /******************************************************************/
        /*!
            \fn        gui::ImGuiHandler::m_Initialize(GLFWwindow* window, const char* glsl_version)
            \brief     Initializes ImGui with GLFW and OpenGL settings.
            \param[in] window       The window handle used by GLFW.
            \param[in] glsl_version The OpenGL GLSL version string.
            \details   Sets up the ImGui context, configures platform and renderer bindings,
                       enables various ImGui features (such as keyboard, gamepad support, and
                       multi-viewport windows), and applies the dark style theme.
        */
        /******************************************************************/
        void m_Initialize(GLFWwindow* window, const char* glsl_version);

        /******************************************************************/
        /*!
            \fn        gui::ImGuiHandler::m_NewFrame()
            \brief     Prepares and starts a new ImGui frame.
            \details   This function is responsible for starting a new frame in ImGui by
                        initializing both OpenGL and GLFW backends and preparing for ImGui rendering.
        */
        /******************************************************************/
        void m_NewFrame();

        /******************************************************************/
        /*!
            \fn        gui::ImGuiHandler::m_Render()
            \brief     Renders ImGui elements and windows.
            \details   This function manages the main rendering loop for ImGui, handling the
                        menu bar, performance window, hierarchy, component, and log windows.
                        It also manages ImGui's viewport and platform-specific rendering.
        */
        /******************************************************************/
        void m_Render();

        /******************************************************************/
        /*!
            \fn        gui::ImGuiHandler::m_Shutdown()
            \brief     Cleans up and shuts down ImGui.
            \details   This function handles shutting down ImGui by cleaning up the OpenGL
                       and GLFW resources, destroying the ImGui and ImPlot contexts.
        */
        /******************************************************************/
        void m_Shutdown();

        void m_DrawMainMenuBar();
        void m_DrawPerformanceWindow(float fps);

        /******************************************************************/
        /*!
            \fn        gui::ImGuiHandler::m_DrawComponentWindow()
            \brief     Draws the ImGui component window, allowing users to interact with and modify ECS components.
            \details   Displays available components for the selected entity and provides options to add, view,
                       and modify components such as Movement, Transform, Sprite, Collider, and more.
                       The window updates real-time changes to the entity's components within the ECS framework.
        */
        /******************************************************************/
        void m_DrawComponentWindow();
        void m_DrawTestWindow();
        void m_DrawInputWindow();

        /******************************************************************/
        /*!
            \fn        gui::ImGuiHandler::DrawHierachyWindow()
            \brief     Draws the ImGui Hierarchy Window for managing game objects in the ECS.
            \return    The ID of the clicked entity in the hierarchy.
            \details   Displays a list of game objects (entities) with options to add, delete,
                       or duplicate them. The function updates the hierarchy view in real time
                   and interacts with the ECS framework.
        */
        /******************************************************************/
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