#include "../Graphics/GraphicsPipe.h"
#include "../Application/Helper.h"
#include "../Graphics/GraphicsCamera.h"

namespace graphicpipe
{
	int GraphicsCamera::m_windowWidth{};
	int GraphicsCamera::m_windowHeight{};
	float GraphicsCamera::m_aspectRatio{};
	std::unique_ptr<GraphicsCamera> GraphicsCamera::m_instancePtr = nullptr;
	GraphicsCamera::OrthoCamera GraphicsCamera::m_currCamera{ glm::vec2(0.f,0.f), glm::vec2(1.f,1.f), 0 };
	glm::mat3 GraphicsCamera::m_currCameraMatrix{1.f};
	glm::mat3 GraphicsCamera::m_currViewMatrix{ 1.f };
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
		m_currCameraMatrix[0][0] = m_currCamera.m_zoom.x;
		m_currCameraMatrix[1][1] = m_currCamera.m_zoom.y;
		m_currCameraMatrix[2][0] = m_currCamera.m_coordinates.x;
		m_currCameraMatrix[2][1] = m_currCamera.m_coordinates.y;
		m_currCameraMatrix[2][2] = 1;
	}

	void GraphicsCamera::calculateCurrView()
	{
		m_currViewMatrix = glm::inverse(m_currCameraMatrix);
	}

	void GraphicsCamera::multiplyOrthoMatrix()
	{
		GraphicsPipe* pipe = GraphicsPipe::m_funcGetInstance();
		glm::mat3 ortho{ 1.f };
		float left = -1.f * (1.f/ m_aspectRatio);
		float right = 1.f * (1.f /m_aspectRatio);
		float bottom = -1.f;
		float top = 1.f;
		ortho[0][0] = (2.0f / (right - left));
		ortho[1][1] = 2.0f / (top - bottom);
		ortho[2][0] = -(right + left) / (right - left);
		ortho[2][1] = -(top + bottom) / (top - bottom);
		ortho[2][2] = 1;

		for (glm::mat3& matrix : pipe->m_modelToNDCMatrix)
		{
			matrix = ortho * matrix;
		}

		for (glm::mat3& debugMatrix : pipe->m_debugToNDCMatrix)
		{
			debugMatrix = ortho * debugMatrix;
		}
		m_cameras.clear();
	}

	void GraphicsCamera::multiplyViewMatrix()
	{
		GraphicsPipe* pipe = GraphicsPipe::m_funcGetInstance();

		for (glm::mat3& matrix : pipe->m_modelToNDCMatrix)
		{
			matrix = m_currViewMatrix * matrix;
		}

		for (glm::mat3& debugMatrix : pipe->m_debugToNDCMatrix)
		{
			debugMatrix = m_currViewMatrix * debugMatrix;
		}
	}
}