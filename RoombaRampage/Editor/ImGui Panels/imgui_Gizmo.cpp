/********************************************************************/
/*!
\file      Prefab.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\brief     This file contains the implementation of the `ImGuiHandler` class's
           function for rendering and manipulating transformation gizmos in the editor.
           - m_DrawGizmo: Renders a transformation gizmo for an entity, allowing translation,
             rotation, and scaling operations within the editor view.

This file leverages ImGuizmo to enable real-time manipulation of entity transforms
in an ECS-based game engine editor, supporting operations in both world and local
space with snapping options.

Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/


#include "../Config/pch.h"
#include "Editor.h"
#include "imgui_impl_opengl3.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Editor/EditorCamera.h"
#include "../ECS/ECS.h"
#include "../Math/mathlib.h"
#include "../Application/Helper.h"
#include "../Math/Mat3x3.h"
#include "../ECS/Hierachy.h"


namespace gui {



    void ImGuiHandler::m_DrawGizmo(float renderPosX, float renderPosY, float renderWidth, float renderHeight)
    {
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        ImGuizmo::SetOrthographic(true);
        ImGuizmo::SetDrawlist();

        static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
        static bool useSnap{false};
        static bool focusMode{ false };

        static float snap[3] = { 1.f, 1.f, 1.f };

        if (ImGui::IsWindowHovered() && ImGui::IsKeyPressed(ImGuiKey_W) && !ImGuizmo::IsUsing())
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        if (ImGui::IsWindowHovered() && ImGui::IsKeyPressed(ImGuiKey_E) && !ImGuizmo::IsUsing())
            mCurrentGizmoOperation = ImGuizmo::ROTATE;
        if (ImGui::IsWindowHovered() && ImGui::IsKeyPressed(ImGuiKey_R) && !ImGuizmo::IsUsing())
            mCurrentGizmoOperation = ImGuizmo::SCALE;
        if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
        {
            if (ImGui::IsWindowHovered() && ImGui::IsKeyPressed(ImGuiKey_Q)) {
                useSnap = useSnap?false:true;
            }

        }


        //float n = -1.f;
        //float f = 1.f;

        float projection[16] =
        { 1.f, 0.f, 0.f, 0.f,
          0.f, 1.f, 0.f, 0.f,
          0.f, 0.f, 1, 0.f,
          0.f, 0.f, 0.f, 1.f };

        projection[0] = EditorCamera::m_editorOrthoMatrix.m_e00;
        projection[5] = EditorCamera::m_editorOrthoMatrix.m_e11;
        projection[12] = EditorCamera::m_editorOrthoMatrix.m_e20;
        projection[13] = EditorCamera::m_editorOrthoMatrix.m_e21;


        float cameraView[16] =
        { 1.f, 0.f, 0.f, 0.f,
          0.f, 1.f, 0.f, 0.f,
          0.f, 0.f, 1.f, 0.f,
          0.f, 0.f, 0.f, 1.f };

        cameraView[0] = EditorCamera::m_editorViewMatrix.m_e00;
        cameraView[5] = EditorCamera::m_editorViewMatrix.m_e11;
        cameraView[12] = EditorCamera::m_editorViewMatrix.m_e20;
        cameraView[13] = EditorCamera::m_editorViewMatrix.m_e21;

        float identity[16] =
        { 1.f, 0.f, 0.f, 0.f,
          0.f, 1.f, 0.f, 0.f,
          0.f, 0.f, 1.f, 0.f,
          0.f, 0.f, 0.f, 1.f };


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

        gridviewmatrix[0] = EditorCamera::m_editorViewMatrix.m_e00;
        gridviewmatrix[9] = EditorCamera::m_editorViewMatrix.m_e11;
        gridviewmatrix[12] = EditorCamera::m_editorViewMatrix.m_e20;
        gridviewmatrix[13] = EditorCamera::m_editorViewMatrix.m_e21;

       // ImGuizmo::DrawGrid(gridviewmatrix, projection, identity, 100.f);

        /**************************************************************************************************/

        //check if any guizmo is clicked
        if (m_clickedEntityId < 0) return;
        ecs::TransformComponent* transcom = (ecs::TransformComponent*)ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(m_clickedEntityId);


        const mat3x3::Mat3x3& transformation = transcom->m_transformation;

        //colum major!!
        float model[16] =
        { transformation.m_e00, transformation.m_e01, 0.f, transformation.m_e02,
          transformation.m_e10, transformation.m_e11, 0.f, transformation.m_e12,
          0.f, 0.f, 1.f, 0.f,//z axis
          transformation.m_e20, transformation.m_e21, 0.f, transformation.m_e22
        };

        float matrixTranslation[3] = { transcom->m_position.m_x, transcom->m_position.m_y, 0 };
        float matrixRotation[3] = {0,0, transcom->m_rotation };
        float matrixScale[3] = { transcom->m_scale.m_x, transcom->m_scale.m_y, 0};

        //Focus mode for objects





        ImGuizmo::SetGizmoSizeClipSpace(EditorCamera::m_editorCamera.m_zoom.m_x / 8.f);
        //DRAW GIZMO
                //to render in full screen also
        if (!ImGui::IsWindowAppearing()){
            ImGuizmo::SetRect(renderPosX, renderPosY, renderWidth, renderHeight);
        }
        //TODO be able to swap between WORLD and LOCAL
        if (ImGuizmo::Manipulate(cameraView, projection, mCurrentGizmoOperation, ImGuizmo::WORLD, model, 0, useSnap ? &snap[0] : NULL)) {

            
            if (ecs::Hierachy::m_GetParent(m_clickedEntityId).has_value()) {
                ecs::EntityID parentId = ecs::Hierachy::m_GetParent(m_clickedEntityId).value();
                mat3x3::Mat3x3 worldtransformation =
                { model[0], model[1], model[3],
                  model[4], model[5], model[7],
                  model[12], model[13], model[15]
                };

                mat3x3::Mat3x3 parenttransformation = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(parentId))->m_transformation;
                mat3x3::Mat3x3 inverseparent;
                mat3x3::Mat3Inverse( parenttransformation, inverseparent);

                mat3x3::Mat3x3 childtransformation = inverseparent * worldtransformation;

                float childmodel[16] =
                { childtransformation.m_e00, childtransformation.m_e01, 0.f, childtransformation.m_e02,
                  childtransformation.m_e10, childtransformation.m_e11, 0.f, childtransformation.m_e12,
                  0.f, 0.f, 1.f, 0.f,//z axis
                  childtransformation.m_e20, childtransformation.m_e21, 0.f, childtransformation.m_e22
                };

                ImGuizmo::DecomposeMatrixToComponents(childmodel, matrixTranslation, matrixRotation, matrixScale);

            }
            else {
                ImGuizmo::DecomposeMatrixToComponents(model, matrixTranslation, matrixRotation, matrixScale);
            }

      

            
            transcom->m_position.m_x = matrixTranslation[0];
            transcom->m_position.m_y = matrixTranslation[1];
            transcom->m_rotation = -matrixRotation[2];
            transcom->m_scale.m_x = matrixScale[0];
            transcom->m_scale.m_y = matrixScale[1];
            //transcom->m_transformation = mat3x3::Mat3Transform(transcom->m_position, transcom->m_scale, transcom->m_rotation);



        
        }
        else {


          
        }

        if (ImGui::IsKeyDown(ImGuiKey_LeftShift) && ImGui::IsKeyPressed(ImGuiKey_F)) {
            focusMode = focusMode ? false : true;
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_F)) {
            EditorCamera::m_editorCamera.m_coordinates.m_x =  transcom->m_transformation.m_e20;
            EditorCamera::m_editorCamera.m_coordinates.m_y =  transcom->m_transformation.m_e21;
        }
        if (focusMode) {
            // UDB not working as intended
            const auto& coordinate = mathlibrary::mathlib::Mix(vector2::Vec2(EditorCamera::m_editorCamera.m_coordinates.m_x, EditorCamera::m_editorCamera.m_coordinates.m_y), vector2::Vec2{ transcom->m_transformation.m_e20 , transcom->m_transformation.m_e21 }, Helper::Helpers::GetInstance()->m_deltaTime * 3.5f);
            EditorCamera::m_editorCamera.m_coordinates.m_x = coordinate.m_x;
            EditorCamera::m_editorCamera.m_coordinates.m_y = coordinate.m_y;


        }
        static ecs::TransformComponent tempComp;
        static bool wasClicked = false;
        if (ImGuizmo::IsUsingAny()) {
            if (!wasClicked) {
                wasClicked = true;
                tempComp = *transcom;
            }
        }
        if (!ImGuizmo::IsUsingAny() && wasClicked) {
            wasClicked = false;
            events::TransformComponentChanged action(ecs::TYPETRANSFORMCOMPONENT, m_clickedEntityId, transcom, tempComp);
            DISPATCH_ACTION_EVENT(action);
        }
	}


}
