/******************************************************************/
/*!
\file      GraphicsFrameBufferUsage.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      17th Jan, 2025
\brief     

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#include "../Config/pch.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Graphics/GraphicsCamera.h"
#include "../Application/Helper.h"

namespace graphicpipe
{
	void GraphicsPipe::m_funcDrawGameFrameBuffer()
	{
		Helper::Helpers* help = Helper::Helpers::GetInstance();
		//glClearColor(help->m_colour.m_x, help->m_colour.m_y, help->m_colour.m_z, 1.f);
		glClearColor(static_cast<GLclampf>(help->m_colour.m_x * m_globalLightIntensity), static_cast<GLclampf>(help->m_colour.m_y * m_globalLightIntensity), static_cast<GLclampf>(help->m_colour.m_z * m_globalLightIntensity), static_cast<GLclampf>(1));
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_funcDrawTilemap();
		m_funcDraw();

		// Switch back to the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}

	void GraphicsPipe::m_funcDrawWindow()
	{
		Helper::Helpers* help = Helper::Helpers::GetInstance();
		//glClearColor(help->m_colour.m_x, help->m_colour.m_y, help->m_colour.m_z, 1.f);
		glClearColor(static_cast<GLclampf>(help->m_colour.m_x * m_globalLightIntensity), static_cast<GLclampf>(help->m_colour.m_y * m_globalLightIntensity), static_cast<GLclampf>(help->m_colour.m_z * m_globalLightIntensity), static_cast<GLclampf>(1));
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_funcDrawTilemap();
		m_funcDraw();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

	}

	void GraphicsPipe::m_drawLightingTexture()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glBindFramebuffer(GL_FRAMEBUFFER, m_lightingFrameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_funcRenderLighting();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GraphicsPipe::m_funcDrawUnlitObjectTexture()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glBindFramebuffer(GL_FRAMEBUFFER, m_unlitScreenFrameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_funcDrawUnlit();
		m_funcDrawText();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

		//Helper::Helpers* help = Helper::Helpers::GetInstance();
		//glClearColor(static_cast<GLclampf>(help->m_colour.m_x * m_globalLightIntensity), static_cast<GLclampf>(help->m_colour.m_y * m_globalLightIntensity), static_cast<GLclampf>(help->m_colour.m_z * m_globalLightIntensity), static_cast<GLclampf>(1));
		//glClearColor(help->m_colour.m_x, help->m_colour.m_y, help->m_colour.m_z, 1.f);
		glClearColor(0, 0, 0, 1.f);

	}

	void GraphicsPipe::m_funcDrawDebugTexture()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glBindFramebuffer(GL_FRAMEBUFFER, m_unlitScreenFrameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_funcDrawUnlit();
		m_funcDrawText();
		m_funcDrawDebug();
		m_funcDrawGrid();
		m_funcDrawGridCollider();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);
		//Helper::Helpers* help = Helper::Helpers::GetInstance();
		//glClearColor(help->m_colour.m_x, help->m_colour.m_y, help->m_colour.m_z, 1.f);
		//glClearColor(0, 0, 0, 1.f);
	}

	void GraphicsPipe::m_renderFinalPass()
	{

		Helper::Helpers* help = Helper::Helpers::GetInstance();
		glViewport(0, 0, (int)help->m_windowWidth, (int)help->m_windowHeight);
		m_funcDrawGameFrameBuffer();
		m_drawLightingTexture();
		m_funcDrawUnlitObjectTexture();
		glClearColor(0, 0, 0, 1.f);

		glBindFramebuffer(GL_FRAMEBUFFER, m_finalPassFrameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*-----------------------------------------------------------------------------*/
		glUseProgram(m_finalPassShaderProgram);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_screenTexture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_lightingTexture);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_unlitScreenTexture);

		glUniform1i(glGetUniformLocation(m_finalPassShaderProgram, "unlitScreenTexture"), 2);
		glUniform1i(glGetUniformLocation(m_finalPassShaderProgram, "lightTexture"), 1);
		glUniform1i(glGetUniformLocation(m_finalPassShaderProgram, "screenTexture"), 0);
		glUniform1f(glGetUniformLocation(m_finalPassShaderProgram, "globalBrightness"), m_globalLightIntensity);

		glBindVertexArray(m_screenTextureVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

		/*------------------------------------------------------------------------------*/


		glViewport(help->m_viewportOffsetX, help->m_viewportOffsetY, (int)help->m_windowWidth, (int)help->m_windowHeight);
		glUseProgram(m_frameBufferShaderProgram);
		glBindVertexArray(m_screenTextureVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_finalPassTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		
		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			//LOGGING_ERROR("First OpenGL Error: 0x%X", err);
			std::cout << "Second Final Pass OpenGL Error: " << err << std::endl;
		}
	}

	void GraphicsPipe::m_renderFinalPassWithDebug()
	{

		Helper::Helpers* help = Helper::Helpers::GetInstance();
		glViewport(0, 0, (int)help->m_windowWidth, (int)help->m_windowHeight);
		m_funcDrawWindow();
		m_drawLightingTexture();
		m_funcDrawDebugTexture();
		glBindFramebuffer(GL_FRAMEBUFFER, m_gamePreviewFrameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*-----------------------------------------------------------------------------*/
		glUseProgram(m_finalPassShaderProgram);

		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			//LOGGING_ERROR("First OpenGL Error: 0x%X", err);
			std::cout << "First Final Pass OpenGL Error: " << err << std::endl;
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_screenTexture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_lightingTexture);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_unlitScreenTexture);

		glUniform1i(glGetUniformLocation(m_finalPassShaderProgram, "unlitScreenTexture"), 2);
		glUniform1i(glGetUniformLocation(m_finalPassShaderProgram, "lightTexture"), 1);
		glUniform1i(glGetUniformLocation(m_finalPassShaderProgram, "screenTexture"), 0);
		glUniform1f(glGetUniformLocation(m_finalPassShaderProgram, "globalBrightness"), m_globalLightIntensity);

		glBindVertexArray(m_screenTextureVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		/*------------------------------------------------------------------------------*/

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

		glViewport(help->m_viewportOffsetX, help->m_viewportOffsetY, (int)help->m_windowWidth, (int)help->m_windowHeight);

		//err = glGetError();
		//if (err != GL_NO_ERROR) {
		//	//LOGGING_ERROR("First OpenGL Error: 0x%X", err);
		//	std::cout << "Second Final Pass OpenGL Error: " << err << std::endl;
		//}
	}

	void GraphicsPipe::m_funcDrawGamePreviewWindow()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_gamePreviewFrameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_funcDrawTilemap();
		m_funcDraw();
		m_funcDrawText();
		m_funcRenderLighting();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

	}
}