#include "Input.h"
#include <iostream>

namespace Input {
	/*--------------------------------------------------------------
	  GLOBAL VARAIBLE
	--------------------------------------------------------------*/
	bool InputSystem::KeyStateW = false;
	bool InputSystem::KeyStateA = false;
	bool InputSystem::KeyStateS = false;
	bool InputSystem::KeyStateD = false;

	Vector2::Vec2 InputSystem::MousePosition = { 0,0 };

	void InputSystem::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (action == GLFW_PRESS) {
			switch (key){
			case GLFW_KEY_W:
				KeyStateW = true;
				break;
			case GLFW_KEY_A:
				KeyStateA = true;
				break;
			case GLFW_KEY_S:
				KeyStateS = true;
				break;
			case GLFW_KEY_D:
				KeyStateD = true;
				break;
			default:
				break;
			}
			std::cout << "key pressed!" << std::endl;
		}
		else if (action == GLFW_RELEASE) {
			switch (key) {
			case GLFW_KEY_W:
				KeyStateW = false;
				break;
			case GLFW_KEY_A:
				KeyStateA = false;
				break;
			case GLFW_KEY_S:
				KeyStateS = false;
				break;
			case GLFW_KEY_D:
				KeyStateD = false;
				break;
			default:
				break;
			}
			std::cout << "key release" << std::endl;
		}
		else if (action == GLFW_REPEAT) {
			std::cout << "Key Repeated" << std::endl;
		}
	}

	void InputSystem::MouseButtonCallBack(GLFWwindow* pwin, int button, int action, int mod) {
		if (action == GLFW_PRESS) {
			// key is press
			std::cout << "Mouse pressed!" << std::endl;
		}
		else if (action == GLFW_RELEASE) {
			std::cout << "Mouse release" << std::endl;
		}
		else if (action == GLFW_REPEAT) {
			std::cout << "Mouse Repeated" << std::endl;
		}
		
	}

	void InputSystem::mousepos_cb(GLFWwindow* pwin, double xpos, double ypos) {
		int width{}, height{};
		
		glfwGetWindowSize(pwin, &width, &height);
		//change origin from top left to bottom left
		ypos = height - ypos;

		MousePosition.x = xpos;
		MousePosition.y = ypos;
		//std::cout << xpos << " : " << ypos << std::endl;

	}

	void InputSystem::SetCallBack(GLFWwindow* Window) {
		glfwSetKeyCallback(Window, KeyCallBack);
		glfwSetMouseButtonCallback(Window, MouseButtonCallBack);
		glfwSetCursorPosCallback(Window, mousepos_cb);
	}



}