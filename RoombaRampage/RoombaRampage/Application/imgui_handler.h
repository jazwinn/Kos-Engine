#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include<vector>
#include<string>
#include <iostream>
#include "../ECS/ECS.h"

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

    void DrawComponentWindow(Ecs::ECS* ecs, Ecs::EntityID entityID, bool& windowOpen, const std::string& windowTitle);
    unsigned int DrawHierachyWindow(ImVec4& clear_color);
    unsigned int clicked_entity_id;

private:
    bool show_demo_window = true;
    bool show_another_window = false;

    bool objectNameBox = false;
    char charBuffer[50] = "";  // Char Buffer
    std::vector<std::string> obj_text_entries;
    std::vector<bool> deleteButton;
    std::vector<bool> obj_component_window;
    std::vector<unsigned int> obj_entity_id;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};