#include "Window.h"


namespace Application {

	int AppWindow::init(){
        /* Initialize the library */
        if (!glfwInit())
            return -1;

        // Get the primary monitor
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        // Create a window based on the current screen size
        int windowWidth = mode->width;
        int windowHeight = mode->height;

        /* Create a windowed mode window and its OpenGL context */
        //Set third param to glfwGetPrimaryMonitor if you want fullscreen borderless

        Window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
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


	int AppWindow::Draw(ImVec4 ClearColor) {

        glClear(GL_COLOR_BUFFER_BIT);
        int display_w, display_h;
        glfwGetFramebufferSize(Window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(ClearColor.x * ClearColor.w, ClearColor.y * ClearColor.w, ClearColor.z * ClearColor.w, ClearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);


        return 0;
	}

	int AppWindow::CleanUp() {

        glfwDestroyWindow(Window);
        return 0;
	}


}