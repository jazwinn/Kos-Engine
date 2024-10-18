#include "../Graphics/GraphicsPipe.h"
#include "../Application/Helper.h"
#include "../Graphics/GraphicsCamera.h"

namespace graphicpipe
{
	std::unique_ptr<GraphicsCamera> GraphicsCamera::m_instancePtr = nullptr;
	float GraphicsCamera::m_aspectRatio{};
	int GraphicsCamera::m_windowWidth{};
	int GraphicsCamera::m_windowHeight{};
	bool GraphicsCamera::m_editorMode = true;
	GraphicsCamera::Camera GraphicsCamera::m_editorCamera{};
	glm::mat3 GraphicsCamera::m_editorCameraMatrix{};
	float GraphicsCamera::m_editorCameraDragSensitivity{ 0.01f };
	float GraphicsCamera::m_editorCameraZoomSensitivity{ 0.01f };
	glm::mat3 GraphicsCamera::m_currCameraMatrix{};
	std::vector<glm::mat3> GraphicsCamera::cameraMatrices{};

	GraphicsCamera* GraphicsCamera::m_funcGetInstance()
	{
		if (!m_instancePtr)
		{
			m_instancePtr.reset(new GraphicsCamera{});
		}
		return m_instancePtr.get();
	}

	void GraphicsCamera::calculateAspectRatio()
	{
		m_windowWidth = static_cast<int>(Helper::Helpers::GetInstance()->m_windowWidth);
		m_windowHeight = static_cast<int>(Helper::Helpers::GetInstance()->m_windowHeight);
		m_aspectRatio = static_cast<float>(static_cast<float>(m_windowHeight) / static_cast<float>(m_windowWidth));
	}
	void GraphicsCamera::multiplyActiveCameraMatrix()
	{
		GraphicsPipe* pipe = GraphicsPipe::m_funcGetInstance();

		for (glm::mat3& matrix : pipe->m_modelToNDCMatrix)
		{
			matrix = m_currCameraMatrix * matrix;
		}
		
		for (glm::mat3& debugMatrix : pipe->m_debugToNDCMatrix)
		{
			debugMatrix = m_currCameraMatrix * debugMatrix;
		}
	}
	void GraphicsCamera::setActiveCamera(unsigned int index)
	{
		m_currCameraMatrix = cameraMatrices[index];
		return;
	}
	void GraphicsCamera::setLevelEditorCamera()
	{
		m_currCameraMatrix = m_editorCameraMatrix;
		return;
	}

	void GraphicsCamera::calculateLevelEditorCamera()
	{
		//Orthogonal Projection Camera
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
}