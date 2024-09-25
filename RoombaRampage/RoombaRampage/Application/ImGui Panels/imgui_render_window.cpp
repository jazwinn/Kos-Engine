#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"
#include "../ECS/ECS.h"
#include "../Application.h"

#include "../Graphics/GraphicsPipe.h"

void ImGuiHandler::DrawRenderScreenWindow(unsigned int windowWidth, unsigned int windowHeight)
{
    GraphicsPipe* pipe;
    pipe = GraphicsPipe::funcGetInstance();
    ImGui::Begin("Scene Window");
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddImage((void*)(long long unsigned int)pipe->screenTexture, pos,
        ImVec2(ImGui::GetCursorScreenPos().x + windowWidth,
            ImGui::GetCursorScreenPos().y + windowHeight),
        ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}