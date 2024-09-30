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

	vector2::Vec2 InputSystem::MousePosition = { 0,0 };

	void InputSystem::KeyCallBack([[maybe_unused]] GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
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

	void InputSystem::MouseButtonCallBack([[maybe_unused]] GLFWwindow* pwin, [[maybe_unused]] int button, [[maybe_unused]] int action, [[maybe_unused]] int mod) {

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
		ypos = static_cast<double>(height - ypos);

		MousePosition.m_x = static_cast<float>(xpos);
		MousePosition.m_y = static_cast<float>(ypos);
		//std::cout << xpos << " : " << ypos << std::endl;

	}

	void InputSystem::SetCallBack(GLFWwindow* Window) {
		glfwSetKeyCallback(Window, KeyCallBack);
		glfwSetMouseButtonCallback(Window, MouseButtonCallBack);
		glfwSetCursorPosCallback(Window, mousepos_cb);
	}



}