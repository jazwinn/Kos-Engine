#pragma once
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../Application/imgui_handler.h"
#include "../WinApp/window.h"
#include "../Events/baseEvent.h"
#include "../Events/windowEvents.h"
#include "../Events/keyEvents.h"
#include "../Events/mouseEvents.h"
#include "../Math/vector2.h"

#define BIND_EVENT_FN(x) std::bind(&classWindowsApp::x, this, std::placeholders::_1)

class classWindowsApp {
public:
	classWindowsApp();

	virtual ~classWindowsApp();

	void funcInit();
	void funcEnd();
	void funcRun();
	void funcEvent(classEvent& givenEvent);
	classWindow& funcGetWin() { return *lvWin; }
	static classWindowsApp& funcGetApp() { return *lvInstance; }
	void funcStopApp() { lvIsRunning = false; }

	float gvDt{};
	const float gvFixedDt = 1.f / 60.f;
	

private:
	//void funcOnUpdate();
	//bool funcOnWinClose(classWindowClosedEvent& givenEvent);
	//bool funcOnWinResize(classWindowResizeEvent& givenEvent);
	//bool funcOnMouseMove(classMouseMoveEvent& givenEvent);
	std::unique_ptr<classWindow> lvWin{};
	bool lvIsRunning;
	static classWindowsApp* lvInstance;
	Vec2 lvWinSize{};
	int lvWinWidth{}, lvWinHeight{};
	int lvWinPosX{}, lvWinPosY{};
	bool lvIsFullscrean = true;

};