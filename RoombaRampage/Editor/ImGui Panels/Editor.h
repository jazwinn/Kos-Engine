/********************************************************************/
/*!
\file      Editor.h
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
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuizmo.h"
#include "../ECS/ECS.h"
#include "../Asset Manager/Audio.h"
#include "../Events/EventHandler.h"



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

    private:

        /******************************************************************/
        /*!         
            \fn        gui::ImGuiHandler::m_DrawMainMenuBar()
            \brief     Renders the main menu bar and handles user input for file operations.
            \details   This function creates a main menu bar with options like "File" and "Window."
                       In the "File" menu, users can save the current state by either selecting the "Save" option
                       or pressing `CTRL + S`. The function interacts with the ECS framework to save component data
                       to a JSON file using the `Serialize::m_SaveComponentsJson` function.
        */
        /******************************************************************/
        void m_DrawMainMenuBar();

        /******************************************************************/
        /*!
            \fn        gui::ImGuiHandler::m_DrawPerformanceWindow(float fps)
            \brief     Displays the performance metrics in an ImGui window.
            \param[in] fps The current frames-per-second value.
            \details   This function renders a window showing performance statistics. It tracks FPS over time and
                       displays an FPS graph using `ImGui::PlotLines`. It also displays system time information
                       for the ECS systems, updating every second to show the percentage of total system time each
                       system uses, along with the actual time in seconds.
        */
        /******************************************************************/

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

        /******************************************************************/
        /*!
            \fn        gui::ImGuiHandler::m_DrawTestWindow()
            \brief     Displays a test window for various ECS and ImGui functionalities.
            \details   This function creates a test window with several buttons and options to:
                       - Play, stop, and control audio through the asset manager.
                       - Trigger an application crash using the `abort()` function.
                       - Test log functionality by adding log entries.
                       - Spawn and delete 2500 entities in the ECS.
                       - Perform a collision test by spawning two entities with movement and collider components.
                       - Control entity movement and reset their positions for collision testing.
        */
        /******************************************************************/
        void m_DrawTestWindow();

        /******************************************************************/
        /*!
            \fn        gui::ImGuiHandler::m_DrawInputWindow()
            \brief     Displays the input status window in ImGui.
            \details   This function creates a window that displays the current mouse position,
                       key status, and mouse button status. It retrieves these values from
                       the `Input::InputSystem` and outputs them as text within the ImGui window.
        */
        /******************************************************************/
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

        bool m_DrawEntityNode(ecs::EntityID);

        /******************************************************************/
        /*!
            \fn        gui::ImGuiHandler::m_DrawLogsWindow()
            \brief     Displays the log output in an ImGui window.
            \details   This function creates a window that displays log entries in reverse order, with the most recent log entries shown first.
                       It retrieves the log entries from the logger instance and outputs them within an ImGui window.
        */
        /******************************************************************/
        void m_DrawLogsWindow();

        /******************************************************************/
        /*!
        \fn        void gui::ImGuiHandler::m_DrawRenderScreenWindow(unsigned int windowWidth, unsigned int windowHeight)
        \brief     Renders the scene texture inside an ImGui window while maintaining the correct aspect ratio.

        \param[in] windowWidth
                   The width of the OpenGL window (scene) being rendered.

        \param[in] windowHeight
                   The height of the OpenGL window (scene) being rendered.

        \details   This function creates an ImGui window labeled "Scene Window" and renders
                   the scene as a texture inside it. The function calculates the appropriate
                   size and position for the texture within the window, ensuring that the
                   aspect ratio is preserved when the window is resized.

                   It uses the available content region size within the ImGui window to
                   adjust the texture size. If the aspect ratio of the texture differs from
                   that of the window, the texture is centered horizontally or vertically.

        \sa        graphicpipe::GraphicsPipe::m_funcGetInstance() to get the screen texture.
        */
        /******************************************************************/
        void m_DrawPlayPauseBar();

        void m_DrawRenderScreenWindow(unsigned int windowWidth, unsigned int windowHeight);

        void m_DrawGizmo(float renderPosX, float renderPosY, float renderWidth, float renderHeight);

        void m_DrawLayerWindow();

        void m_DrawGameSceneWindow();
        
        void m_DrawContentBrowser();

        void m_OnButtonPress(const events::BaseEvent<events::ButtonEvents>& givenEvent);

        void m_OnAction(const events::BaseEvent<events::Actions>& givenEvent);

        void m_UpdateOnPrefabMode();
        

        int m_clickedEntityId{ -1 };

        std::string m_activeScene{};

        /***********PREFAB*************/
        bool m_prefabSceneMode{ false };
        std::unordered_map<std::string, bool> m_savedSceneState;

        bool m_objectNameBox = false;
        char m_charBuffer[50] = "";  // Char Buffer

        ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    };
}