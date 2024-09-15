#pragma once
#include <glew.h>

#include <glfw3.h>
#include <functional>
#include <string>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"
#include "../Events/baseEvent.h"
#include "../Events/keyEvents.h"
#include "../Events/mouseEvents.h"
#include "../Events/windowEvents.h"



namespace Application {

	class winProperties {
	public:
		std::string lvWinTitle;
		unsigned int lvWinWidth, lvWinHeight;

		winProperties(const std::string& givenTitle = "Roomba Rampage", unsigned int givenWidth = 1280, unsigned int givenHeight = 720) :
			lvWinTitle(givenTitle), lvWinWidth(givenWidth), lvWinHeight(givenHeight) {}
	};

	class AppWindow{

	public:
		using eventCallbackFunc = std::function<void(classEvent&)>;


		AppWindow(const winProperties& givenProps);
		virtual ~AppWindow();
		void funcUpdate();
		unsigned int funcGetWinWidth() const{ return lvWinData.lvWidth; }
		unsigned int funcGetWinHeight() const { return lvWinData.lvHeight; }
		void funcSetEventCallback(const eventCallbackFunc& givenCallback) { lvWinData.lvEventCallback = givenCallback; }
		void funcSetVSync(bool enable) ;
		bool funcGetVsync() const;

		virtual void* funcGetNatWin() const { return lvWin; }



		static AppWindow* funcCreateWindow(const winProperties& givenProperties = winProperties());

		//int init();

		int Draw(ImVec4 ClearColor);

		//int CleanUp();

		//GLFWwindow* Window;
		//GLFWmonitor* monitor;
		//const GLFWvidmode* mode;
	private:
		void funcWinInit(const winProperties& givenProps);
		void funcWinShutdown();
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





}