#include "Application/Application.h"
#include "Application/baseInput.h"

namespace Input {
	classInput* classInput::gvInputInstance = new classInput();
	Vec2 classInput::funcGetMousePos() {
		double lvX{}, lvY{};
		auto tempWin = static_cast<GLFWwindow*>(Application::Application::funcGetApp().funcGetWin().funcGetNatWin());
		glfwGetCursorPos(tempWin, &lvX, &lvY);
		return Vec2{ static_cast<float>(lvX), static_cast<float>(lvY) };
	}
	float classInput::funcGetMouseX() {
		return funcGetMousePos().x;
	}

	float classInput::funcGetMouseY() {
		return funcGetMousePos().y;
	}

	bool classInput::funcIsKeyPress(int givenKeyCode) {
		auto tempWin = static_cast<GLFWwindow*>(Application::Application::funcGetApp().funcGetWin().funcGetNatWin());
		auto lvPress = glfwGetKey(tempWin, givenKeyCode);
		
		return lvPress == GLFW_PRESS || lvPress == GLFW_REPEAT;
	}

	bool classInput::funcIsKeyTriggered(int givenKeyCode) {
		//not yet implemented
		return false;
	}

	bool classInput::funcIsMouseButtonPress(int givenButton) {
		auto tempWin = static_cast<GLFWwindow*>(Application::Application::funcGetApp().funcGetWin().funcGetNatWin());
		auto lvMousePress = glfwGetKey(tempWin, givenButton);
		return lvMousePress == GLFW_PRESS || lvMousePress == GLFW_REPEAT;
	}

	bool classInput::funcIsMouseButtonTriggered(int givenButton) {
		//not yet implemented
		return false;
	}
}