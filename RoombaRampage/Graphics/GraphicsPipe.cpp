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
#include "../Graphics/GraphicsCamera.h"
#include "../Asset Manager/AssetManager.h"
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
		m_funcCalculateModelToWorldMatrix();
		GraphicsCamera::calculateAspectRatio();
		
		if (GraphicsCamera::m_cameras.size() > 0 && m_gameMode)
		{
			GraphicsCamera::setCurrCamera(0);
			GraphicsCamera::calculateCurrCamera();
			GraphicsCamera::calculateCurrView();
		}
		//GraphicsCamera::multiplyActiveCameraMatrix();
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
		m_modelMatrix.clear();
		m_modelData.clear();
		m_debugToNDCMatrix.clear();
		m_debugBoxCollisionChecks.clear();
		m_debugBoxData.clear();
		
	}

	void GraphicsPipe::m_funcRenderGameScene()
	{
		if (m_gameMode)
		{
			m_funcDraw();
			m_funcDrawText();
		}

		m_funcClearContainers();
	}

}
	

