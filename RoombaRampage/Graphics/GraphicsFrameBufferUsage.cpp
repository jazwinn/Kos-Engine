#include "../Config/pch.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Graphics/GraphicsCamera.h"

namespace graphicpipe
{
	void GraphicsPipe::m_funcDrawGameFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_funcDrawTilemap();
		m_funcDraw();
		m_funcDrawText();
		//m_funcRenderLighting();

		// Switch back to the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GraphicsPipe::m_funcDrawWindow()
	{

		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_funcDrawGrid();
		m_funcDrawDebug();
		m_funcDrawTilemap();
		m_funcDrawGridCollider();
		m_funcDraw();
		//m_funcDrawLine({ 1.f,1.f,0 }, { -1.f,-1.f,0 }); // Comment this out when done debugging;
		m_funcDrawText();
		//m_funcRenderLighting();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

	}

	void GraphicsPipe::m_drawLightingTexture()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_lightingFrameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_funcRenderLighting();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GraphicsPipe::m_renderFinalPass()
	{
		m_funcDrawGameFrameBuffer();
		m_drawLightingTexture();
		glBindFramebuffer(GL_FRAMEBUFFER, m_finalPassFrameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*-----------------------------------------------------------------------------*/
		glUseProgram(m_finalPassShaderProgram);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_screenTexture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_lightingTexture);

		glUniform1i(glGetUniformLocation(m_finalPassShaderProgram, "lightTexture"), 1);
		glUniform1i(glGetUniformLocation(m_finalPassShaderProgram, "screenTexture"), 0);
		glUniform1f(glGetUniformLocation(m_finalPassShaderProgram, "globalBrightness"), m_globalLightIntensity);

		glBindVertexArray(m_screenTextureVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

		/*------------------------------------------------------------------------------*/

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
		m_funcDrawWindow();
		m_drawLightingTexture();
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

		glUniform1i(glGetUniformLocation(m_finalPassShaderProgram, "lightTexture"), 1);
		glUniform1i(glGetUniformLocation(m_finalPassShaderProgram, "screenTexture"), 0);
		glUniform1f(glGetUniformLocation(m_finalPassShaderProgram, "globalBrightness"), m_globalLightIntensity);

		glBindVertexArray(m_screenTextureVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		/*------------------------------------------------------------------------------*/

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

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