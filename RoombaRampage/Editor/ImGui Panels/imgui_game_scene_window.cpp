#include "imgui_handler.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Graphics/GraphicsCamera.h"

namespace gui
{
	void ImGuiHandler::m_DrawGameSceneWindow()
	{
        graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
        ImGui::Begin("Game Window");

        //graphicpipe::GraphicsCamera::m_currCameraMatrix = graphicpipe::GraphicsCamera::m_currCameraMatrix;
        //pipe->m_funcUpdate();
        //pipe->m_funcDrawWindow();
        


        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 renderWindowSize = ImGui::GetContentRegionAvail();
        float textureAspectRatio = graphicpipe::GraphicsCamera::m_windowWidth / graphicpipe::GraphicsCamera::m_windowHeight;
        float renderWindowAspectRatio = renderWindowSize.x / renderWindowSize.y;

        ImVec2 imageSize;
        imageSize.x = graphicpipe::GraphicsCamera::m_windowWidth / 2;
        imageSize.y = graphicpipe::GraphicsCamera::m_windowHeight / 2;


        if (renderWindowAspectRatio > textureAspectRatio)
        {
            imageSize.y = renderWindowSize.y;
            imageSize.x = imageSize.y * textureAspectRatio;
        }
        else
        {
            imageSize.x = renderWindowSize.x;
            imageSize.y = imageSize.x / textureAspectRatio;
        }

        if (imageSize.x <= renderWindowSize.x)
        {
            pos.x += (renderWindowSize.x - imageSize.x) / 2;
        }

        if (imageSize.y <= renderWindowSize.y)
        {
            pos.y += (renderWindowSize.y - imageSize.y) / 2;
        }

      

        ImGui::GetWindowDrawList()->AddImage(
            (void*)(long long unsigned int)pipe->m_screenTexture, pos,
            ImVec2(pos.x + imageSize.x, pos.y + imageSize.y),
            ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
	}

}