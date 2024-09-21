#pragma once

#include "Window.h"
#include "imgui_handler.h"

namespace Application{

	class Application {

	public:

		static int Init();

		static int Run();

		static int Cleanup();
	
		static AppWindow lvWindow;

		static ImGuiHandler imgui_manager;


	};



}