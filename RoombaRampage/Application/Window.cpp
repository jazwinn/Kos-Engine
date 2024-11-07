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

namespace Application {

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
        //Set third param to glfwGetPrimaryMonitor if you want fullscreen borderless

       m_window = glfwCreateWindow(static_cast<int>(m_windowWidth), static_cast<int>(m_windowHeight), "Kos", NULL, NULL);
       Input::InputSystem::m_windowInput = m_window;
        if (!m_window)
        {
            glfwTerminate();
            return -1;
        }
        /* Make the window's context current */
        glfwMakeContextCurrent(m_window);

        /* Only initialize GLEW after defining OpenGL context*/
        if (glewInit() != GLEW_OK)
        {
            std::cout << "Failed to initialize GLEW" << std::endl;
            return -1;
        }

        
        return 0;
	}


	int AppWindow::Draw() {
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
        glClearColor(static_cast<GLclampf>(0.86), static_cast<GLclampf>(0.86), static_cast<GLclampf>(0.86), static_cast<GLclampf>(0.86));
        glClear(GL_COLOR_BUFFER_BIT);

       /* double xPos, yPos;
        glfwGetCursorPos(m_window, &xPos, &yPos);
        xPos = glm::clamp(xPos, 0.0, (double)display_w);
        yPos = glm::clamp(yPos, 0.0, (double)display_h);
        glfwSetCursorPos(m_window, xPos, yPos);*/

        return 0;
	}

	int AppWindow::CleanUp() {

        glfwDestroyWindow(m_window);
        return 0;
	}

  

}