#include "../Graphics/GraphicsPipe.h"
#include "../Application/Helper.h"
#include "../Graphics/GraphicsCamera.h"

namespace graphicpipe
{
	int GraphicsCamera::m_windowWidth{};
	int GraphicsCamera::m_windowHeight{};
	float GraphicsCamera::m_aspectRatio{};
	std::unique_ptr<GraphicsCamera> GraphicsCamera::m_instancePtr = nullptr;
	GraphicsCamera::OrthoCamera GraphicsCamera::m_currCamera{};
	glm::mat3 GraphicsCamera::m_currCameraMatrix{1.f};
	std::vector<GraphicsCamera::OrthoCamera> GraphicsCamera::m_cameras{};

	GraphicsCamera* GraphicsCamera::m_funcGetInstance()
	{
		if (!m_instancePtr)
		{
			m_instancePtr.reset(new GraphicsCamera{});
		}
		return m_instancePtr.get();
	}

	void GraphicsCamera::setCurrCamera(unsigned int index)
	{
		m_currCamera = m_cameras[index];
	}

	void GraphicsCamera::calculateAspectRatio()
	{
		m_windowWidth = static_cast<float>(Helper::Helpers::GetInstance()->m_windowWidth);
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

	void GraphicsCamera::calculateCurrCamera()
	{
		float left = m_currCamera.m_coordinates.x - 1.f / m_currCamera.m_zoom.x;
		float right = m_currCamera.m_coordinates.x + 1.f / m_currCamera.m_zoom.x;
		float bottom = m_currCamera.m_coordinates.y - 1.f / m_currCamera.m_zoom.y;
		float top = m_currCamera.m_coordinates.y + 1.f / m_currCamera.m_zoom.y;
		m_currCameraMatrix[0][0] = 2.0f / (right - left);
		m_currCameraMatrix[1][1] = 2.0f / (top - bottom);
		m_currCameraMatrix[2][0] = -(right + left) / (right - left);
		m_currCameraMatrix[2][1] = -(top + bottom) / (top - bottom);
		m_currCameraMatrix[2][2] = 1;
	}
}