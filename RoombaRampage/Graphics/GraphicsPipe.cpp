/******************************************************************/
/*!
\file      GraphicsPipe.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th Nov, 2024
\brief     This file implements the `GraphicsPipe` class functions for setting up and managing
		   the rendering pipeline, including initializing shaders, buffers, and rendering models
		   and debug geometries.

\details   The `GraphicsPipe` class provides functionality for initializing rendering settings,
		   setting up VAOs (Vertex Array Objects), and compiling shader programs for various
		   tasks like drawing models, debug shapes, and text. It also handles the transformation
		   of models from world space to normalized device coordinates (NDC) and facilitates the
		   offscreen rendering through framebuffers. Additionally, the class manages the current
		   camera state and updates transformations as per the game or editor mode.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "../Config/pch.h"
#include "../Graphics/GraphicsPipe.h"
<<<<<<< HEAD
#include "../Graphics/GraphicsCamera.h"
#include "../Asset Manager/AssetManager.h"
=======
#include "../Assets/AssetManager.h"
>>>>>>> 2614f36e3dde51625ed71ac1889d9f61bb456128
#include "../Application/Application.h"
#include "../Application/Helper.h"
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include "../ECS/ECSList.h"
#include "../Editor/EditorCamera.h"

namespace graphicpipe {


	std::unique_ptr<GraphicsPipe> GraphicsPipe::m_instancePtr = nullptr;


	void GraphicsPipe::m_funcInit()
	{
		//Enable transparency for images
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glLineWidth(4.f);
		m_squareLinesMesh.m_shapeType = SQUARE_LINES;
		m_testMatrix = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

		// Reserve memory for the maximum number of entities.
		m_modelData.reserve(ecs::MaxEntity);
		m_debugBoxData.reserve(ecs::MaxEntity);
		m_textData.reserve(ecs::MaxEntity);
		m_modelToNDCMatrix.reserve(ecs::MaxEntity);
		m_debugBoxToNDCMatrix.reserve(ecs::MaxEntity);
		m_debugBoxCollisionChecks.reserve(ecs::MaxEntity);
		m_frameNumbers.reserve(ecs::MaxEntity);
		m_stripCounts.reserve(ecs::MaxEntity);
		m_layers.reserve(ecs::MaxEntity);

		// Set up VAOs for different shapes and text rendering.
		m_funcSetupVao(m_squareMesh);
		m_funcSetupCircleLinesVao();
		m_funcSetupSquareLinesVao();
		m_funcSetupGridVao();
		m_funcSetupTextVao();
		m_funcSetDrawMode(GL_FILL);

		// Compile and set up shader programs for various rendering tasks.
		m_genericShaderProgram = m_funcSetupShader(genericVertexShader, genericFragmentShader);
		m_frameBufferShaderProgram = m_funcSetupShader(frameBufferVertexShader, frameBufferFragmentShader);
		m_debugShaderProgram = m_funcSetupShader(debugVertexShader, debugFragmentShader);
		m_textShaderProgram = m_funcSetupShader(textVertexShader, textFragmentShader);
		m_gridShaderProgram = m_funcSetupShader(gridVertexShader, gridFragmentShader);
		m_tilemapShaderProgram = m_funcSetupShader(tilemapVertexShader, tilemapFragmentShader);

		// Initialize model-to-NDC transformation matrix and other drawing data.
		m_modelToNDCMatrix.push_back(m_testMatrix);
		m_textureOrder.push_back(0);
		m_frameNumbers.push_back(0);
		m_stripCounts.push_back({ 0,0 });
		m_layers.push_back(0);
		m_tileIndexes.push_back({0});
		m_colors.push_back({ 0.f, 0.f, 0.f, 0.f });
		m_debugBoxToNDCMatrix.push_back(m_testMatrix);
		m_debugBoxCollisionChecks.push_back(false);

		m_windowWidth = static_cast<int>(Helper::Helpers::GetInstance()->m_windowWidth);
		m_windowHeight = static_cast<int>(Helper::Helpers::GetInstance()->m_windowHeight);
		m_aspectRatio = static_cast<float>(static_cast<float>(m_windowHeight) / static_cast<float>(m_windowWidth));

		// Set up array buffer and framebuffers for offscreen rendering.
		m_funcSetupArrayBuffer();
		m_funcSetupFrameBuffer();
		m_funcSetupGamePreviewFrameBuffer();

		// Clear temporary data structures used during setup.
		m_debugBoxToNDCMatrix.clear();
		m_debugBoxCollisionChecks.clear();
		m_modelToNDCMatrix.clear();
		m_textureOrder.clear();
		m_frameNumbers.clear();
		m_stripCounts.clear();
		m_layers.clear();
		m_colors.clear();
		m_tileIndexes.clear();

		// Enable scissor test for limiting rendering to a specific area.
		glEnable(GL_SCISSOR_TEST);

	}

	GraphicsPipe::~GraphicsPipe()
	{
		//delete instancePtr;
		if (m_frameBufferObject)
		{
			glDeleteFramebuffers(1, &m_frameBufferObject);
		}
		m_funcDeleteShader();
	}

	GraphicsPipe* GraphicsPipe::m_funcGetInstance()
	{
		if (!m_instancePtr)
		{
			m_instancePtr.reset(new GraphicsPipe{});
		}
		return m_instancePtr.get();
	}

	void GraphicsPipe::m_funcUpdate()
	{
		m_windowWidth = static_cast<int>(Helper::Helpers::GetInstance()->m_windowWidth);
		m_windowHeight = static_cast<int>(Helper::Helpers::GetInstance()->m_windowHeight);
		m_aspectRatio = static_cast<float>(static_cast<float>(m_windowHeight) / static_cast<float>(m_windowWidth));
		if (m_modelData.size() > 0)
		{
<<<<<<< HEAD
			GraphicsCamera::setCurrCamera(0);
			
			GraphicsCamera::calculateCurrView();
		}
		GraphicsCamera::multiplyViewMatrix();
		GraphicsCamera::multiplyOrthoMatrix();

		if (!m_gameMode)
		{
			m_funcDrawWindow();
		}
		
	}

	void GraphicsPipe::m_funcClearContainers()
	{
		m_modelToNDCMatrix.clear();
		m_textureOrder.clear();
		m_frameNumbers.clear();
		m_stripCounts.clear();
		m_layers.clear();
		m_modelMatrix.clear();
		m_modelData.clear();
		m_debugBoxToNDCMatrix.clear();
		m_debugBoxCollisionChecks.clear();
		m_debugCircleToNDCMatrix.clear();
		m_debugCircleCollisionChecks.clear();
		m_debugBoxData.clear();
		m_textData.clear();
		m_colors.clear();
		m_tilemapData.clear();
		m_transformedTilemaps.clear();
		m_tileIndexes.clear();
		m_tilemapIndexArrays.clear();
		GraphicsCamera::m_cameras.clear();
		
	}

	void GraphicsPipe::m_funcRenderGameScene()
	{

		if (m_gameMode)
		{
			Helper::Helpers* help = Helper::Helpers::GetInstance();
			/*if (GraphicsCamera::m_cameras.size() > 0 && m_gameMode)
			{
				GraphicsCamera::setCurrCamera(0);
				GraphicsCamera::calculateCurrView();
			}
			GraphicsCamera::setCurrCamera(0);
			GraphicsCamera::calculateCurrView();*/
			glClearColor(0.86f, 0.86f, 0.86f, 1.f);
			glEnable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			m_funcDraw();
			m_funcDrawTilemap();
			m_funcDrawText();
		}

		m_funcClearContainers();
