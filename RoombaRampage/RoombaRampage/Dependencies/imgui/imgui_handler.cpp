#include "imgui_handler.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"

ImGuiHandler::ImGuiHandler(){} //CTOR

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

void ImGuiHandler::DrawCustomWindow(bool& show_demo_window, bool& show_another_window, ImVec4& clear_color)
{
    // Custom window with example widgets
    ImGui::Begin("Hello, world!");

    ImGui::Text("This is some useful text.");
    ImGui::Checkbox("Another Window", &show_another_window);
    ImGui::ColorEdit3("clear color", (float*)&clear_color);

    static float f = 0.0f;
    static int counter = 0;
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

    if (ImGui::Button("Button"))
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    DrawAnotherWindow(show_another_window);  // Call the function to show the "Another Window"
}

void ImGuiHandler::Shutdown()
{
    // Shutdown ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}