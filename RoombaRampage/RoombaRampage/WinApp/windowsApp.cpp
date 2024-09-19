#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../Application/imgui_handler.h"
#include <glfw3.h>
#include "windowsApp.h"


classWindowsApp* classWindowsApp::lvInstance = nullptr;

classWindowsApp::classWindowsApp() {
	std::string lvTitle;//, lvWidthS, lvHeightS;
	int lvWidth, lvHeight;
	lvTitle = "Roomba Rampage";
	lvWidth = 1280;
	lvHeight = 720;

	classWinProperties lvProps{};
	lvProps.lvWinTitle = lvTitle;
	lvProps.lvWinWidth = lvWidth;
	lvProps.lvWinHeight = lvHeight;

	lvInstance = this;
	lvWin = std::unique_ptr<classWindow>(classWindow::funcCreateWindow(lvProps));
	lvWin->funcSetEventCallback(BIND_EVENT_FN(funcEvent));
	GLFWmonitor* gvMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(gvMonitor);

	lvWinSize.x = static_cast<float>(classWindowsApp::funcGetApp().funcGetWin().funcGetWinWidth());
	lvWinSize.y = static_cast<float>(classWindowsApp::funcGetApp().funcGetWin().funcGetWinHeight());

	lvWinWidth = mode->width;
	lvWinHeight = mode->height;

	glfwGetWindowPos(static_cast<GLFWwindow*>(classWindowsApp::funcGetApp().funcGetWin().funcGetNatWin()), &lvWinPosX, &lvWinPosY);
	
}

classWindowsApp::~classWindowsApp() {
	//ecs stuff
}

void classWindowsApp::funcInit() {
	//init audio manager asset manager and renderer;
}

void classWindowsApp::funcEnd() {
	//audio and renderer cleanup
}

void classWindowsApp::funcRun() {
	//ecs stuff
}

void classWindowsApp::funcEvent(classEvent& givenEvent) {
	/*classEventDispatch lvDispatcher(givenEvent);
	lvDispatcher.funcDispatch<classWindowClosedEvent>(BIND_EVENT_FN(funcOnWinClose));
	lvDispatcher.funcDispatch<classWindowResizeEvent>(BIND_EVENT_FN(funcOnWinResize));
	lvDispatcher.funcDispatch<classMouseMoveEvent>(BIND_EVENT_FN(funcOnMouseMove));*/
}