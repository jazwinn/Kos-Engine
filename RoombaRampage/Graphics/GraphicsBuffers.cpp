#include "../Graphics/GraphicsPipe.h"
#include "../Application/Helper.h"

namespace graphicpipe
{
	void GraphicsPipe::m_funcSetupArrayBuffer()
	{
		//Square Mesh Buffer Setup
		glBindVertexArray(m_squareMesh.m_vaoId);
		glGenBuffers(1, &m_modelMatrixArrayBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_modelMatrixArrayBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_modelToNDCMatrix.size() * sizeof(glm::mat3), &m_modelToNDCMatrix[0], GL_DYNAMIC_DRAW);
		glBindVertexArray(m_squareMesh.m_vaoId);
		unsigned int location = 6;
		for (int i = 0; i < 3; ++i)
		{
			glEnableVertexAttribArray(location + i);
			glVertexAttribPointer(location + i, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), (void*)(sizeof(glm::vec3) * i));
			glVertexAttribDivisor(location + i, 1);
		}
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			std::cout << "First OpenGL Error: " << err << std::endl;
		}

		glBindVertexArray(m_squareMesh.m_vaoId);
		glGenBuffers(1, &m_textureOrderBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_textureOrderBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_textureOrder.size() * sizeof(int), &m_textureOrder[0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(5);
		glVertexAttribIPointer(5, 1, GL_INT, sizeof(int), (void*)0);
		glVertexAttribDivisor(5, 1);
		glGenBuffers(1, &m_stripCountBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_stripCountBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_stripCounts.size() * sizeof(int), &m_stripCounts[0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(4);
		glVertexAttribIPointer(4, 1, GL_INT, sizeof(int), (void*)0);
		glVertexAttribDivisor(4, 1);
		glGenBuffers(1, &m_frameNumberBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_frameNumberBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_frameNumbers.size() * sizeof(int), &m_frameNumbers[0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(3);
		glVertexAttribIPointer(3, 1, GL_INT, sizeof(int), (void*)0);
		glVertexAttribDivisor(3, 1);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);



		//Square Lines Mesh Buffer Setup
		glBindVertexArray(m_squareLinesMesh.m_vaoId);
		glGenBuffers(1, &m_debugMatrixArrayBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_debugMatrixArrayBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_debugBoxToNDCMatrix.size() * sizeof(glm::mat3), &m_debugBoxToNDCMatrix[0], GL_DYNAMIC_DRAW);
		unsigned int otherLocation = 7; // Location 7
		for (int i = 0; i < 3; ++i)
		{
			glEnableVertexAttribArray(otherLocation + i);
			glVertexAttribPointer(otherLocation + i, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), (void*)(sizeof(glm::vec3) * i));
			glVertexAttribDivisor(otherLocation + i, 1);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &m_debugCollisionCheckBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_debugCollisionCheckBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_debugBoxCollisionChecks.size() * sizeof(int), &m_debugBoxCollisionChecks[0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
		glVertexAttribDivisor(4, 1);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Circle Lines
		glBindVertexArray(m_circleLinesMesh.m_vaoId);
		glBindBuffer(GL_ARRAY_BUFFER, m_debugMatrixArrayBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_debugBoxToNDCMatrix.size() * sizeof(glm::mat3), &m_debugBoxToNDCMatrix[0], GL_DYNAMIC_DRAW);
		for (int i = 0; i < 3; ++i)
		{
			glEnableVertexAttribArray(otherLocation + i);
			glVertexAttribPointer(otherLocation + i, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), (void*)(sizeof(glm::vec3) * i));
			glVertexAttribDivisor(otherLocation + i, 1);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, m_debugCollisionCheckBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_debugBoxCollisionChecks.size() * sizeof(int), &m_debugBoxCollisionChecks[0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
		glVertexAttribDivisor(4, 1);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		err = glGetError();
		if (err != GL_NO_ERROR) {
			std::cout << "Third OpenGL Error: " << err << std::endl;
		}

	}

	void GraphicsPipe::m_funcSetupFrameBuffer()
	{
		Helper::Helpers* help = Helper::Helpers::GetInstance();

		if (m_screenTexture)
		{
			glDeleteTextures(1, &m_screenTexture);
		}
		if (m_depthBufferObject)
		{
			glDeleteRenderbuffers(1, &m_depthBufferObject);
		}
		if (m_frameBufferObject)
		{
			glDeleteFramebuffers(1, &m_frameBufferObject);
		}
		glGenFramebuffers(1, &m_frameBufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);

		glGenTextures(1, &m_screenTexture);
		glBindTexture(GL_TEXTURE_2D, m_screenTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, help->m_windowWidth, help->m_windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_screenTexture, 0);


		glGenRenderbuffers(1, &m_depthBufferObject);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferObject);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, help->m_windowWidth, help->m_windowHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferObject);

#if DEBUG
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
			LOGGING_INFO("Framebuffer successfully created");
		}
		else
		{
			LOGGING_INFO("Framebuffer has not been created");
		}
#endif
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glScissor(0, 0, help->m_windowWidth, help->m_windowHeight);

	}

	void GraphicsPipe::m_funcSetupGamePreviewFrameBuffer()
	{
		Helper::Helpers* help = Helper::Helpers::GetInstance();

		if (m_gamePreviewTexture)
		{
			glDeleteTextures(1, &m_gamePreviewTexture);
		}
		if (m_depthBufferObject)
		{
			glDeleteRenderbuffers(1, &m_gamePreviewDepthBufferObject);
		}
		if (m_gamePreviewFrameBufferObject)
		{
			glDeleteFramebuffers(1, &m_gamePreviewFrameBufferObject);
		}
		glGenFramebuffers(1, &m_gamePreviewFrameBufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, m_gamePreviewFrameBufferObject);

		glGenTextures(1, &m_gamePreviewTexture);
		glBindTexture(GL_TEXTURE_2D, m_gamePreviewTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, help->m_windowWidth, help->m_windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gamePreviewTexture, 0);


		glGenRenderbuffers(1, &m_gamePreviewDepthBufferObject);
		glBindRenderbuffer(GL_RENDERBUFFER, m_gamePreviewDepthBufferObject);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, help->m_windowWidth, help->m_windowHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_gamePreviewDepthBufferObject);

#if DEBUG
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
			LOGGING_INFO("Framebuffer successfully created");
		}
		else
		{
			LOGGING_INFO("Framebuffer has not been created");
		}
#endif
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glScissor(0, 0, help->m_windowWidth, help->m_windowHeight);

	}
}