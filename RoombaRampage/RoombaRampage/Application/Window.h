#pragma once
#include <glew.h>
#include <glfw3.h>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGui Panels/imgui_handler.h"


namespace Application {

	class AppWindow{

	public:

		int init();

		int Draw();

		int CleanUp();

		GLFWwindow* Window;
		GLFWmonitor* monitor;
		const GLFWvidmode* mode;

		float WindowHeight;
		float WindowWidth;

		
	};





}