#pragma once

#include "Window.h"
#include "../Assets/Audio.h"

namespace Application{



	class Application {

	public:


		static int Init();

		static int Run();

		static int Cleanup();
	
		static AppWindow lvWindow;

	private:
		static ImGuiHandler imgui_manager; //should only remain in Application


	};
}