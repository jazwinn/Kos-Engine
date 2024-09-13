#include "Input.h"
#include <iostream>

namespace Input {
	bool classInput::funcWindowInit(int width, int height, std::string title) {
		classInput::lvWidth = width;
		classInput::lvHeight = height;
		classInput::lvWindowTitle = title;

		if (!glfwInit()) {
			std::cout << "GLFW failed to initialize." << std::endl;
			return false;
		}

		glfwSetErrorCallback(classInput::funcErrorCallback);
		classInput::funcSetupGLFW();
		classInput::lvWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		if (!lvWindow) {
			std::cerr << "GLFW unable to create window." << std::endl;
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(lvWindow);
		classInput::funcAddEventCallback();
		glfwSetInputMode(lvWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		return true;
	}

	bool classInput::funcSetupGLFW() {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
		glfwWindowHint(GLFW_DEPTH_BITS, 24);
		glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
		glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	}

	void classInput::funcAddEventCallback() {
		glfwSetFramebufferSizeCallback(classInput::lvWindow, classInput::funcFrameBuffCallback);
		glfwSetKeyCallback(classInput::lvWindow, classInput::funcKeyCallback);
		glfwSetMouseButtonCallback(classInput::lvWindow, classInput::funcMouseCallback);
		glfwSetCursorPosCallback(classInput::lvWindow, classInput::funcMouseMoveCallback);
		glfwSetScrollCallback(classInput::lvWindow, classInput::funcMouseScrollCallback);
	}

	void classInput::funcCalcdt(double interval = 1.0) {
		static double lvPrevTime = glfwGetTime();
		double lvCurrTime = glfwGetTime();
		classInput::lvDt = lvCurrTime - lvPrevTime;
		lvPrevTime = lvCurrTime;

		static double lvStart = glfwGetTime();
		static double lvIters = 0.0;
		lvIters++;
		double lvElapsed = lvCurrTime - lvStart;

		if (interval < 0.0) {
			interval = 0.0;
		}
		if (interval > 5.0) {
			interval = 5.0;
		}
		if (lvElapsed > interval) {
			classInput::lvFps = lvIters / lvElapsed;
			lvStart = lvCurrTime;
			lvIters = 0.0;
		}
	}


	void classInput::funcKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			switch (key) {
			case GLFW_KEY_W:
				classInput::lvWButton = true;
				break;
			case GLFW_KEY_A:
				classInput::lvAButton = true;
				break;
			case GLFW_KEY_S:
				classInput::lvSButton = true;
				break;
			case GLFW_KEY_D:
				classInput::lvSButton = true;
				break;
			default:
				break;
			}
		}

		if (action == GLFW_RELEASE) {
			switch (key) {
			case GLFW_KEY_W:
				classInput::lvWButton = false;
				break;
			case GLFW_KEY_A:
				classInput::lvAButton = false;
				break;
			case GLFW_KEY_S:
				classInput::lvSButton = false;
				break;
			case GLFW_KEY_D:
				classInput::lvSButton = false;
				break;
			default:
				break;
			}
		}
	}

	void classInput::funcMouseMoveCallback(GLFWwindow* window, double posX, double posY) {
		classInput::lvMousePos.x = posX / classInput::lvWidth;
		classInput::lvMousePos.y = posY / classInput::lvHeight;
	}

	void classInput::funcFrameBuffCallback(GLFWwindow* window, int width, int height) {
		classInput::lvWidth = width;
		classInput::lvHeight = height;
	}

	void classInput::funcMouseCallback(GLFWwindow* window, int key, int action, int mods) {
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			switch (key) {
			case GLFW_MOUSE_BUTTON_LEFT:
				classInput::lvLMB = true;
				break;
			case GLFW_MOUSE_BUTTON_RIGHT:
				classInput::lvRMB = true;
				break;
			default:
				break;
			}
		}

		if (action == GLFW_RELEASE) {
			switch (key) {
			case GLFW_MOUSE_BUTTON_LEFT:
				classInput::lvLMB = false;
				break;
			case GLFW_MOUSE_BUTTON_RIGHT:
				classInput::lvRMB = false;
				break;
			default:
				break;
			}
		}
	}

	void classInput::funcErrorCallback(int error, const char* reason) {
		std::cerr << "GLFW Error " << error << ": " << reason << std::endl;
	}

	void classInput::funcMouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {

	}

}