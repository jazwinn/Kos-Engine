#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include<vector>
#include<string>
#include <iostream>

class ImGuiHandler
{
public:
    //CTOR & Destructor
    ImGuiHandler();
    ~ImGuiHandler();

    void Initialize(GLFWwindow* window, const char* glsl_version);
    void NewFrame();
    void Render();
    void Shutdown();

    void DrawDemoWindow(bool& show_demo_window);
    void DrawHierachyWindow(bool& show_demo_window, bool& show_another_window, ImVec4& clear_color);
    void DrawAnotherWindow(bool& show_another_window);

private:
    bool show_demo_window = true;
    bool show_another_window = false;

    bool stringBox = false;
    char charBuffer[50] = "";  // Char Buffer
    std::vector<std::string> obj_text_entries;
    std::vector<bool> deleteButton;
    std::vector<bool> obj_component_window;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};