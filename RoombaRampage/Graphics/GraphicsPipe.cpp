/******************************************************************/
/*!
\file      GraphicsPipe.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      2nd Oct, 2024
\brief     This file contains the function definitions for the GraphicsPipe
		   class. The functions range from initializing the various buffers needed,
		   setting up Vertex Atttribute Objects, updating the model matrices for each model,
		   substituting new data into the buffers, preparing and linking shaders and draw
		   functions for a frame buffer object, generic draw objects and text objects.

		   It also holds the shader files used inside this file.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "../Graphics/GraphicsPipe.h"
#include "../Assets/AssetManager.h"
#include "../Application/Application.h"
#include "../Application/Helper.h"
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include "../ECS/ECSList.h"

namespace graphicpipe {


	std::unique_ptr<GraphicsPipe> GraphicsPipe::m_instancePtr = nullptr;


	void GraphicsPipe::m_funcInit()
	{
		//Enable transparency for images
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glLineWidth(4.f);

		m_squareMesh.m_shapeType = SQUARE;
		m_squareLinesMesh.m_shapeType = SQUARE_LINES;
		m_testMatrix = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

		// Reserve memory for the maximum number of entities.
		m_modelData.reserve(ecs::MaxEntity);
		m_debugBoxData.reserve(ecs::MaxEntity);
		m_textData.reserve(ecs::MaxEntity);
		m_modelToNDCMatrix.reserve(ecs::MaxEntity);
		m_debugToNDCMatrix.reserve(ecs::MaxEntity);
		m_debugBoxCollisionChecks.reserve(ecs::MaxEntity);
		m_frameNumbers.reserve(ecs::MaxEntity);
		m_stripCounts.reserve(ecs::MaxEntity);

		// Set up VAOs for different shapes and text rendering.
		m_funcSetupVao(m_squareMesh);
		m_funcSetupSquareLinesVao();
		m_funcSetupFboVao();
		m_funcSetupTextVao();
		m_funcSetDrawMode(GL_FILL);

		// Compile and set up shader programs for various rendering tasks.
		m_genericShaderProgram = m_funcSetupShader(genericVertexShader, genericFragmentShader);
		m_frameBufferShaderProgram = m_funcSetupShader(frameBufferVertexShader, frameBufferFragmentShader);
		m_debugShaderProgram = m_funcSetupShader(debugVertexShader, debugFragmentShader);
		m_textShaderProgram = m_funcSetupShader(textVertexShader, textFragmentShader);

		// Initialize model-to-NDC transformation matrix and other drawing data.
		m_modelToNDCMatrix.push_back(m_testMatrix);
		m_textureOrder.push_back(0);
		m_frameNumbers.push_back(0);
		m_stripCounts.push_back(0);
		m_debugToNDCMatrix.push_back(m_testMatrix);
		m_debugBoxCollisionChecks.push_back(false);

		m_windowWidth = static_cast<int>(Helper::Helpers::GetInstance()->m_windowWidth);
		m_windowHeight = static_cast<int>(Helper::Helpers::GetInstance()->m_windowHeight);
		m_aspectRatio = static_cast<float>(static_cast<float>(m_windowHeight) / static_cast<float>(m_windowWidth));

		// Set up array buffer and framebuffers for offscreen rendering.
		m_funcSetupArrayBuffer();
		m_funcSetupFrameBuffer();

		// Clear temporary data structures used during setup.
		m_debugToNDCMatrix.clear();
		m_debugBoxCollisionChecks.clear();
		m_modelToNDCMatrix.clear();
		m_textureOrder.clear();
		m_frameNumbers.clear();
		m_stripCounts.clear();

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
			for (int n{}; n < m_modelData.size(); n++)
			{
				float heightRatio = static_cast<float>(m_imageData[m_modelData[n].m_textureID].m_height) / m_unitHeight;
				float widthRatio = static_cast<float>(m_imageData[m_modelData[n].m_textureID].m_width) / m_unitWidth;

				float imageAspectRatio = static_cast<float>(m_imageData[m_modelData[n].m_textureID].m_width) / static_cast<float>(m_imageData[m_modelData[n].m_textureID].m_height);



				m_modelData[n].transformation[0][0] = m_modelData[n].transformation[0][0] * widthRatio / imageAspectRatio;
				m_modelData[n].transformation[0][1] = m_modelData[n].transformation[0][1] * widthRatio / imageAspectRatio;
				m_modelData[n].transformation[1][1] = m_modelData[n].transformation[1][1] * heightRatio;
				m_modelData[n].transformation[1][0] = m_modelData[n].transformation[1][0] * heightRatio;

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





				m_modelToNDCMatrix.push_back(lvNDCScale * m_modelData[n].transformation);
				m_textureOrder.push_back(m_modelData[n].m_textureID);
				m_stripCounts.push_back(m_imageData[m_modelData[n].m_textureID].m_stripCount);
				m_frameNumbers.push_back(m_modelData[n].frameNumber);
			}
		}

		if (m_debugBoxData.size() > 0)
		{
			for (int i{}; i < m_debugBoxData.size(); i++)
			{
				glm::mat3 lvScale{ m_debugBoxData[i].m_scale.x, 0, 0, 0, m_debugBoxData[i].m_scale.y, 0, 0 , 0 ,1 };
				glm::mat3 lvRotate{ cos(m_debugBoxData[i].m_rotate * 3.1415f / 180.f), -sin(m_debugBoxData[i].m_rotate * 3.1415f / 180.f), 0.f,
									sin(m_debugBoxData[i].m_rotate * 3.1415f / 180.f), cos(m_debugBoxData[i].m_rotate * 3.1415f / 180.f), 0.f,
									0.f , 0.f ,1.f };
				glm::mat3 lvTranslate{ 1, 0, 0, 0, 1, 0, m_debugBoxData[i].m_worldCoordinates.x , m_debugBoxData[i].m_worldCoordinates.y ,1 };
				glm::mat3 lvNDCScale{ m_aspectRatio, 0, 0, 0, 1.f, 0, 0 , 0 ,1.f };
				m_debugToNDCMatrix.push_back(lvNDCScale * lvTranslate * lvRotate * lvScale);
				m_debugBoxCollisionChecks.push_back(static_cast<float>(m_debugBoxData[i].m_isCollided));

			}
		}

	}

}
	

