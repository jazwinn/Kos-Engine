#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"

#include "Window.h"
#include "../Events/baseEvent.h"
#include "../Events/windowEvents.h"
#include "../Events/keyEvents.h"
#include "../Events/mouseEvents.h"
#include "../Math/vector2.h"
#include "../Application/baseInput.h"
#include "../Graphics/GraphicsPipe.h"
#include "../ECS/ECS.h"



#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace Application{

	class Application {

	public:
		~Application() = default;

		void funcEvent(classEvent& givenEvent);
		inline AppWindow& funcGetWin() { return *lvWin; }
		static Application& funcGetApp();

		float gvDt{};
		const float gvFixedDt = 1.f / 60.f;

		static int Init();

		static int Run();

		static int Cleanup();


		Vector2::Vec2 lvMousePos{};

		

	private:
		bool funcOnMouseMove(classMouseMoveEvent& givenEvent);
		static std::unique_ptr<AppWindow> lvWin;
		static Application* lvInstance;


	};



}