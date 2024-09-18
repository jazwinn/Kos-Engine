#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"
#include "../ECS/ECS.h"
#include<vector>
#include<string>
#include <iostream>

unsigned int ImGuiHandler::DrawHierachyWindow(ImVec4& clear_color)
{
    //fetch ecs
    ECS* ecs = ECS::GetInstance();

    // Custom window with example widgets
    ImGui::Begin("Hierachy Window");

    ImGui::Text("Roomba Rampage");

    if (ImGui::Button("+ Add GameObject"))
        ImGuiHandler::objectNameBox ? ImGuiHandler::objectNameBox = false : objectNameBox = true;
		
    if (objectNameBox)
    {
        if (ImGui::InputText("##", charBuffer, IM_ARRAYSIZE(charBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            //create ID then push into vector
            EntityID newEntityID = ecs-> CreateEntity();
            obj_entity_id.push_back(newEntityID);

            //Add the string into the vector
            obj_text_entries.push_back(std::string(charBuffer));

            //Set to false as no button showing first
            //Used to track and maintain sync between objtextentries and deletebutton vector
            deleteButton.push_back(false);
            obj_component_window.push_back(false);

            charBuffer[0] = '\0';
            objectNameBox = false;

        }
    }

    //For loop to display all the gameobj text as button 
    for (size_t i = 0; i < obj_text_entries.size(); i++)
    {
        // '##' let IMGui set an internal unique ID to widget without visible label!
        std::string buttonName = obj_text_entries[i] +"##"+ std::to_string(i);
        if (ImGui::Button(buttonName.c_str()))
        {
            deleteButton[i] ? deleteButton[i] = false : deleteButton[i] = true;

            obj_component_window[i] = true;

            clicked_entity_id = obj_entity_id[i];
            std::cout << "Entity ID clicked: " << clicked_entity_id << std::endl; //For debug purposes, remove later
        }

        //Render delete button
        if (deleteButton[i])
        {
            //Use _button,_buttonhover_buttonactive
            //To change the button color
            //Dont forget to pop
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.02f, 0.2f, 1.0f));  // Red 
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));  // Lighter red
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.1f, 0.1f, 1.0f));  // Darker red

            //Use this to make the button side by side on the same line
            ImGui::SameLine();
            std::string deleteButtonLabel = "Delete ##" + std::to_string(i);

            if (ImGui::Button(deleteButtonLabel.c_str()))
            {

                //Delete entity from ecs
                ECS::GetInstance()->DeleteEntity(obj_entity_id[i]);

                obj_component_window[i] = false;

                //remove the entries 
                obj_text_entries.erase(obj_text_entries.begin() + i);
                obj_entity_id.erase(obj_entity_id.begin() + i);
                deleteButton.erase(deleteButton.begin() + i);
                obj_component_window.erase(obj_component_window.begin() + i);

                i--;

                ImGui::PopStyleColor(3);  // Pop the 3 style colors (button, hovered, and active)
                continue;
            }

            ImGui::PopStyleColor(3);  // Pop the 3 style colors (button, hovered, and active)
        }

        //Render the game object component window
        if (obj_component_window[i] && obj_entity_id[i] == clicked_entity_id)
        {
            bool windowOpen = obj_component_window[i];  // Store the value in a temporary bool
            std::string windowTitle = obj_text_entries[i] + "'s Component Window ";

            DrawComponentWindow(ecs, obj_entity_id[i], windowOpen, windowTitle);

            obj_component_window[i] = windowOpen;
        }
    }

    ImGui::End();

    return clicked_entity_id;
}
