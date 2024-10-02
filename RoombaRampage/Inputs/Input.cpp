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
	std::string InputSystem::m_mouseString;
	std::string InputSystem::m_keyString;

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
			InputSystem::m_keyString = "key pressed!";
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
			InputSystem::m_keyString = "-";
		}
		else if (action == GLFW_REPEAT) {
			InputSystem::m_keyString = "Key Repeated";
		}
	}

	void InputSystem::MouseButtonCallBack([[maybe_unused]] GLFWwindow* pwin, [[maybe_unused]] int button, [[maybe_unused]] int action, [[maybe_unused]] int mod) {

		if (action == GLFW_PRESS) {
			// key is press
			InputSystem::m_mouseString = "Mouse pressed!";
		}
		else if (action == GLFW_RELEASE) {
			InputSystem::m_mouseString = "-";
		}
		else if (action == GLFW_REPEAT) {
			InputSystem::m_mouseString = "Mouse Repeated";
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