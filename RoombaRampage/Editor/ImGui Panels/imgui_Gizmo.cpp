#include "imgui_handler.h"
#include "imgui_impl_opengl3.h"
#include "../Graphics/GraphicsPipe.h"
#include "../ECS/ECS.h"
#include "../Math/mathlib.h"
#include "../Application/Helper.h"


//for test
#include <glm.hpp>

namespace gui {

    //void OrthoGraphic(const float l, float r, float b, const float t, float zn, const float zf, float* m16)
    //{
    //    m16[0] = 2 / (r - l);
    //    m16[1] = 0.0f;
    //    m16[2] = 0.0f;
    //    m16[3] = 0.0f;
    //    m16[4] = 0.0f;
    //    m16[5] = 2 / (t - b);
    //    m16[6] = 0.0f;
    //    m16[7] = 0.0f;
    //    m16[8] = 0.0f;
    //    m16[9] = 0.0f;
    //    m16[10] = 1.0f / (zf - zn);
    //    m16[11] = 0.0f;
    //    m16[12] = (l + r) / (l - r);
    //    m16[13] = (t + b) / (b - t);
    //    m16[14] = zn / (zn - zf);
    //    m16[15] = 1.0f;
    //}


    void ImGuiHandler::m_DrawGizmo(float renderPosX, float renderPosY, float renderWidth, float renderHeight)
    {
        graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        Helper::Helpers* help = Helper::Helpers::GetInstance();
        float aspectRatio = help->m_windowHeight / help->m_windowWidth;

        ecs::TransformComponent* transcom = (ecs::TransformComponent*)ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(m_clickedEntityId);

        //refer to guizmo example for snapping
        ImGuizmo::SetOrthographic(true);
        ImGuizmo::SetDrawlist();

        ImGuiIO& io = ImGui::GetIO();
        //Set Viewport to be Render Screen
        ImGuizmo::SetRect(renderPosX, renderPosY, renderWidth, renderHeight);
        

        float projection[16] =
        { 1.f, 0.f, 0.f, 0.f,
          0.f, 1.f, 0.f, 0.f,
          0.f, 0.f, 1.f, 0.f,
          0.f, 0.f, 0.f, 1.f };

        projection[0] = pipe->m_editorCameraMatrix[0][0] * aspectRatio;
        projection[5] = pipe->m_editorCameraMatrix[1][1];
        projection[12] = pipe->m_editorCameraMatrix[2][0];
        projection[13] = pipe->m_editorCameraMatrix[2][1];

        /* projection[1] = pipe->m_editorCameraMatrix[0][1];
       projection[4] = pipe->m_editorCameraMatrix[1][0];
       projection[5] = pipe->m_editorCameraMatrix[1][1];
       projection[12] = pipe->m_editorCameraMatrix[2][0];
       projection[13] = pipe->m_editorCameraMatrix[2][1];*/
        //float viewWidth = 10.f;
        //float viewHeight = viewWidth * io.DisplaySize.y / io.DisplaySize.x;
        //OrthoGraphic(-viewWidth, viewWidth, -viewHeight, viewHeight, 1000.f, -1000.f, cameraProjection);

        float cameraView[16] =
        { 1.f, 0.f, 0.f, 0.f,
          0.f, 1.f, 0.f, 0.f,
          0.f, 0.f, 1.f, 0.f,
          0.f, 0.f, 0.f, 1.f };

        const mat3x3::Mat3x3& transformation = transcom->m_transformation;

        //colum major!!
        float model[16] =
        { transformation.m_e00, transformation.m_e01, 0.f, transformation.m_e02,
          transformation.m_e10, transformation.m_e11, 0.f, transformation.m_e12,
          0.f, 0.f, 1.f, 0.f,//z axis
          transformation.m_e20, transformation.m_e21, 0.f, transformation.m_e22
         };

        float matrixTranslation[3];
        float matrixRotation[3];
        float matrixScale[3];
        //ImGuizmo::DecomposeMatrixToComponents(model, matrixTranslation, matrixRotation, matrixScale);
        ////std::cout << matrixTranslation[0] << " , " << matrixTranslation[1] << std::endl;
        //ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, model);

        if (ImGuizmo::Manipulate(cameraView, projection, ImGuizmo::TRANSLATE, ImGuizmo::WORLD, model)) {
            ImGuizmo::DecomposeMatrixToComponents(model, matrixTranslation, matrixRotation, matrixScale);
            transcom->m_position.m_x = matrixTranslation[0];
            transcom->m_position.m_y = matrixTranslation[1];
            transcom->m_rotation = matrixRotation[2];
            transcom->m_scale.m_x = matrixScale[0];
            transcom->m_scale.m_y = matrixScale[1];
        }

	}


}
