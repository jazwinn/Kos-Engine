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


namespace Input {
	/*--------------------------------------------------------------
	  GLOBAL VARAIBLE
	--------------------------------------------------------------*/

	vector2::Vec2 InputSystem::MousePosition = { 0,0 };
	std::string InputSystem::m_mouseString;
	std::string InputSystem::m_keyString;
	GLFWwindow* Input::InputSystem::m_windowInput;
	std::vector<std::string> InputSystem::m_droppedFiles;


	void InputSystem::KeyCallBack([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
		if (action == GLFW_PRESS) {
			InputSystem::m_keyString = "key pressed!";
		}
		else if (action == GLFW_RELEASE) {
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
	

	void InputSystem::dropCallback([[maybe_unused]] GLFWwindow* window, int count, const char** paths) {
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

	void InputSystem::m_inputUpdate() {
		for (auto& currKey : m_wasTriggered) {
			int state;
			if (currKey.first == keys::LMB || currKey.first == keys::RMB || currKey.first == keys::MMB) {
				state = glfwGetMouseButton(m_windowInput, currKey.first);
			}
			else {
				state = glfwGetKey(m_windowInput, currKey.first);
			}

			if (!m_wasPressed[currKey.first] && state == GLFW_PRESS) {
				m_wasPressed[currKey.first] = true;
				m_wasTriggered[currKey.first] = true;
			}
			else if (m_wasPressed[currKey.first]) {
				m_wasTriggered[currKey.first] = false;
			}
			if (state == GLFW_RELEASE) {
				m_wasPressed[currKey.first] = false;
				m_wasTriggered[currKey.first] = false;
			}
		}
	}

	bool InputSystem::m_isKeyTriggered(const keyCode givenKey) {
		return m_wasTriggered[givenKey];
	}
	bool InputSystem::m_isKeyPressed(const keyCode givenKey) {
		int state;
		if (givenKey == keys::LMB || givenKey == keys::RMB || givenKey == keys::MMB) {
			state = glfwGetMouseButton(m_windowInput, givenKey);
		}
		else {
			state = glfwGetKey(m_windowInput, givenKey);
		}
		if (m_wasTriggered[givenKey] && state == GLFW_PRESS) {
			m_wasTriggered[givenKey] = false;
			m_wasPressed[givenKey] = true;
		}
		return m_wasPressed[givenKey];
	}
	bool InputSystem::m_isKeyReleased(const keyCode givenKey) {
		int state;
		if (givenKey == keys::LMB || givenKey == keys::RMB || givenKey == keys::MMB) {
			state = glfwGetMouseButton(m_windowInput, givenKey);
		}
		else {
			state = glfwGetKey(m_windowInput, givenKey);
		}
		return state == 0 ? true : false;
	}



}