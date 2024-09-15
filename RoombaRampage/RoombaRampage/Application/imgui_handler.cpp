#include "../Application/imgui_handler.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "../ECS/ECS.h"

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
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiHandler::DrawDemoWindow(bool& show_demo_window)
{
    if (show_demo_window)
    {
        ImGui::ShowDemoWindow(&show_demo_window);
    }
}

void ImGuiHandler::DrawAnotherWindow(bool& show_another_window)
{
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);  // Create a new ImGui window
        ImGui::Text("Hello from another window!");             // Add text inside the window
        if (ImGui::Button("Close Me"))                         // Add a button to close the window
            show_another_window = false;                       // Close the window when the button is clicked
        ImGui::End();
    }
}

void ImGuiHandler::Shutdown()
{
    // Shutdown ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}