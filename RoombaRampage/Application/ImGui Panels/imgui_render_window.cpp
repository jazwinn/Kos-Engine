#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"
#include "../ECS/ECS.h"
#include "../Application.h"

#include "../Graphics/GraphicsPipe.h"

void gui::ImGuiHandler::m_DrawRenderScreenWindow(unsigned int windowWidth, unsigned int windowHeight)
{
   /* GraphicsPipe* pipe;
    pipe = GraphicsPipe::funcGetInstance();
    ImGui::Begin("Scene Window");
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddImage((void*)(long long unsigned int)pipe->screenTexture, pos,
        ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth(),
            ImGui::GetCursorScreenPos().y + ImGui::GetWindowHeight()),
        ImVec2(0, 1), ImVec2(1, 0)); 

    ImGui::End();*/

    graphicpipe::GraphicsPipe* pipe;
    pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
    ImGui::Begin("Scene Window");

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 windowSize = ImGui::GetContentRegionAvail();

    float textureAspectRatio = (float)windowWidth / (float)windowHeight;
    float windowAspectRatio = windowSize.x / windowSize.y;

    ImVec2 imageSize;

    if (windowAspectRatio > textureAspectRatio) 
    {
        imageSize.y = windowSize.y;
        imageSize.x = imageSize.y * textureAspectRatio;
    }
    else 
    {
        imageSize.x = windowSize.x;
        imageSize.y = imageSize.x / textureAspectRatio;
    }

    if (imageSize.x <= windowSize.x)
    {
        pos.x += (windowSize.x - imageSize.x) / 2;
    }

    if (imageSize.y <= windowSize.y)
    {
        pos.y += (windowSize.y - imageSize.y) / 2;
    }

    ImGui::GetWindowDrawList()->AddImage(
        (void*)(long long unsigned int)pipe->m_screenTexture, pos,
        ImVec2(pos.x + imageSize.x, pos.y + imageSize.y),
        ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
   
}