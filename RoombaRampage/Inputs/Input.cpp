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
	//bool InputSystem::KeyStateW = false;
	//bool InputSystem::KeyStateA = false;
	//bool InputSystem::KeyStateS = false;
	//bool InputSystem::KeyStateD = false;
	//bool InputSystem::KeyStateE = false;
	//bool InputSystem::KeyStateR = false;
	//bool InputSystem::KeyStateSpace = false;
	//bool InputSystem::KeyStateEsc = false;
	//bool InputSystem::KeyState0 = false;
	//bool InputSystem::KeyStateLMB = false;
	//bool InputSystem::KeyStateF11 = false;
	int InputSystem::m_keyStateW = 0;
	int InputSystem::m_keyStateA = 0;
	int InputSystem::m_keyStateS = 0;
	int InputSystem::m_keyStateD = 0;
	int InputSystem::m_keyStateE = 0;
	int InputSystem::m_keyStateR = 0;
	int InputSystem::m_keyState0 = 0;
	int InputSystem::m_keyStateF11 = 0;
	int InputSystem::m_keyStateSpace = 0;
	int InputSystem::m_keyStateEsc = 0;
	int InputSystem::m_keyStateLMB = 0;

	vector2::Vec2 InputSystem::MousePosition = { 0,0 };
	std::string InputSystem::m_mouseString;
	std::string InputSystem::m_keyString;

	std::vector<std::string> InputSystem::m_droppedFiles;


	void InputSystem::KeyCallBack([[maybe_unused]] GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
		if (action == GLFW_PRESS) {
			switch (key){
			case GLFW_KEY_W:
				m_keyStateW = GLFW_PRESS;
				break;
			case GLFW_KEY_A:
				m_keyStateA = GLFW_PRESS;
				break;
			case GLFW_KEY_S:
				m_keyStateS = GLFW_PRESS;
				break;
			case GLFW_KEY_D:
				m_keyStateD = GLFW_PRESS;
				break;
			case GLFW_KEY_F11:
				m_keyStateF11 = GLFW_PRESS;
				break;
			case GLFW_KEY_E:
				m_keyStateE = GLFW_PRESS;
				break;
			case GLFW_KEY_ESCAPE:
				m_keyStateEsc = GLFW_PRESS;
				break;
			case GLFW_KEY_R:
				m_keyStateR = GLFW_PRESS;
				break;
			case GLFW_KEY_SPACE:
				m_keyStateSpace = GLFW_PRESS;
				break;
			default:
				break;
			}
			InputSystem::m_keyString = "key pressed!";
		}
		else if (action == GLFW_RELEASE) {
			switch (key) {
			case GLFW_KEY_W:
				m_keyStateW = GLFW_RELEASE;
				break;
			case GLFW_KEY_A:
				m_keyStateA = GLFW_RELEASE;
				break;
			case GLFW_KEY_S:
				m_keyStateS = GLFW_RELEASE;
				break;
			case GLFW_KEY_D:
				m_keyStateD = GLFW_RELEASE;
				break;
			case GLFW_KEY_F11:
				m_keyStateF11 = GLFW_RELEASE;
				break;
			case GLFW_KEY_E:
				m_keyStateE = GLFW_RELEASE;
				break;
			case GLFW_KEY_ESCAPE:
				m_keyStateEsc = GLFW_RELEASE;
				break;
			case GLFW_KEY_R:
				m_keyStateR = GLFW_RELEASE;
				break;
			case GLFW_KEY_SPACE:
				m_keyStateSpace = GLFW_RELEASE;
				break;
			default:
				break;
			}
			InputSystem::m_keyString = "-";
		}
		else if (action == GLFW_REPEAT) {
			InputSystem::m_keyString = "Key Repeated";
			switch (key) {
			case GLFW_KEY_W:
				m_keyStateW = GLFW_REPEAT;
				break;
			case GLFW_KEY_A:
				m_keyStateA = GLFW_REPEAT;
				break;
			case GLFW_KEY_S:
				m_keyStateS = GLFW_REPEAT;
				break;
			case GLFW_KEY_D:
				m_keyStateD = GLFW_REPEAT;
				break;
			case GLFW_KEY_F11:
				m_keyStateF11 = GLFW_REPEAT;
				break;
			case GLFW_KEY_E:
				m_keyStateE = GLFW_REPEAT;
				break;
			case GLFW_KEY_ESCAPE:
				m_keyStateEsc = GLFW_REPEAT;
				break;
			case GLFW_KEY_R:
				m_keyStateR = GLFW_REPEAT;
				break;
			case GLFW_KEY_SPACE:
				m_keyStateSpace = GLFW_REPEAT;
				break;
			default:
				break;
			}
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
		switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			switch (action) {
			case GLFW_PRESS:
				m_keyStateLMB = GLFW_PRESS;
				break;
			case GLFW_RELEASE:
				m_keyStateLMB = GLFW_RELEASE;
				break;
			case GLFW_REPEAT:
				m_keyStateLMB = GLFW_REPEAT;
				break;
			}
			break;
		default:
			break;
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

	bool InputSystem::m_isKeyTriggered(const keyCode givenKey) {
		switch (givenKey) {
		case keys::W:
			return m_keyStateW == 1? true : false;
			break;
		case keys::A:
			return m_keyStateA == 1? true : false;
			break;
		case keys::S:
			return m_keyStateS == 1? true : false;
			break;
		case keys::D:
			return m_keyStateD == 1? true : false;
			break;
		case keys::E:
			return m_keyStateE == 1 ? true : false;
			break;
		case keys::R:
			return m_keyStateR == 1 ? true : false;
			break;
		case keys::NUM0:
			return m_keyState0 == 1 ? true : false;
			break;
		case keys::LMB:
			return m_keyStateLMB == 1 ? true : false;
			break;
		case keys::F11:
			return m_keyStateF11 == 1 ? true : false;
			break;
		case keys::ESC:
			return m_keyStateEsc == 1 ? true : false;
			break;
		case keys::SPACE:
			return m_keyStateSpace == 1 ? true : false;
			break;
		default:
			return false;
			break;
		}
	}
	bool InputSystem::m_isKeyPressed(const keyCode givenKey) {
		switch (givenKey) {
		case keys::W:
			return m_keyStateW == 2 ? true : false;
			//break;
		case keys::A:
			return m_keyStateW == 2 ? true : false;
			//break;
		case keys::S:
			return m_keyStateW == 2 ? true : false;
			//break;
		case keys::D:
			return m_keyStateW == 2 ? true : false;
			//break;
		case keys::E:
			return m_keyStateE == 2 ? true : false;
			//break;
		case keys::R:
			return m_keyStateR == 2 ? true : false;
			//break;
		case keys::NUM0:
			return m_keyState0 == 2 ? true : false;
			break;
		case keys::LMB:
			return m_keyStateLMB == 2 ? true : false;
			//break;
		case keys::F11:
			return m_keyStateF11 == 2 ? true : false;
			//break;
		case keys::ESC:
			return m_keyStateEsc == 2 ? true : false;
			//break;
		case keys::SPACE:
			return m_keyStateSpace == 2 ? true : false;
			//break;
		default:
			return false;
			//break;
		}
	}
	bool InputSystem::m_isKeyReleased(const keyCode givenKey) {
		switch (givenKey) {
		case keys::W:
			return m_keyStateW == 0 ? true : false;
			break;
		case keys::A:
			return m_keyStateA == 0 ? true : false;
			break;
		case keys::S:
			return m_keyStateS == 0 ? true : false;
			break;
		case keys::D:
			return m_keyStateD == 0 ? true : false;
			break;
		case keys::E:
			return m_keyStateE == 0 ? true : false;
			break;
		case keys::R:
			return m_keyStateR == 0 ? true : false;
			break;
		case keys::NUM0:
			return m_keyState0 == 0 ? true : false;
			break;
		case keys::LMB:
			return m_keyStateLMB == 0 ? true : false;
			break;
		case keys::F11:
			return m_keyStateF11 == 0 ? true : false;
			break;
		case keys::ESC:
			return m_keyStateEsc == 0 ? true : false;
			break;
		case keys::SPACE:
			return m_keyStateSpace == 0 ? true : false;
			break;
		default:
			return false;
			break;
		}
	}



}