#pragma once
#include <glfw3.h>
#include "../Math/vector2.h"
#include "../Input/keycodes.h"
#include "../Input/mousecodes.h"


namespace Input {
	class classInput {
	public:
		static bool funcIsKeyPress(int givenKeyCode);
		static bool funcIsMouseButtonPress(int givenButton);
		static Vector2::Vec2 funcGetMousePos();
		static float funcGetMouseX();
		static float funcGetMouseY();
		~classInput() = default; 

	};
}