#include "Window.h"
#include "Helper.h"

namespace Application {

	int AppWindow::init(){
        /* Initialize the library */
        if (!glfwInit())
            return -1;

        // Get the primary monitor
        //TOCHECK
        //naming of monitor and mode is same as member function
        //GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        //const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        // Create a window based on the current screen size
        WindowWidth = Helper::Helpers::GetInstance()->m_windowWidth;
        WindowHeight = Helper::Helpers::GetInstance()->m_windowHeight;
        
        //Set Context Version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);


       
        /* Create a windowed mode window and its OpenGL context */
        //Set third param to glfwGetPrimaryMonitor if you want fullscreen borderless

        Window = glfwCreateWindow(static_cast<int>(WindowWidth), static_cast<int>(WindowHeight), "Kos", NULL, NULL);
        if (!Window)
        {
            glfwTerminate();
            return -1;
        }
        /* Make the window's context current */
        glfwMakeContextCurrent(Window);

        /* Only initialize GLEW after defining OpenGL context*/
        if (glewInit() != GLEW_OK)
        {
            std::cout << "Failed to initialize GLEW" << std::endl;
            return -1;
        }

        
        return 0;
	}


	int AppWindow::Draw() {

        glClear(GL_COLOR_BUFFER_BIT);
        int display_w, display_h;
        glfwGetFramebufferSize(Window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(static_cast<GLclampf>(0.86), static_cast<GLclampf>(0.86), static_cast<GLclampf>(0.86), static_cast<GLclampf>(0.86));
        glClear(GL_COLOR_BUFFER_BIT);

        return 0;
	}

	int AppWindow::CleanUp() {

        glfwDestroyWindow(Window);
        return 0;
	}


}