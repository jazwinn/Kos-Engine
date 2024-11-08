/******************************************************************/
/*!
\file      imgui_editor_window.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th November, 2024
\brief     This file implements the editor window interface for the
           application, using ImGui for the graphical user interface.

           The editor window provides an interactive interface for the
           user to manipulate and view objects in the application's
           scene, such as entities, cameras, and textures. It includes
           functionality for rendering the scene in a window, zooming,
           dragging, and resetting the camera view. Additionally, it
           supports drag-and-drop of assets (such as .png, .ttf, and
           .prefab files) into the editor window, automatically creating
           entities and setting their properties based on the file type.

           Key features of this file:
           - Draws the editor window and render screen.
           - Handles camera zoom, drag, and reset controls.
           - Manages drag-and-drop functionality for asset files.
           - Updates camera matrices and view transformations.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/


#include "../Config/pch.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"
#include "imgui_internal.h"


#include "../ECS/ECS.h"
#include "../Application/Application.h"

#include "../Graphics/GraphicsPipe.h"
#include "../Graphics/GraphicsCamera.h"
#include "../Editor/EditorCamera.h"

#include "../ECS/Hierachy.h"



void gui::ImGuiHandler::m_DrawRenderScreenWindow(unsigned int windowWidth, unsigned int windowHeight)
{
    graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
    ecs::ECS* ecs = ecs::ECS::m_GetInstance();

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_MenuBar;
    bool open = true;

    ImGui::Begin("Editor Window", &open, window_flags);


    m_DrawPlayPauseBar();
    

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 renderWindowSize = ImGui::GetContentRegionAvail();

    float textureAspectRatio = (float)windowWidth / (float)windowHeight;
    float renderWindowAspectRatio = renderWindowSize.x / renderWindowSize.y;

    ImVec2 imageSize;
    imageSize.x = windowWidth / 2.f;
    imageSize.y = windowHeight / 2.f;

    //Dynamic Window Resizing
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





    float scrollInput = ImGui::GetIO().MouseWheel; // Positive for zoom in, negative for zoom out

    if (ImGui::IsWindowHovered())
    {
        EditorCamera::m_editorCamera.m_zoom.x -= scrollInput * EditorCamera::m_editorCameraZoomSensitivity * EditorCamera::m_editorCamera.m_zoom.x;
        EditorCamera::m_editorCamera.m_zoom.y -= scrollInput * EditorCamera::m_editorCameraZoomSensitivity * EditorCamera::m_editorCamera.m_zoom.y;

        EditorCamera::m_editorCamera.m_zoom.x = glm::clamp(EditorCamera::m_editorCamera.m_zoom.x, 0.1f, 100.f);
        EditorCamera::m_editorCamera.m_zoom.y = glm::clamp(EditorCamera::m_editorCamera.m_zoom.y, 0.1f, 100.f);
    }
     
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
    {
        ImVec2 mouseDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);

        glm::vec2 delta = glm::vec2(mouseDelta.x, mouseDelta.y) * EditorCamera::m_editorCameraDragSensitivity * EditorCamera::m_editorCamera.m_zoom.x;

        // Update the camera position
        EditorCamera::m_editorCamera.m_coordinates.x -= delta.x;
        EditorCamera::m_editorCamera.m_coordinates.y += delta.y;
     
        ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);
    }

    //Reset Camera To Center
    if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_R))
    {
        EditorCamera::m_editorCamera.m_coordinates.x = 0.f;
        EditorCamera::m_editorCamera.m_coordinates.y = 0.f;
        EditorCamera::m_editorCamera.m_zoom.x = 1.f;
        EditorCamera::m_editorCamera.m_zoom.y = 1.f;
    }

    EditorCamera::calculateLevelEditorCamera();
    EditorCamera::calculateLevelEditorView();
    EditorCamera::calculateLevelEditorOrtho();

    graphicpipe::GraphicsCamera::m_currCameraMatrix = EditorCamera::m_editorCameraMatrix;
    graphicpipe::GraphicsCamera::m_currViewMatrix = EditorCamera::m_editorViewMatrix;

    if (graphicpipe::GraphicsCamera::m_cameras.size() == 0)
    {
        graphicpipe::GraphicsCamera::m_currCameraScaleX = EditorCamera::m_editorCamera.m_zoom.x;
        graphicpipe::GraphicsCamera::m_currCameraScaleY = EditorCamera::m_editorCamera.m_zoom.y;
        graphicpipe::GraphicsCamera::m_currCameraTranslateX = EditorCamera::m_editorCamera.m_coordinates.x;
        graphicpipe::GraphicsCamera::m_currCameraTranslateY = EditorCamera::m_editorCamera.m_coordinates.y;
        graphicpipe::GraphicsCamera::m_currCameraRotate = 0.f;
    }
  
    //Draw gizmo
    m_DrawGizmo(pos.x, pos.y, imageSize.x, imageSize.y);
    
    ImGui::Dummy(renderWindowSize);
    if (ImGui::BeginDragDropTarget())
    {

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("file"))
        {
            ecs::ECS* fileecs = ecs::ECS::m_GetInstance();

            IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
            std::filesystem::path* filename = static_cast<std::filesystem::path*>(payload->Data);

            float screencordX = ImGui::GetMousePos().x - pos.x;
            float screencordY = ImGui::GetMousePos().y - pos.y;

            //TODO calculate mouse pos correctly
            float cordX = (screencordX - imageSize.x / 2.f) / (imageSize.x / 2.f);
            float cordY = (std::abs(screencordY) - imageSize.y / 2.f) / (imageSize.y / 2.f);

            glm::vec3 translate = { cordX , -cordY, 0.f };
            translate.x *= EditorCamera::m_editorCameraMatrix[0][0];
            translate.y *= EditorCamera::m_editorCameraMatrix[1][1];
            translate.x *= 1.f / graphicpipe::GraphicsCamera::m_aspectRatio;
            translate.x += EditorCamera::m_editorCameraMatrix[2][0];
            translate.y += EditorCamera::m_editorCameraMatrix[2][1];
            

            if (filename->filename().extension().string() == ".png") {
                ecs::EntityID id = fileecs->m_CreateEntity(m_activeScene); //assign to active scene
                ecs::TransformComponent* transCom = static_cast<ecs::TransformComponent*>(fileecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id));
                transCom->m_position = { translate.x, translate.y };
                // Insert matrix
                ecs::NameComponent* nameCom = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id));
                nameCom->m_entityName = filename->filename().stem().string();
                ecs::SpriteComponent * spriteCom = static_cast<ecs::SpriteComponent*>(ecs->m_AddComponent(ecs::TYPESPRITECOMPONENT, id));
                spriteCom->m_imageFile = filename->filename().string();

                if (m_prefabSceneMode) {
                    ecs::Hierachy::m_SetParent(ecs->m_ECS_SceneMap.find(m_activeScene)->second.m_prefabID, id);

                }

                m_clickedEntityId = id;
            }
            if (filename->filename().extension().string() == ".ttf") {
                
                ecs::EntityID id = fileecs->m_CreateEntity(m_activeScene); //assign to top most scene
                ecs::TransformComponent* transCom = static_cast<ecs::TransformComponent*>(fileecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id));
                transCom->m_position = { translate.x, translate.y };
                // Insert matrix
                ecs::NameComponent* nameCom = static_cast<ecs::NameComponent*>(fileecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id));
                nameCom->m_entityName = filename->filename().stem().string();
                ecs::TextComponent* textCom = static_cast<ecs::TextComponent*>(fileecs->m_AddComponent(ecs::TYPETEXTCOMPONENT, id));
                textCom->m_fileName = filename->filename().string();

                if (m_prefabSceneMode) {
                    ecs::Hierachy::m_SetParent(ecs->m_ECS_SceneMap.find(m_activeScene)->second.m_prefabID, id);
                }

                m_clickedEntityId = id;
            }

            if (!m_prefabSceneMode && filename->filename().extension().string() == ".prefab") {//dont allow adding of prefab in prefab 
                ecs::EntityID id = prefab::Prefab::m_CreatePrefab(filename->filename().string(), m_activeScene);
                ecs::TransformComponent* transCom = static_cast<ecs::TransformComponent*>(fileecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id));
                transCom->m_position = { translate.x, translate.y };
            }


        }
        ImGui::EndDragDropTarget();
    }


    ImGui::End();


}



