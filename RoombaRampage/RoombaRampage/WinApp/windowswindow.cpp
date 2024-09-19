#include "glew.h"
#include "windowswindow.h"
#include "../Events/keyEvents.h"
#include "../Events/mouseEvents.h"
#include "../Events/windowEvents.h"
#include <iostream>

static bool gvGLFWInit = false;

static void funcErrorCallback(int givenErrorCode, const char* reason) {
	std::cerr << "GLFW error: " << reason << std::endl;
}

classWindow* classWindow::funcCreateWindow(const classWinProperties& givenProperties) {
	return new classWindowsWin(givenProperties);
}

classWindowsWin::classWindowsWin(const classWinProperties& givenProperties) {
	classWindowsWin::funcWinInit(givenProperties);
}

classWindowsWin::~classWindowsWin() {
	classWindowsWin::funcWinShutdown();
}


void classWindowsWin::funcWinInit(const classWinProperties& givenProperties) {
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


	

}

void classWindowsWin::funcWinShutdown() {
	glfwDestroyWindow(lvWin);
	gvGLFWInit = false;
	glfwTerminate();
}

void classWindowsWin::funcUpdate() {
	glfwPollEvents();

}

void classWindowsWin::funcSetVSync(bool enable) {
	if (enable) {
		glfwSwapInterval(1);
	}
	else {
		glfwSwapInterval(0);
	}
	lvWinData.lvVsyncEnabled = enable;
}

bool classWindowsWin::funcGetVsync() const {
	return lvWinData.lvVsyncEnabled;
}