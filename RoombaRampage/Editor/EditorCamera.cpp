/******************************************************************/
/*!
\file      EditorCamera.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th November, 2024
\brief     This file contains the implementation of the EditorCamera
           class, which provides camera control for the editor view
           in a graphical application.

           The EditorCamera is responsible for setting up and managing
           the orthographic view, including the camera's zoom, drag
           sensitivity, and aspect ratio. It provides methods to:
           - Calculate the camera transformation matrix.
           - Calculate the view matrix based on the camera position.
           - Set up an orthographic projection matrix.

           These calculations enable smooth zooming and dragging within
           the editor, ensuring the camera view can be adjusted
           interactively by the user.

           Key features of this file:
           - Configurable camera zoom and drag sensitivity.
           - Camera matrix, view matrix, and orthographic projection.
           - Static members to control aspect ratio and window size.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/


#include "../Config/pch.h"
#include "../Editor/EditorCamera.h"
#include "../Graphics/GraphicsCamera.h"
#include <iostream>

float EditorCamera::m_aspectRatio{};
int EditorCamera::m_windowWidth{};
int EditorCamera::m_windowHeight{};
glm::ivec2 EditorCamera::m_editorWindowPosition{};
glm::ivec2 EditorCamera::m_editorWindowDimensions{};
bool EditorCamera::m_editorMode = true;
EditorCamera::Camera EditorCamera::m_editorCamera{ 1.f, 1.f ,glm::vec2(0.f,0.f), glm::vec2(5.f,5.f), 0 }; // Zoom Set To 5.f on Init
glm::mat3 EditorCamera::m_editorCameraMatrix{1.f};
glm::mat3 EditorCamera::m_editorViewMatrix{ 1.f };
glm::mat3 EditorCamera::m_editorOrthoMatrix{ 1.f };

float EditorCamera::m_editorCameraDragSensitivity{ 0.01f };
float EditorCamera::m_editorCameraZoomSensitivity{ 0.10f };

void EditorCamera::calculateLevelEditorCamera()
{
	m_editorCameraMatrix[0][0] = m_editorCamera.m_zoom.x;
	m_editorCameraMatrix[1][1] = m_editorCamera.m_zoom.y;
	m_editorCameraMatrix[2][0] = m_editorCamera.m_coordinates.x;
	m_editorCameraMatrix[2][1] = m_editorCamera.m_coordinates.y;
	m_editorCameraMatrix[2][2] = 1;
}

void EditorCamera::calculateLevelEditorView()
{
	m_editorViewMatrix = glm::inverse(m_editorCameraMatrix);
}

void EditorCamera::calculateLevelEditorOrtho()
{
    float left = -1.f * (1.f/graphicpipe::GraphicsCamera::m_aspectRatio);
    float right = 1.f * (1.f/graphicpipe::GraphicsCamera::m_aspectRatio);
    float bottom = -1.f;
    float top = 1.f;
    m_editorOrthoMatrix[0][0] = (2.0f / (right - left));
    m_editorOrthoMatrix[1][1] = 2.0f / (top - bottom);
    m_editorOrthoMatrix[2][0] = -(right + left) / (right - left);
    m_editorOrthoMatrix[2][1] = -(top + bottom) / (top - bottom);
    m_editorOrthoMatrix[2][2] = 1;
}

vector2::Vec2 EditorCamera::calculateWorldCoordinatesFromMouse(int mouseX, int mouseY)
{
    float screencoordX = mouseX - m_editorWindowPosition.x;
    float screencoordY = mouseY - m_editorWindowPosition.y;

    float cordX = (screencoordX - m_editorWindowDimensions.x / 2.f) / (m_editorWindowDimensions.x / 2.f);
    float cordY = (std::abs(screencoordY) - m_editorWindowDimensions.y / 2.f) / (m_editorWindowDimensions.y / 2.f);

    glm::vec3 translate = { cordX , -cordY, 0.f };
    translate.x *= EditorCamera::m_editorCameraMatrix[0][0];
    translate.y *= EditorCamera::m_editorCameraMatrix[1][1];
    translate.x *= 1.f / graphicpipe::GraphicsCamera::m_aspectRatio;
    translate.x += EditorCamera::m_editorCameraMatrix[2][0];
    translate.y += EditorCamera::m_editorCameraMatrix[2][1];

    return vector2::Vec2{ translate.x, translate.y };
}
