#pragma once
#include <glfw3.h>
#include "window.h"

class classWindowsWin : public classWindow {
public:
	classWindowsWin(const classWinProperties& givenProps);
	virtual ~classWindowsWin();
	void funcUpdate() override;
	unsigned int funcGetWinWidth() const override { return lvWinData.lvWidth; }
	unsigned int funcGetWinHeight() const override { return lvWinData.lvHeight; }
	void funcSetEventCallback(const eventCallbackFunc& givenCallback) override {lvWinData.lvEventCallback = givenCallback; }
	void funcSetVSync(bool enable) override;
	bool funcGetVsync() const override;
	virtual void* funcGetNatWin() const { return lvWin; }
private:
	virtual void funcWinInit(const classWinProperties& givenProps);
	virtual void funcWinShutdown();
	GLFWwindow* lvWin;
	class classWinData {
	public:
		std::string lvTitle;
		unsigned int lvWidth = 0, lvHeight = 0;
		bool lvVsyncEnabled = false;
		eventCallbackFunc lvEventCallback;
	};

	classWinData lvWinData;
};