#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"
#include "imgui_internal.h"

#include "../../De&Serialization/json_handler.h"
#include "../ECS/ECS.h"

void gui::ImGuiHandler::m_DrawMainMenuBar() {

    ImGuiIO& io = ImGui::GetIO();  // Get input/output data
    //If CTRL + S press, save
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S))) {
        Serialization::Serialize::m_SaveComponentsJson("../RoombaRampage/Json", ecs::ECS::m_GetInstance()->m_ECS_EntityMap, m_objTextEntries, m_objEntityId);
        std::cout << "Saving data..." << std::endl;
    }

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save")) {


                Serialization::Serialize::m_SaveComponentsJson("../RoombaRampage/Json", ecs::ECS::m_GetInstance()->m_ECS_EntityMap, m_objTextEntries, m_objEntityId);

                
            }
            
            if (ImGui::BeginMenu("Open - Work In Progress")) {


                if (ImGui::MenuItem("Components.json")) {
                    //Serialization::Serialize::LoadComponentsJson("../RoombaRampage/Json/components.json", Ecs::ECS::GetInstance(), obj_text_entries);
                }

                ImGui::EndMenu();
            }

                

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window"))
        {

            ImGui::EndMenu();
        }


        ImGui::EndMainMenuBar();
    }

}