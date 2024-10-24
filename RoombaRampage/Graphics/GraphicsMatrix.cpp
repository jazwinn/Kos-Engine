#include "../Graphics/GraphicsPipe.h"
#include "../Graphics/GraphicsCamera.h"

namespace graphicpipe
{
	void GraphicsPipe::m_funcCalculateModelToWorldMatrix()
	{
		if (m_modelData.size() > 0)
		{
			for (int n{}; n < m_modelData.size(); n++)
			{
				float heightRatio = static_cast<float>(m_imageData[m_modelData[n].m_textureID].m_height) / m_unitHeight;
				float widthRatio = static_cast<float>(m_imageData[m_modelData[n].m_textureID].m_width) / m_unitWidth;

				float imageAspectRatio = static_cast<float>(m_imageData[m_modelData[n].m_textureID].m_width) / static_cast<float>(m_imageData[m_modelData[n].m_textureID].m_height);

				m_modelData[n].m_transformation[0][0] = m_modelData[n].m_transformation[0][0] * widthRatio / imageAspectRatio;
				m_modelData[n].m_transformation[0][1] = m_modelData[n].m_transformation[0][1] * widthRatio / imageAspectRatio;
				m_modelData[n].m_transformation[1][1] = m_modelData[n].m_transformation[1][1] * heightRatio;
				m_modelData[n].m_transformation[1][0] = m_modelData[n].m_transformation[1][0] * heightRatio;

				m_modelMatrix.push_back(m_modelData[n].m_transformation);
				m_textureOrder.push_back(m_modelData[n].m_textureID);
				m_stripCounts.push_back(m_imageData[m_modelData[n].m_textureID].m_stripCount);
				m_frameNumbers.push_back(m_modelData[n].m_frameNumber);
			}
			m_modelData.clear();
		}

		if (m_debugBoxData.size() > 0)
		{
			for (int i{}; i < m_debugBoxData.size(); i++)
			{
				m_debugToNDCMatrix.push_back(m_debugBoxData[i].m_transformation);
				m_debugBoxCollisionChecks.push_back(static_cast<float>(m_debugBoxData[i].m_isCollided));

			}
			m_debugBoxData.clear();
		}
	}
}