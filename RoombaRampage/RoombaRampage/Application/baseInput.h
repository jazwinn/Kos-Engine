#pragma once
#include <glfw3.h>
#include "../Math/vector2.h"


namespace Application {
	class classInput {
	public:
		static bool funcIsKeyPress(int givenKeyCode);
		static bool funcIsKeyTriggered(int giveKeyCode);
		static bool funcIsMouseButtonPress(int givenButton);
		static bool funcIsMouseButtonTriggered(int givenButton);
		static Vec2 funcGetMousePos();
		static float funcGetMouseX();
		static float funcGetMouseY();
		void funcInputUpdate();
		static void funcInputFree();
		~classInput() = default;

	private:
		static classInput* gvInputInstance;

	};
}