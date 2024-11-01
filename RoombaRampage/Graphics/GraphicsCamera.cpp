#include "../Graphics/GraphicsPipe.h"
#include "../Application/Helper.h"
#include "../Graphics/GraphicsCamera.h"

namespace graphicpipe
{
	int GraphicsCamera::m_windowWidth{};
	int GraphicsCamera::m_windowHeight{};
	float GraphicsCamera::m_aspectRatio{};
	std::unique_ptr<GraphicsCamera> GraphicsCamera::m_instancePtr = nullptr;
	glm::mat3 GraphicsCamera::m_currCameraMatrix{1.f};
	glm::mat3 GraphicsCamera::m_currViewMatrix{ 1.f };
	glm::mat3 GraphicsCamera::m_currOrthoMatrix{ 1.f };
	std::vector<glm::mat3> GraphicsCamera::m_cameras{};

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
		m_currCameraMatrix = m_cameras[index];
	}

	void GraphicsCamera::calculateAspectRatio()
	{
		m_windowWidth = static_cast<int>(Helper::Helpers::GetInstance()->m_windowWidth);
		m_windowHeight = static_cast<int>(Helper::Helpers::GetInstance()->m_windowHeight);
		m_aspectRatio = static_cast<float>(static_cast<float>(m_windowHeight) / static_cast<float>(m_windowWidth));
	}
	void GraphicsCamera::multiplyActiveCameraMatrix()
	{
		/*GraphicsPipe* pipe = GraphicsPipe::m_funcGetInstance();

		for (glm::mat3& matrix : pipe->m_modelToNDCMatrix)
		{
			matrix = m_currCameraMatrix * matrix;
		}
		
		for (glm::mat3& debugMatrix : pipe->m_debugToNDCMatrix)
		{
			debugMatrix = m_currCameraMatrix * debugMatrix;
		}*/
	
	}

	void GraphicsCamera::calculateCurrCamera()
	{

	}

	void GraphicsCamera::calculateCurrView()
	{
		m_currViewMatrix = glm::inverse(m_currCameraMatrix);
	}

	void GraphicsCamera::multiplyOrthoMatrix()
	{
		GraphicsPipe* pipe = GraphicsPipe::m_funcGetInstance();
		float left = -1.f * (1.f/ m_aspectRatio);
		float right = 1.f * (1.f /m_aspectRatio);
		float bottom = -1.f;
		float top = 1.f;
		m_currOrthoMatrix[0][0] = (2.0f / (right - left));
		m_currOrthoMatrix[1][1] = 2.0f / (top - bottom);
		m_currOrthoMatrix[2][0] = -(right + left) / (right - left);
		m_currOrthoMatrix[2][1] = -(top + bottom) / (top - bottom);
		m_currOrthoMatrix[2][2] = 1;

		for (glm::mat3& matrix : pipe->m_modelToNDCMatrix)
		{
			matrix = m_currOrthoMatrix * matrix;
		}

		for (glm::mat3& debugMatrix : pipe->m_debugBoxToNDCMatrix)
		{
			debugMatrix = m_currOrthoMatrix * debugMatrix;
		}

		for (glm::mat3& debugMatrix : pipe->m_debugCircleToNDCMatrix)
		{
			debugMatrix = m_currOrthoMatrix * debugMatrix;
		}
		//m_cameras.clear();
	}

	void GraphicsCamera::multiplyViewMatrix()
	{
		GraphicsPipe* pipe = GraphicsPipe::m_funcGetInstance();
		if (!(pipe->m_modelToNDCMatrix.empty()))
		{
			pipe->m_modelToNDCMatrix.clear();
		}
		for (const glm::mat3& matrix : pipe->m_modelMatrix)
		{
			pipe->m_modelToNDCMatrix.push_back(m_currViewMatrix * matrix);
		}
		for (glm::mat3& debugMatrix : pipe->m_debugBoxToNDCMatrix)
		{
			debugMatrix = m_currViewMatrix * debugMatrix;
		}
		for (glm::mat3& debugMatrix : pipe->m_debugCircleToNDCMatrix)
		{
			debugMatrix = m_currViewMatrix * debugMatrix;
		}

	
	}
	
}