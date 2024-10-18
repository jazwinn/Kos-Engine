#include "../Graphics/GraphicsPipe.h"
#include "../Graphics/GraphicsCamera.h"


namespace graphicpipe
{
	void GraphicsPipe::m_funcDraw()
	{
		if (!m_modelToNDCMatrix.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_modelMatrixArrayBuffer);
			glNamedBufferData(m_modelMatrixArrayBuffer, m_modelToNDCMatrix.size() * sizeof(glm::mat3), &m_modelToNDCMatrix[0], GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, m_textureOrderBuffer);
			glNamedBufferData(m_textureOrderBuffer, m_textureOrder.size() * sizeof(int), &m_textureOrder[0], GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, m_stripCountBuffer);
			glNamedBufferData(m_stripCountBuffer, m_stripCounts.size() * sizeof(int), &m_stripCounts[0], GL_DYNAMIC_DRAW);


			glBindBuffer(GL_ARRAY_BUFFER, m_frameNumberBuffer);
			glNamedBufferData(m_frameNumberBuffer, m_frameNumbers.size() * sizeof(int), &m_frameNumbers[0], GL_DYNAMIC_DRAW);


			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glUseProgram(m_genericShaderProgram);

			for (int i = 0; i < m_textureIDs.size(); ++i)
			{
				glActiveTexture(GL_TEXTURE1 + i);
				glBindTexture(GL_TEXTURE_2D, m_textureIDs[i]);
			}

			GLint lvUniformVarLoc1 = glGetUniformLocation(m_genericShaderProgram, "textures");

			if (lvUniformVarLoc1 >= 0)
			{
				glUniform1iv(lvUniformVarLoc1, static_cast<GLsizei>(m_textureIDs.size()), (GLint*)&m_textureIDs[0]);
			}
			else
			{
				std::cout << lvUniformVarLoc1 << std::endl;
				std::cout << "Uniform variable doesn't exist!!!\n";
				std::exit(EXIT_FAILURE);
			}

			glBindVertexArray(m_squareMesh.m_vaoId);
			glDrawElementsInstanced(m_squareMesh.m_primitiveType, m_squareMesh.m_indexElementCount, GL_UNSIGNED_SHORT, NULL, static_cast<GLsizei>(m_modelToNDCMatrix.size()));
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			m_modelToNDCMatrix.clear();
			m_textureOrder.clear();
			m_frameNumbers.clear();
			m_stripCounts.clear();
		}
		if (!m_modelData.empty())
		{
			m_modelData.clear();
		}

	}

	void GraphicsPipe::m_funcDrawDebug()
	{
		if (!m_debugToNDCMatrix.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_debugCollisionCheckBuffer);
			glNamedBufferData(m_debugCollisionCheckBuffer, m_debugBoxCollisionChecks.size() * sizeof(float), &m_debugBoxCollisionChecks[0], GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glUseProgram(m_debugShaderProgram);

			glBindBuffer(GL_ARRAY_BUFFER, m_debugMatrixArrayBuffer);
			glNamedBufferData(m_debugMatrixArrayBuffer, m_debugToNDCMatrix.size() * sizeof(glm::mat3), &m_debugToNDCMatrix[0], GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(m_squareLinesMesh.m_vaoId);
			glDrawElementsInstanced(m_squareLinesMesh.m_primitiveType, m_squareLinesMesh.m_indexElementCount, GL_UNSIGNED_SHORT, NULL, static_cast<GLsizei>(m_debugToNDCMatrix.size()));
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			m_debugToNDCMatrix.clear();
			m_debugBoxCollisionChecks.clear();
		}
		if (!m_debugBoxData.empty())
		{
			m_debugBoxData.clear();
		}
	}

	void GraphicsPipe::m_funcDrawWindow()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_funcDrawDebug();
		m_funcDraw();
		m_funcDrawText();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(m_frameBufferShaderProgram);

		glBindVertexArray(m_screenMesh.m_vaoId);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_screenTexture);
		int loc = glGetUniformLocation(m_frameBufferShaderProgram, "screenTexture");

		if (loc != -1)
		{
			glUniform1i(loc, 0);
		}
		else
		{
			std::cout << "Uniform not found" << std::endl;
		}
	}

	void GraphicsPipe::m_funcDrawText()
	{

		if (!m_textData.empty()) {

			for (auto& textData : m_textData) {
				// activate corresponding render state	
				glUseProgram(m_textShaderProgram);
				glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(GraphicsCamera::m_windowWidth), 0.0f, static_cast<float>(GraphicsCamera::m_windowHeight));
				glUniformMatrix4fv(glGetUniformLocation(m_textShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

				//int loc = glGetUniformLocation(m_textShaderProgram, "projection");

				glUniform3f(glGetUniformLocation(m_textShaderProgram, "textColor"), textData.m_color.x, textData.m_color.y, textData.m_color.z);
				glActiveTexture(GL_TEXTURE0);
				glBindVertexArray(m_textMesh.m_vaoId);

				// iterate through all characters
				std::string::const_iterator c;
				assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
				for (c = textData.m_text.begin(); c != textData.m_text.end(); c++)
				{
					text::CharacterData ch = assetmanager->m_characters[*c];

					float xpos = textData.m_x + ch.m_bearing.x * textData.m_scale;
					float ypos = textData.m_y - (ch.m_size.y - ch.m_bearing.y) * textData.m_scale;

					float w = ch.m_size.x * textData.m_scale;
					float h = ch.m_size.y * textData.m_scale;
					// update VBO for each character
					float vertices[6][4] = {
						{ xpos,     ypos + h,   0.0f, 0.0f },
						{ xpos,     ypos,       0.0f, 1.0f },
						{ xpos + w, ypos,       1.0f, 1.0f },

						{ xpos,     ypos + h,   0.0f, 0.0f },
						{ xpos + w, ypos,       1.0f, 1.0f },
						{ xpos + w, ypos + h,   1.0f, 0.0f }
					};
					// render glyph texture over quad
					glBindTexture(GL_TEXTURE_2D, ch.m_textureID);

					// update content of VBO memory
					glBindBuffer(GL_ARRAY_BUFFER, m_textBuffer);
					glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					// render quad
					glDrawArrays(GL_TRIANGLES, 0, 6);


					// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
					textData.m_x += (ch.m_advance >> 6) * textData.m_scale; // bitshift by 6 to get value in pixels (2^6 = 64)
				}
				glBindVertexArray(0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			if (!m_textData.empty())
			{
				m_textData.clear();
			}
		}
	}

	void GraphicsPipe::m_funcSetDrawMode(GLenum mode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, mode);
	}

}


