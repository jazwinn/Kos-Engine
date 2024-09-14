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

}