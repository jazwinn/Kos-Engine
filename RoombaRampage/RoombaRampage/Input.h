#pragma once
#include <glew.h> // for access to OpenGL API declarations 
#include <glfw3.h>
#include <string>
#include <glm.hpp>
#include "Math/vector2.h"

namespace Input {
	class classInput {
	public:
		static int lvWidth, lvHeight;
		static double lvFps, lvDt;
		static std::string lvWindowTitle;
		static GLFWwindow* lvWindow;
		static Vec2 lvMousePos;
		static bool lvLMB, lvRMB, lvWButton, lvAButton, lvSButton, lvDButton;
		


		static bool funcWindowInit(int width, int height, std::string title);
		static bool funcSetupGLFW();
		static void funcErrorCallback(int error, const char* reason);
		static void funcFrameBuffCallback(GLFWwindow* window, int width, int height); 
		static void funcKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void funcMouseCallback(GLFWwindow* window, int key, int action, int mods);
		static void funcMouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
		static void funcMouseMoveCallback(GLFWwindow* window, double posX, double posY);

		static void funcAddEventCallback();
		static void funcCalcdt(double interval = 1.0);


	};
}