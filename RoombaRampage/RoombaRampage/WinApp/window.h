#pragma once
#include "../Events/baseEvent.h"
#include <functional>
#include <string>

class classWinProperties {
public:
	std::string lvWinTitle;
	unsigned int lvWinWidth, lvWinHeight;

	classWinProperties(const std::string& givenTitle = "Roomba Rampage", unsigned int givenWidth = 1280, unsigned int givenHeight = 720) :
		lvWinTitle(givenTitle), lvWinWidth(givenWidth), lvWinHeight(givenHeight) {}
};

class classWindow {
public:
	using eventCallbackFunc = std::function<void(classEvent&)>;

	virtual ~classWindow() {}
	virtual void funcUpdate() = 0;
	virtual unsigned int funcGetWinWidth() const = 0;
	virtual unsigned int funcGetWinHeight() const = 0;
	virtual void funcSetEventCallback(const eventCallbackFunc& givenCallback) = 0;
	virtual void funcSetVSync(bool enable) = 0;
	virtual bool funcGetVsync() const = 0;
	virtual void* funcGetNatWin() const = 0;

	static classWindow* funcCreateWindow(const classWinProperties& givenProperties = classWinProperties());
};