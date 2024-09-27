#include "../Application/ImGui Panels/imgui_handler.h"
#include "../Application/Helper.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "../ECS/ECS.h"
#include "../Dependencies/rapidjson/document.h"
#include "../Dependencies/rapidjson/writer.h"
#include "../Dependencies/rapidjson/stringbuffer.h"

ImGuiHandler::ImGuiHandler(){} //CTORdoing 

ImGuiHandler::~ImGuiHandler(){} //Destructor

void ImGuiHandler::Initialize(GLFWwindow* window, const char* glsl_version)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;// Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;// Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void ImGuiHandler::NewFrame()
{
    // Start a new ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiHandler::Render()
{
    // Render ImGui
    NewFrame();

    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
    DrawHierachyWindow();
    DrawComponentWindow();
    DrawLogsWindow();
    DrawRenderScreenWindow(Helper::Helpers::GetInstance()->WindowWidth, Helper::Helpers::GetInstance()->WindowHeight);
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


}


void ImGuiHandler::Shutdown()
{
    // Shutdown ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}