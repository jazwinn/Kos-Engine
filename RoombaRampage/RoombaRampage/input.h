#pragma once
#include <glew.h> // for access to OpenGL API declarations 
#include <glfw3.h>
#include <string>
#include <glm.hpp>

namespace input {
	class classInputHelper {
	public:
		static int gvWidth, gvHeight;
		static double gvFps;
		static std::string gvTitle;
	};
}