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

        static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
        static bool useSnap(false);
        static float snap[3] = { 1.f, 1.f, 1.f };

        if (ImGui::IsKeyPressed(ImGuiKey_T))
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        if (ImGui::IsKeyPressed(ImGuiKey_E))
            mCurrentGizmoOperation = ImGuizmo::ROTATE;
        if (ImGui::IsKeyPressed(ImGuiKey_R))
            mCurrentGizmoOperation = ImGuizmo::SCALE;
        if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
        {
            if (ImGui::IsKeyPressed(ImGuiKey_Q)) {
                useSnap = true;
            }

        }


        float n = -1.f;
        float f = 1.f;

        float projection[16] =
        { 1.f, 0.f, 0.f, 0.f,
          0.f, 1.f, 0.f, 0.f,
          0.f, 0.f, 1, 0.f,
          0.f, 0.f, 0.f, 1.f };

        projection[0] = pipe->m_editorCameraMatrix[0][0] * aspectRatio;
        projection[5] = pipe->m_editorCameraMatrix[1][1];
        projection[12] = pipe->m_editorCameraMatrix[2][0];
        projection[13] = pipe->m_editorCameraMatrix[2][1];


        float cameraView[16] =
        { 1.f, 0.f, 0.f, 0.f,
          0.f, 1.f, 0.f, 0.f,
          0.f, 0.f, 1.f, 0.f,
          0.f, 0.f, 0.f, 1.f };

        float identity[16] =
        { 1.f, 0.f, 0.f, 0.f,
          0.f, 1.f, 0.f, 0.f,
          0.f, 0.f, 1.f, 0.f,
          0.f, 0.f, 0.f, 1.f };


        static float upx{}, upy{}, upz{}, camx{}, camy{}, camz{}, targetx{}, targety{}, targetz{};
        ImGui::Begin("test");
        ImGui::DragFloat("testcam X##", &camx, 0.02f, -2.f, 2.f, "%.2f");
        ImGui::DragFloat("testcam y##", &camy, 0.02f, -2.f, 2.f, "%.2f");
        ImGui::DragFloat("testcam z##", &camz, 0.02f, -2.f, 2.f, "%.2f");

        ImGui::DragFloat("target X##", &targetx, 0.02f, -2.f, 2.f, "%.2f");
        ImGui::DragFloat("target y##", &targety, 0.02f, -2.f, 2.f, "%.2f");
        ImGui::DragFloat("target z##", &targetz, 0.02f, -2.f, 2.f, "%.2f");

        ImGui::DragFloat("testX##", &upx, 0.02f, -2.f, 2.f, "%.2f");
        ImGui::DragFloat("testy##", &upy, 0.02f, -2.f, 2.f, "%.2f");
        ImGui::DragFloat("testz##", &upz, 0.02f, -2.f, 2.f, "%.2f");



        ImGui::End();

        // dont ask how just do view transform matrix
        // bellow matrix derived from
        // pos - 0,1,0
        // target - 0,0,0
        // up - 0,0,-1
        float gridviewmatrix[] =
        { 1.f,0.f,0.f,0.f,
          0.f,0.f,1.f,0.f,
          0.f,1.f,0.f,0.f,
          0.f,0.f,1.f,1.f
        };



        ImGuizmo::DrawGrid(gridviewmatrix, projection, identity, 100.f);

        /**************************************************************************************************/

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

        //DRAW GIZMO
                //to render in full screen also
        ImGuizmo::SetRect(renderPosX, renderPosY, renderWidth, renderHeight);
        if (ImGuizmo::Manipulate(cameraView, projection, mCurrentGizmoOperation, ImGuizmo::WORLD, model, NULL, useSnap? &snap[0] : NULL)) {
            ImGuizmo::DecomposeMatrixToComponents(model, matrixTranslation, matrixRotation, matrixScale);
            transcom->m_position.m_x = matrixTranslation[0];
            transcom->m_position.m_y = matrixTranslation[1];
            transcom->m_rotation = matrixRotation[2];
            transcom->m_scale.m_x = matrixScale[0];
            transcom->m_scale.m_y = matrixScale[1];
        }

	}


}
