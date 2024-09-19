#include "Window.h"

namespace Application {


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
		lvGLFWWin = glfwCreateWindow(givenProperties.lvWinWidth, givenProperties.lvWinHeight, givenProperties.lvWinTitle.c_str(), nullptr, nullptr);
		if (!lvGLFWWin) {
			std::cerr << "GLFW unable to create openGL context\n";
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(lvGLFWWin);
		GLenum lvError = glewInit();
		if (lvError != GLEW_OK) {
			std::cerr << "GLEW unable to initalize due to " << glewGetErrorString(lvError) << std::endl;
			return;
		}
		glfwSetWindowUserPointer(lvGLFWWin, &lvWinData);

		glfwSetWindowSizeCallback(lvGLFWWin, [](GLFWwindow* givenWin, int givenW, int givenH) {
			classWinData& lvData = *static_cast<classWinData*>(glfwGetWindowUserPointer(givenWin));
			lvData.lvWidth = givenW;
			lvData.lvHeight = givenH;
			classWindowResizeEvent lvEvent(givenW, givenH);
			lvData.lvEventCallback(lvEvent);
		});

		glfwSetWindowCloseCallback(lvGLFWWin, [](GLFWwindow* givenWin) {
			classWinData& lvData = *static_cast<classWinData*>(glfwGetWindowUserPointer(givenWin));
			classWindowClosedEvent lvEvent;
			lvData.lvEventCallback(lvEvent);
		});

		glfwSetKeyCallback(lvGLFWWin, [](GLFWwindow* givenWin, int givenKey, [[maybe_unused]] int scanCode, int givenAction, [[maybe_unused]] int mods) {
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

		glfwSetMouseButtonCallback(lvGLFWWin, [](GLFWwindow* givenWin, int givenKey,  int givenAction, [[maybe_unused]] int mods) {
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

		glfwSetScrollCallback(lvGLFWWin, [](GLFWwindow* givenWin, double givenX, double givenY) {
			classWinData& lvData = *static_cast<classWinData*>(glfwGetWindowUserPointer(givenWin));
			classMouseScrollEvent lvEvent(static_cast<float>(givenX), static_cast<float>(givenY));
			lvData.lvEventCallback(lvEvent);
		});

		glfwSetCursorPosCallback(lvGLFWWin, [](GLFWwindow* givenWin, double givenX, double givenY) {
			classWinData& lvData = *static_cast<classWinData*>(glfwGetWindowUserPointer(givenWin));
			classMouseMoveEvent lvEvent(static_cast<float>(givenX), static_cast<float>(givenY));
			lvData.lvEventCallback(lvEvent);
		});


		

	}

	void AppWindow::funcWinShutdown() {
		glfwDestroyWindow(lvGLFWWin);
		gvGLFWInit = false;
		glfwTerminate();
	}

	void AppWindow::funcUpdate() {
		glfwPollEvents();

	}

	int AppWindow::Draw(ImVec4 ClearColor) {

        glClear(GL_COLOR_BUFFER_BIT);
        int display_w, display_h;
        glfwGetFramebufferSize(lvGLFWWin, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(ClearColor.x * ClearColor.w, ClearColor.y * ClearColor.w, ClearColor.z * ClearColor.w, ClearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);

        return 0;
	}



}