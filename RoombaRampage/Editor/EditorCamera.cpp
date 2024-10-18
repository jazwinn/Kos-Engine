#include "../Editor/EditorCamera.h"


float EditorCamera::m_aspectRatio{};
int EditorCamera::m_windowWidth{};
int EditorCamera::m_windowHeight{};
bool EditorCamera::m_editorMode = true;
EditorCamera::Camera EditorCamera::m_editorCamera{};
glm::mat3 EditorCamera::m_editorCameraMatrix{1.f};
float EditorCamera::m_editorCameraDragSensitivity{ 0.01f };
float EditorCamera::m_editorCameraZoomSensitivity{ 0.01f };

void EditorCamera::calculateLevelEditorCamera()
{
	float left = m_editorCamera.m_coordinates.x - 1.f / m_editorCamera.m_zoom.x;
	float right = m_editorCamera.m_coordinates.x + 1.f / m_editorCamera.m_zoom.x;
	float bottom = m_editorCamera.m_coordinates.y - 1.f / m_editorCamera.m_zoom.y;
	float top = m_editorCamera.m_coordinates.y + 1.f / m_editorCamera.m_zoom.y;
	m_editorCameraMatrix[0][0] = 2.0f / (right - left);
	m_editorCameraMatrix[1][1] = 2.0f / (top - bottom);
	m_editorCameraMatrix[2][0] = -(right + left) / (right - left);
	m_editorCameraMatrix[2][1] = -(top + bottom) / (top - bottom);
	m_editorCameraMatrix[2][2] = 1;
}
