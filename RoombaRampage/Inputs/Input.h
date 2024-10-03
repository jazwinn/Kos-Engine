#ifndef INPUT_H
#define INPUT_H

#include <glfw3.h>
#include <../Math/Vector2.h>
#include <string>

namespace Input {

	class InputSystem {

	public:

		static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);

		static void MouseButtonCallBack(GLFWwindow* pwin, int button, int action, int mod);

		static void mousepos_cb(GLFWwindow* pwin, double xpos, double ypos);

		void SetCallBack(GLFWwindow* window);


		static bool KeyStateW;
		static bool KeyStateA;
		static bool KeyStateS;
		static bool KeyStateD;

		static vector2::Vec2 MousePosition;

		static std::string m_mouseString;
		static std::string m_keyString;
	};

}




#endif INPUT_H
