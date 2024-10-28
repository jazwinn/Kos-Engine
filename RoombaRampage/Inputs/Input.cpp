/******************************************************************/
/*!
\file      Input.cpp
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      16 Sept, 2024
\brief     defines the class functions that contains all the callback functions that will be given to GLFW along with variables to hold the return values

The header provides declarations for the Performance class functions that
handle performance tracking and output for various engine systems such as
movement, rendering, and collision.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
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
	bool InputSystem::KeyStateE = false;
	bool InputSystem::KeyStateR = false;
	bool InputSystem::KeyStateSpace = false;
	bool InputSystem::KeyStateEsc = false;
	bool InputSystem::KeyStateF11 = false;

	vector2::Vec2 InputSystem::MousePosition = { 0,0 };
	std::string InputSystem::m_mouseString;
	std::string InputSystem::m_keyString;

	std::vector<std::string> InputSystem::m_droppedFiles;


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
			case GLFW_KEY_F11:
				KeyStateF11 = true;
				break;
			case GLFW_KEY_E:
				KeyStateE = true;
				break;
			case GLFW_KEY_ESCAPE:
				KeyStateEsc = true;
				break;
			case GLFW_KEY_R:
				KeyStateR = true;
				break;
			case GLFW_KEY_SPACE:
				KeyStateSpace = true;
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
			case GLFW_KEY_F11:
				KeyStateF11 = false;
				break;
			case GLFW_KEY_E:
				KeyStateE = false;
				break;
			case GLFW_KEY_ESCAPE:
				KeyStateEsc = false;
				break;
			case GLFW_KEY_R:
				KeyStateR = false;
				break;
			case GLFW_KEY_SPACE:
				KeyStateSpace = false;
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
	

	void InputSystem::dropCallback(GLFWwindow* window, int count, const char** paths) {
		m_droppedFiles.clear();
		for (int i = 0; i < count; ++i) {
			m_droppedFiles.emplace_back(paths[i]);
		}
	}

	void InputSystem::SetCallBack(GLFWwindow* Window) {
		glfwSetKeyCallback(Window, KeyCallBack);
		glfwSetDropCallback(Window, dropCallback);
		glfwSetMouseButtonCallback(Window, MouseButtonCallBack);
		glfwSetCursorPosCallback(Window, mousepos_cb);
	}



}