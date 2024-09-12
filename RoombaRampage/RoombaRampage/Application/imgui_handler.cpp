#include "../Application/imgui_handler.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "../ECS/ECS.h"

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

void ImGuiHandler::DrawHierachyWindow(bool& show_demo_window, bool& show_another_window, ImVec4& clear_color)
{
    //fetch ecs
    ECS* ecs = ECS::GetInstance();


    // Custom window with example widgets
    ImGui::Begin("Hierachy Window");

    ImGui::Text("Roomba Rampage");

    if (ImGui::Button("+ Add GameObject")) {
        
        stringBox ? stringBox = false : stringBox = true;
    }
		

    if (stringBox)
    {
        if (ImGui::InputText("##", charBuffer, IM_ARRAYSIZE(charBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
        {

            //Add the string into the vector
            obj_text_entries.push_back(std::string(charBuffer));

            //Set to false as no button showing first
            //Used to track and maintain sync between objtextentries and deletebutton vector
            deleteButton.push_back(false);
            obj_component_window.push_back(false);

            charBuffer[0] = '\0';
            stringBox = false;
        }
    }

    //For loop to display all the gameobj text as button 
    for (size_t i = 0; i < obj_text_entries.size(); i++)
    {
        // '##' let IMGui set an internal unique ID to widget without visible label!
        std::string buttonName = obj_text_entries[i] +"##"+ std::to_string(i);
        if (ImGui::Button(buttonName.c_str()))
        {
            deleteButton[i] = true;
            obj_component_window[i] = true;
        }

        //Render delete button
        if (deleteButton[i])
        {
            //Use _button,_buttonhover_buttonactive
            //To change the button color
            //Dont forget to pop
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));  // Red 
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));  // Lighter red
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.1f, 0.1f, 1.0f));  // Darker red

            //Use this to make the button side by side on the same line
            ImGui::SameLine();
            std::string deleteButtonLabel = "Delete ##" + std::to_string(i);
            if (ImGui::Button(deleteButtonLabel.c_str()))
            {

                //Delete entity from ecs
                //ecs->DeleteEntity();


                obj_component_window[i] = false;

                //remove the entries 
                obj_text_entries.erase(obj_text_entries.begin() + i);
                deleteButton.erase(deleteButton.begin() + i);
                obj_component_window.erase(obj_component_window.begin() + i);

                i--;

                ImGui::PopStyleColor(3);  // Pop the 3 style colors (button, hovered, and active)
                continue;
            }

            ImGui::PopStyleColor(3);  // Pop the 3 style colors (button, hovered, and active)
        }

        //Render the game object component window
        if (obj_component_window[i])
        {
            bool windowOpen = obj_component_window[i];  // Store the value in a temporary bool
            std::string windowTitle = obj_text_entries[i] + "'s Component Window ";

            ImGui::Begin(windowTitle.c_str(), &windowOpen);  // Window title based on index
            ImGui::Text("This is the separate window for %s", obj_text_entries[i].c_str());
            ImGui::End();

            obj_component_window[i] = windowOpen;
        }
    }

    ImGui::End();
}

void ImGuiHandler::Shutdown()
{
    // Shutdown ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}