=======
			for (int n{}; n < m_modelData.size(); n++)
			{
				float heightRatio = static_cast<float>(m_imageData[m_modelData[n].m_textureID].m_height) / m_unitHeight;
				float widthRatio = static_cast<float>(m_imageData[m_modelData[n].m_textureID].m_width) / m_unitWidth;

				float imageAspectRatio = static_cast<float>(m_imageData[m_modelData[n].m_textureID].m_width) / static_cast<float>(m_imageData[m_modelData[n].m_textureID].m_height);



				m_modelData[n].m_transformation[0][0] = m_modelData[n].m_transformation[0][0] * widthRatio / imageAspectRatio;
				m_modelData[n].m_transformation[0][1] = m_modelData[n].m_transformation[0][1] * widthRatio / imageAspectRatio;
				m_modelData[n].m_transformation[1][1] = m_modelData[n].m_transformation[1][1] * heightRatio;
				m_modelData[n].m_transformation[1][0] = m_modelData[n].m_transformation[1][0] * heightRatio;

				/*glm::mat3 lvScale{ m_modelData[n].m_scale.x * widthRatio / imageAspectRatio, 0, 0, 0, m_modelData[n].m_scale.y * heightRatio , 0, 0 , 0 ,1 };
				glm::mat3 lvRotate{ cos(m_modelData[n].m_rotate * 3.1415f / 180.f), -sin(m_modelData[n].m_rotate * 3.1415f / 180.f), 0.f,
								   sin(m_modelData[n].m_rotate * 3.1415f / 180.f), cos(m_modelData[n].m_rotate * 3.1415f / 180.f), 0.f,
									0.f , 0.f ,1.f };
				glm::mat3 lvTranslate{ 1, 0, 0, 0, 1, 0, m_modelData[n].m_worldCoordinates.x , m_modelData[n].m_worldCoordinates.y ,1 };*/

				glm::mat3 lvNDCScale{ m_aspectRatio, 0, 0, 0, 1.f, 0, 0 , 0 ,1.f };

				//glm::mat3 ortho = glm::mat3(1.0f);

				//// Scale X and Y by aspect ratio
				//float left = -0.5f;
				//float right = 0.5f;
				//float bottom = -0.5f;
				//float top = 0.5f;

				//ortho[0][0] = 2.0f / (right - left);  // Scale X
				//ortho[1][1] = 2.0f / (top - bottom);  // Scale Y

				//ortho[2][0] = -(right + left) / (right - left);  // Translate X
				//ortho[2][1] = -(top + bottom) / (top - bottom);  // Translate Y





				m_modelToNDCMatrix.push_back(lvNDCScale * m_modelData[n].m_transformation);
				m_textureOrder.push_back(m_modelData[n].m_textureID);
				m_stripCounts.push_back(m_imageData[m_modelData[n].m_textureID].m_stripCount);
				m_frameNumbers.push_back(m_modelData[n].m_frameNumber);
			}
		}

		if (m_debugBoxData.size() > 0)
		{
			for (int i{}; i < m_debugBoxData.size(); i++)
			{
				glm::mat3 lvNDCScale{ m_aspectRatio, 0, 0, 0, 1.f, 0, 0 , 0 ,1.f };
				m_debugToNDCMatrix.push_back(lvNDCScale * m_debugBoxData[i].m_transformation);
				m_debugBoxCollisionChecks.push_back(static_cast<float>(m_debugBoxData[i].m_isCollided));

			}
		}

>>>>>>> 2614f36e3dde51625ed71ac1889d9f61bb456128
	}

}
	

