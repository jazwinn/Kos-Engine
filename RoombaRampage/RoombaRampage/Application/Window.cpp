#include "Window.h"



namespace Application {
    //temp commented
	//int AppWindow::init(){
 //       /* Initialize the library */
 //       if (!glfwInit())
 //           return -1;

 //       // Get the primary monitor
 //       GLFWmonitor* monitor = glfwGetPrimaryMonitor();
 //       const GLFWvidmode* mode = glfwGetVideoMode(monitor);

 //       // Create a window based on the current screen size
 //       int windowWidth = mode->width;
 //       int windowHeight = mode->height;

 //       /* Create a windowed mode window and its OpenGL context */
 //       //Set third param to glfwGetPrimaryMonitor if you want fullscreen borderless

 //       Window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
 //       if (!Window)
 //       {
 //           glfwTerminate();
 //           return -1;
 //       }
 //       /* Make the window's context current */
 //       glfwMakeContextCurrent(Window);

 //       /* Only initialize GLEW after defining OpenGL context*/
 //       if (glewInit() != GLEW_OK)
 //       {
 //           std::cout << "Failed to initialize GLEW" << std::endl;
 //           return -1;
 //       }

 //       return 0;
	//}

    static bool gvGLFWInit = false;

    static void funcErrorCallback(int givenErrorCode, const char* reason) {
        std::cerr << "GLFW error: " << reason << std::endl;
    }

    AppWindow* AppWindow::funcCreateWindow(const winProperties& givenProperties) {
        return new AppWindow(givenProperties);
    }

    AppWindow::AppWindow(const winProperties& givenProperties) {
        AppWindow::funcWinInit(givenProperties);
    }

    AppWindow::~AppWindow() {
        AppWindow::funcWinShutdown();
    }

	void AppWindow::funcWinInit(const winProperties& givenProperties) {
		lvWinData.lvTitle = givenProperties.lvWinTitle;
		lvWinData.lvWidth = givenProperties.lvWinWidth;
		lvWinData.lvHeight = givenProperties.lvWinHeight;

		if (!gvGLFWInit) {
			int lvInit = glfwInit();
			if (lvInit == false) {
				std::cerr << "GLFW unable to initialize\n";
				glfwTerminate();
				return;
			}
		}
		lvWin = glfwCreateWindow(givenProperties.lvWinWidth, givenProperties.lvWinHeight, givenProperties.lvWinTitle.c_str(), nullptr, nullptr);
		if (!lvWin) {
			std::cerr << "GLFW unable to create openGL context\n";
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(lvWin);
		GLenum lvError = glewInit();
		if (lvError != GLEW_OK) {
			std::cerr << "GLEW unable to initalize due to " << glewGetErrorString(lvError) << std::endl;
			return;
		}
		glfwSetWindowUserPointer(lvWin, &lvWinData);
		funcSetVSync(false);

		glfwSetWindowSizeCallback(lvWin, [](GLFWwindow* givenWin, int givenW, int givenH) {
			classWinData& lvData = *static_cast<classWinData*>(glfwGetWindowUserPointer(givenWin));
			lvData.lvWidth = givenW;
			lvData.lvHeight = givenH;
			classWindowResizeEvent lvEvent(givenW, givenH);
			lvData.lvEventCallback(lvEvent);
		});

		glfwSetWindowCloseCallback(lvWin, [](GLFWwindow* givenWin) {
			classWinData& lvData = *static_cast<classWinData*>(glfwGetWindowUserPointer(givenWin));
			classWindowClosedEvent lvEvent;
			lvData.lvEventCallback(lvEvent);
		});

		glfwSetKeyCallback(lvWin, [](GLFWwindow* givenWin, int givenKey, [[maybe_unused]] int scanCode, int givenAction, [[maybe_unused]] int mods) {
			classWinData& lvData = *static_cast<classWinData*>(glfwGetWindowUserPointer(givenWin));

			switch (givenAction) {
				case GLFW_PRESS:
				{
					classKeyPressedEvent lvEvent(givenKey, false);
					lvData.lvEventCallback(lvEvent);
					break;
				}
				case GLFW_REPEAT: 
				{
					classKeyPressedEvent lvEvent(givenKey, true);
					lvData.lvEventCallback(lvEvent);
					break;
				}
				case GLFW_RELEASE:
				{
					classKeyReleasedEvent lvEvent(givenKey);
					lvData.lvEventCallback(lvEvent);
					break;
				}
			}
		});

		glfwSetMouseButtonCallback(lvWin, [](GLFWwindow* givenWin, int givenKey,  int givenAction, [[maybe_unused]] int mods) {
			classWinData& lvData = *static_cast<classWinData*>(glfwGetWindowUserPointer(givenWin));

			switch (givenAction) {
				case GLFW_PRESS:
				{
					classMouseClickEvent lvEvent(givenKey);
					lvData.lvEventCallback(lvEvent);
					break;
				}
				case GLFW_RELEASE:
				{
					classMouseReleasedEvent lvEvent(givenKey);
					lvData.lvEventCallback(lvEvent);
					break;
				}
			}
		});

		glfwSetScrollCallback(lvWin, [](GLFWwindow* givenWin, double givenX, double givenY) {
			classWinData& lvData = *static_cast<classWinData*>(glfwGetWindowUserPointer(givenWin));
			classMouseScrollEvent lvEvent(static_cast<float>(givenX), static_cast<float>(givenY));
			lvData.lvEventCallback(lvEvent);
		});

		glfwSetCursorPosCallback(lvWin, [](GLFWwindow* givenWin, double givenX, double givenY) {
			classWinData& lvData = *static_cast<classWinData*>(glfwGetWindowUserPointer(givenWin));
			classMouseMoveEvent lvEvent(static_cast<float>(givenX), static_cast<float>(givenY));
			lvData.lvEventCallback(lvEvent);
		});


		

	}

	void AppWindow::funcWinShutdown() {
		glfwDestroyWindow(lvWin);
		gvGLFWInit = false;
		glfwTerminate();
	}

	void AppWindow::funcUpdate() {
		glfwPollEvents();

	}

	void AppWindow::funcSetVSync(bool enable) {
		if (enable) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}
		lvWinData.lvVsyncEnabled = enable;
	}

	bool AppWindow::funcGetVsync() const {
		return lvWinData.lvVsyncEnabled;
	}


	int AppWindow::Draw(ImVec4 ClearColor) {

        glClear(GL_COLOR_BUFFER_BIT);
        int display_w, display_h;
        glfwGetFramebufferSize(lvWin, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(ClearColor.x * ClearColor.w, ClearColor.y * ClearColor.w, ClearColor.z * ClearColor.w, ClearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);


        return 0;
	}

	//int AppWindow::CleanUp() {

 //       glfwDestroyWindow(lvWin);
 //       return 0;
	//}


}