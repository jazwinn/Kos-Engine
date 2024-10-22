#include "../Editor/EditorCamera.h"
#include <iostream>

float EditorCamera::m_aspectRatio{};
int EditorCamera::m_windowWidth{};
int EditorCamera::m_windowHeight{};
bool EditorCamera::m_editorMode = true;
EditorCamera::Camera EditorCamera::m_editorCamera{ 1.f, 1.f ,glm::vec2(0.f,0.f), glm::vec2(1.f,1.f), 0 };
glm::mat3 EditorCamera::m_editorCameraMatrix{1.f};
glm::mat3 EditorCamera::m_editorViewMatrix{ 1.f };

float EditorCamera::m_editorCameraDragSensitivity{ 0.01f };
float EditorCamera::m_editorCameraZoomSensitivity{ 0.01f };

void EditorCamera::calculateLevelEditorCamera()
{
	/*std::cout << "Editor X: " << m_editorCamera.m_coordinates.x << std::endl;
	float left = (m_editorCamera.m_coordinates.x - 1.f / m_editorCamera.m_zoom.x);
	std::cout << "Editor Left: " << left << std::endl;
	float right = (m_editorCamera.m_coordinates.x + 1.f / m_editorCamera.m_zoom.x);
	float bottom = m_editorCamera.m_coordinates.y - 1.f / m_editorCamera.m_zoom.y;
	float top = m_editorCamera.m_coordinates.y + 1.f / m_editorCamera.m_zoom.y;
	m_editorCameraMatrix[0][0] = (2.0f / (right - left));
	m_editorCameraMatrix[1][1] = 2.0f / (top - bottom);
	m_editorCameraMatrix[2][0] = -(right + left) / (right - left);
	m_editorCameraMatrix[2][1] = -(top + bottom) / (top - bottom);
	m_editorCameraMatrix[2][2] = 1;*/

	std::cout << "Editor X: " << m_editorCamera.m_coordinates.x << std::endl;
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

void EditorCamera::calculateLevelEditorNDC()
{
	
}
