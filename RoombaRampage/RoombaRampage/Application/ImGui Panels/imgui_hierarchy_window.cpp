#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"
#include "../ECS/ECS.h"
#include "../../De&Serialization/json_handler.h"

#include<vector>
#include<string>
#include <iostream>

unsigned int ImGuiHandler::DrawHierachyWindow()
{
    //fetch ecs
    Ecs::ECS* ecs = Ecs::ECS::GetInstance();


    // Custom window with example widgets
    bool open = true;
    ImGui::Begin("Hierachy Window", &open, ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Rename"))
        {

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Prefabs"))
        {

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }


    //ImGui::Text("Roomba Rampage");

    if (ImGui::Button("+ Add GameObject"))
        ImGuiHandler::objectNameBox ? ImGuiHandler::objectNameBox = false : objectNameBox = true;
		
    if (objectNameBox)
    {
        if (ImGui::InputText("##", charBuffer, IM_ARRAYSIZE(charBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
        {

            //create ID then push into vector
            Ecs::EntityID newEntityID = ecs-> CreateEntity();
            obj_entity_id.push_back(newEntityID);

            //set new ID to be clicked
            clicked_entity_id = newEntityID;
            //Add the string into the vector
            obj_text_entries.push_back(std::string(charBuffer));

            //Set to false as no button showing first
            //Used to track and maintain sync between objtextentries and deletebutton vector
            deleteButton.push_back(false);
            DuplicateButton.push_back(false);
            obj_component_window.push_back(true);

            charBuffer[0] = '\0';
            objectNameBox = false;

            SaveComponentsJson("../RoombaRampage/Json Texts", Ecs::ECS::GetInstance()->ECS_EntityMap);
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
            DuplicateButton[i] ? DuplicateButton[i] = false : DuplicateButton[i] = true;

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
                Ecs::ECS::GetInstance()->DeleteEntity(obj_entity_id[i]);

                obj_component_window[i] = false;

                //remove the entries 
                obj_text_entries.erase(obj_text_entries.begin() + i);
                obj_entity_id.erase(obj_entity_id.begin() + i);
                deleteButton.erase(deleteButton.begin() + i);
                DuplicateButton.erase(DuplicateButton.begin() + i);
                obj_component_window.erase(obj_component_window.begin() + i);

                i--;

                ImGui::PopStyleColor(3);  // Pop the 3 style colors (button, hovered, and active)
                SaveComponentsJson("../RoombaRampage/Json Texts", Ecs::ECS::GetInstance()->ECS_EntityMap);
                continue;
            }

            ImGui::PopStyleColor(3);  // Pop the 3 style colors (button, hovered, and active)
        }

        //Render Duplicate button
        if (DuplicateButton[i])
        {
            //Use _button,_buttonhover_buttonactive
            //To change the button color
            //Dont forget to pop
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.5f, 0.1f, 1.0f));  // green
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.8f, 0.1f, 1.0f));  // Lighter red
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 1.f, 0.1f, 1.0f));  // Darker red

            //Use this to make the button side by side on the same line
            ImGui::SameLine();
            std::string DuplicateButtonLabel = "Duplicate ##" + std::to_string(i);

            if (ImGui::Button(DuplicateButtonLabel.c_str()))
            {

                //create ID then push into vector
                Ecs::EntityID newEntityID = ecs->DuplicateEntity(obj_entity_id[i]);
                obj_entity_id.push_back(newEntityID);

                //Add the string into the vector
                obj_text_entries.push_back(std::string(charBuffer));

                //Set to false as no button showing first
                //Used to track and maintain sync between objtextentries and deletebutton vector
                deleteButton.push_back(false);
                DuplicateButton.push_back(false);
                obj_component_window.push_back(false);

                charBuffer[0] = '\0';
                objectNameBox = false;

                ImGui::PopStyleColor(3);  // Pop the 3 style colors (button, hovered, and active)
                continue;
            }

            ImGui::PopStyleColor(3);  // Pop the 3 style colors (button, hovered, and active)
        }


    }

    ImGui::End();

    return clicked_entity_id;
}
