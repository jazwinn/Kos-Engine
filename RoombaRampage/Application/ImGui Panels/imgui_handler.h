#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include<vector>
#include<string>
#include <iostream>
#include "../ECS/ECS.h"
#include "../Assets/Audio.h"

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

    void DrawMainMenuBar();
    void DrawPerformanceWindow(float fps);
    void DrawComponentWindow();
    void DrawTestWindow();
    void DrawInputWindow();
    unsigned int DrawHierachyWindow();
    void DrawLogsWindow();
    void DrawRenderScreenWindow(unsigned int windowWidth, unsigned int windowHeight);
    int clicked_entity_id;

private:

    bool objectNameBox = false;
    char charBuffer[50] = "";  // Char Buffer
    std::vector<std::string> obj_text_entries;
    std::vector<bool> deleteButton;
    std::vector<bool> DuplicateButton;
    std::vector<unsigned int> obj_entity_id;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};