#pragma once
#include <glew.h>
#include <glfw3.h>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"


namespace Application {

	class AppWindow{

	public:

		int init();

		int Draw(ImVec4 ClearColor);

		int CleanUp();

		GLFWwindow* Window;
		GLFWmonitor* monitor;
		const GLFWvidmode* mode;

	};





}