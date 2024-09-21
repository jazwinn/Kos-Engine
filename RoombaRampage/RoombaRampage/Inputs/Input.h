#ifndef INPUT_H
#define INPUT_H

#include <glfw3.h>
#include <../Math/vector2.h>

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

		static Vector2::Vec2 MousePosition;
	};

}




#endif INPUT_H
