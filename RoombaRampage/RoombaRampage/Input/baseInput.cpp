#include "../Application/Application.h"
#include "baseInput.h"

namespace Input {


	Vector2::Vec2 classInput::funcGetMousePos() {
		double lvX{}, lvY{};
		auto tempWin = static_cast<GLFWwindow*>(Application::Application::funcGetApp().funcGetWin().funcGetNatWin());
		glfwGetCursorPos(tempWin, &lvX, &lvY);
		return Vector2::Vec2{ static_cast<float>(lvX), static_cast<float>(lvY) };
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

	bool classInput::funcIsMouseButtonPress(int givenButton) {
		auto tempWin = static_cast<GLFWwindow*>(Application::Application::funcGetApp().funcGetWin().funcGetNatWin());
		auto lvMousePress = glfwGetMouseButton(tempWin, givenButton);
		return lvMousePress == GLFW_PRESS || lvMousePress == GLFW_REPEAT;
	}

}