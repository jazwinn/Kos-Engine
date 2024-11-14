/******************************************************************/
/*!
\file      GraphicsMatrix.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th Nov, 2024
\brief     This file implements the `GraphicsPipe` class functions for handling model transformations
		   and managing debug box and circle data within a graphics pipeline.

\details   The `GraphicsPipe` class includes functionality for calculating the transformation matrices
		   for models, handling texture data, and performing collision checks for debug geometries.
		   This file contains the implementation of matrix calculations that transform models from
		   local space to world space, as well as processing debug data (boxes and circles) for visual
		   debugging. The class supports managing model data and generating transformation matrices
		   for rendering models and debugging.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "../Config/pch.h"
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
				m_stripCounts.push_back({ m_imageData[m_modelData[n].m_textureID].m_stripCount, m_modelData[n].m_frameNumber });
				//m_frameNumbers.push_back(m_modelData[n].m_frameNumber);
				m_layers.push_back(m_modelData[n].m_layer);
				m_colors.push_back(m_modelData[n].m_color);
			}
			m_modelData.clear();
		}

		if (m_debugBoxData.size() > 0)
		{
			for (int i{}; i < m_debugBoxData.size(); i++)
			{
				if (m_debugBoxData[i].m_shapeType == SQUARE_LINES)
				{
					m_debugBoxToNDCMatrix.push_back(m_debugBoxData[i].m_transformation);
					m_debugBoxCollisionChecks.push_back(static_cast<float>(m_debugBoxData[i].m_isCollided));
				}
				else
				{
					m_debugCircleToNDCMatrix.push_back(m_debugBoxData[i].m_transformation);
					m_debugCircleCollisionChecks.push_back(static_cast<float>(m_debugBoxData[i].m_isCollided));
				}
			}
			m_debugBoxData.clear();
		}

		if (m_tilemapData.size() > 0)
		{
			for (int n{}; n < m_tilemapData.size(); n++)
			{
				float heightRatio = static_cast<float>(m_imageData[m_tilemapData[n].m_textureID].m_height) / m_unitHeight;
				float widthRatio = static_cast<float>(m_imageData[m_tilemapData[n].m_textureID].m_width) / m_unitWidth;
				float imageAspectRatio = static_cast<float>(m_imageData[m_tilemapData[n].m_textureID].m_width) / static_cast<float>(m_imageData[m_tilemapData[n].m_textureID].m_height);
				m_tilemapData[n].m_transformation[0][0] = m_tilemapData[n].m_transformation[0][0] * widthRatio / imageAspectRatio;
				m_tilemapData[n].m_transformation[0][1] = m_tilemapData[n].m_transformation[0][1] * widthRatio / imageAspectRatio;
				m_tilemapData[n].m_transformation[1][1] = m_tilemapData[n].m_transformation[1][1] * heightRatio;
				m_tilemapData[n].m_transformation[1][0] = m_tilemapData[n].m_transformation[1][0] * heightRatio;
				int moveCount = 0;
				for (int i = 0; i < m_tilemapIndexArrays[n].size(); ++i)
				{
					m_tileIndexes.insert( m_tileIndexes.end(),
										  std::make_move_iterator(m_tilemapIndexArrays[n][i].begin()),
										  std::make_move_iterator(m_tilemapIndexArrays[n][i].end())
										);
				}
			}
			/*for (int x : m_tileIndexes)
			{
				std::cout << x << ' ' << std::endl;
			}*/
			m_transformedTilemaps = std::move(m_tilemapData);
			m_tilemapData.clear();
		}
	}
}