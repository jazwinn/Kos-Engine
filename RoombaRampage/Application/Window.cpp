/******************************************************************/
/*!
\file      Window.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      2nd Oct, 2024
\brief

           This file contains the implementation of the `AppWindow` class
           methods, which handle the window lifecycle including initialization,
           drawing/rendering loop, and cleanup.

           The `init` method sets up GLFW and OpenGL contexts for rendering,
           while the `Draw` method manages the rendering loop including
           ImGui interface rendering. The `CleanUp` method is responsible
           for releasing resources used during the window’s operation.

           The application window makes use of GLFW for window and context
           management, and OpenGL for rendering. ImGui is integrated to
           provide a graphical interface within the application.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#include "../Config/pch.h"
#include "Window.h"
#include "Helper.h"
#include "../Graphics/GraphicsPipe.h"
#include "../ECS/ECS.h"
#include "../Inputs/Input.h"

namespace Application {

    float AppWindow::m_windowHeight;

    float AppWindow::m_windowWidth;

    bool AppWindow::m_fullScreen{ true };

    const GLFWvidmode* AppWindow::m_mode;

    GLFWmonitor* AppWindow::m_monitor;


    static void windowFocusCallback(GLFWwindow* window, int focused)
    {
        if (!focused) {
            // If the window loses focus, set it to windowed mode
            glfwSetWindowMonitor(window, nullptr, 100, 100, static_cast<int>(AppWindow::m_windowWidth), static_cast<int>(AppWindow::m_windowHeight), 0);  // Change to windowed mode with a standard resolution
            AppWindow::m_fullScreen = false;
        }
        else if (!AppWindow::m_fullScreen) {
            // If the window regains focus, switch back to full screen
            glfwSetWindowMonitor(window, AppWindow::m_monitor, 0, 0, AppWindow::m_mode->width, AppWindow::m_mode->height, AppWindow::m_mode->refreshRate);
            AppWindow::m_fullScreen = true;
        }

    }

    static void iconifyCallback([[maybe_unused]]GLFWwindow* window, int iconified)
    {
        auto& audioManager = assetmanager::AssetManager::m_funcGetInstance()->m_audioManager;
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        if (iconified == GLFW_TRUE)
        {
            
            audioManager.m_PauseAllSounds();  // Pause all sounds

            if (ecs->m_getState() == ecs::RUNNING) {
                //std::cout << "Window minimized!" << std::endl;
                ecs::ECS::m_GetInstance()->m_nextState = ecs::WAIT;
            }
        }
        else
        {
            
            audioManager.m_UnpauseAllSounds();  // Unpause all sounds

            if (ecs->m_getState() == ecs::WAIT) {
                //std::cout << "Window restored!" << std::endl;
                ecs::ECS::m_GetInstance()->m_nextState = ecs::RUNNING;
            }
        }
    }

	int AppWindow::init(){
        /* Initialize the library */
        if (!glfwInit())
            return -1;

        // Create a window based on the current screen size
        m_windowWidth = Helper::Helpers::GetInstance()->m_windowWidth;
        m_windowHeight = Helper::Helpers::GetInstance()->m_windowHeight;
        
        //Set Context Version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

        /* Create a windowed mode window and its OpenGL context */

        m_monitor = glfwGetPrimaryMonitor();
        m_mode = glfwGetVideoMode(m_monitor);
        m_window = glfwCreateWindow(static_cast<int>(m_windowWidth), static_cast<int>(m_windowHeight), "Kos", m_enabledFullScreen ?m_monitor : NULL , NULL);

        Input::InputSystem::m_windowInput = m_window;
        if (!m_window)
        {
            glfwTerminate();
            return -1;
        }
        //set call back
        if(m_enabledFullScreen) glfwSetWindowFocusCallback(m_window, windowFocusCallback);
        glfwSetWindowIconifyCallback(m_window, iconifyCallback);

        /* Make the window's context current */
        glfwMakeContextCurrent(m_window);

        /* Only initialize GLEW after defining OpenGL context*/
        if (glewInit() != GLEW_OK)
        {
            LOGGING_ERROR("Failed to initialize GLEW");
            return -1;
        }

        
        return 0;
	}




	int AppWindow::Draw() {

        if (Input::InputSystem::m_isKeyPressed(keys::LeftAlt) && Input::InputSystem::m_isKeyTriggered(keys::ENTER)) {
            if (m_enabledFullScreen) {
                glfwSetWindowFocusCallback(m_window, NULL);
                glfwSetWindowMonitor(m_window, nullptr, 100, 100, static_cast<int>(AppWindow::m_windowWidth), static_cast<int>(AppWindow::m_windowHeight), 0);
                m_enabledFullScreen = false;
            }
            else {
                glfwSetWindowFocusCallback(m_window, windowFocusCallback);
                glfwSetWindowMonitor(m_window, AppWindow::m_monitor, 0, 0, AppWindow::m_mode->width, AppWindow::m_mode->height, AppWindow::m_mode->refreshRate);

                m_enabledFullScreen = true;
            }
        }


        Helper::Helpers *help = Helper::Helpers::GetInstance();
        glClear(GL_COLOR_BUFFER_BIT);

        int display_w, display_h;
        glfwGetFramebufferSize(m_window, &display_w, &display_h);
        static GLint old_w{}, old_h{};
        // update viewport settings in vps only if window's dimension change
        if (display_w != old_w || display_h != old_h)
        {
            graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
            
            old_w = display_w;
            old_h = display_h;
            help->m_windowHeight = static_cast<float>(display_h);
            help->m_windowWidth = static_cast<float>(display_w);
            pipe->m_funcSetupFrameBuffer();
            pipe->m_funcSetupGamePreviewFrameBuffer();
            glViewport(0, 0, display_w, display_h);
            
        }
        glClearColor(static_cast<GLclampf>(help->m_colour.m_x), static_cast<GLclampf>(help->m_colour.m_y), static_cast<GLclampf>(help->m_colour.m_z), static_cast<GLclampf>(1));
        glClear(GL_COLOR_BUFFER_BIT);

        if (help->m_closeWindow) {
            glfwSetWindowShouldClose(m_window, GLFW_TRUE);
        }

        return 0;
	}

	int AppWindow::CleanUp() {

        glfwDestroyWindow(m_window);
        return 0;
	}

  

}