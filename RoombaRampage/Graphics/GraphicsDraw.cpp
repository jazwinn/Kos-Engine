#include "../Config/pch.h"
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
			glNamedBufferData(m_stripCountBuffer, m_stripCounts.size() * sizeof(glm::ivec2), &m_stripCounts[0], GL_DYNAMIC_DRAW);

		/*	glBindBuffer(GL_ARRAY_BUFFER, m_frameNumberBuffer);
			glNamedBufferData(m_frameNumberBuffer, m_frameNumbers.size() * sizeof(int), &m_frameNumbers[0], GL_DYNAMIC_DRAW);*/
			glBindBuffer(GL_ARRAY_BUFFER, m_layerBuffer);
			glNamedBufferData(m_layerBuffer, m_layers.size() * sizeof(int), &m_layers[0], GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
			glNamedBufferData(m_colorBuffer, m_colors.size() * sizeof(glm::vec4), &m_colors[0], GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, m_layerBuffer);




			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glUseProgram(m_genericShaderProgram);

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

			for (int i = 0; i < m_textureIDs.size(); ++i)
			{
				glActiveTexture(GL_TEXTURE0 + m_textureIDs[i]);
				glBindTexture(GL_TEXTURE_2D, m_textureIDs[i]);
			
			}
			

			glBindVertexArray(m_squareMesh.m_vaoId);
			glDrawElementsInstanced(m_squareMesh.m_primitiveType, m_squareMesh.m_indexElementCount, GL_UNSIGNED_SHORT, NULL, static_cast<GLsizei>(m_modelToNDCMatrix.size()));
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

		
		
		}
	}

	void GraphicsPipe::m_funcDrawDebug()
	{
		if (!m_debugBoxToNDCMatrix.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_debugCollisionCheckBuffer);
			glNamedBufferData(m_debugCollisionCheckBuffer, m_debugBoxCollisionChecks.size() * sizeof(float), &m_debugBoxCollisionChecks[0], GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glUseProgram(m_debugShaderProgram);

			glBindBuffer(GL_ARRAY_BUFFER, m_debugMatrixArrayBuffer);
			glNamedBufferData(m_debugMatrixArrayBuffer, m_debugBoxToNDCMatrix.size() * sizeof(glm::mat3), &m_debugBoxToNDCMatrix[0], GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(m_squareLinesMesh.m_vaoId);
			glDrawElementsInstanced(m_squareLinesMesh.m_primitiveType, m_squareLinesMesh.m_indexElementCount, GL_UNSIGNED_SHORT, NULL, static_cast<GLsizei>(m_debugBoxToNDCMatrix.size()));
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			
		}
		if (!m_debugCircleToNDCMatrix.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_debugCollisionCheckBuffer);
			glNamedBufferData(m_debugCollisionCheckBuffer, m_debugCircleCollisionChecks.size() * sizeof(float), &m_debugCircleCollisionChecks[0], GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glUseProgram(m_debugShaderProgram);

			glBindBuffer(GL_ARRAY_BUFFER, m_debugMatrixArrayBuffer);
			glNamedBufferData(m_debugMatrixArrayBuffer, m_debugCircleToNDCMatrix.size() * sizeof(glm::mat3), &m_debugCircleToNDCMatrix[0], GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(m_circleLinesMesh.m_vaoId);
			glDrawElementsInstanced(m_circleLinesMesh.m_primitiveType, m_circleLinesMesh.m_indexElementCount, GL_UNSIGNED_SHORT, NULL, static_cast<GLsizei>(m_debugCircleToNDCMatrix.size()));
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}

	void GraphicsPipe::m_funcDrawWindow()
	{
		
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_funcDrawGrid();
		m_funcDrawDebug();
		m_funcDraw();
		m_funcDrawText();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(m_frameBufferShaderProgram);

	}

	void GraphicsPipe::m_funcDrawGamePreviewWindow()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_gamePreviewFrameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_funcDraw();
		m_funcDrawText();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(m_frameBufferShaderProgram);

	}

	void GraphicsPipe::m_funcDrawText()
	{

		if (!m_textData.empty()) {
			for (auto& textData : m_textData) {
				// Activate corresponding render state
				glUseProgram(m_textShaderProgram);

				glUniform1i(glGetUniformLocation(m_textShaderProgram, "layer"), textData.m_layer);

				glUniformMatrix3fv(glGetUniformLocation(m_textShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currViewMatrix));

				glUniformMatrix3fv(glGetUniformLocation(m_textShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currOrthoMatrix));

				glm::vec2 point = { textData.m_x, textData.m_y };

				float angle = textData.m_rotate * 3.1451f / 180.f;

				glm::mat3 rotationMatrix = {
				cos(angle), -sin(angle), 0.0f,
				sin(angle), cos(angle),  0.0f,
				0.0f,       0.0f,       1.0 };

				glUniformMatrix3fv(glGetUniformLocation(m_textShaderProgram, "rotate"), 1, GL_FALSE, glm::value_ptr(rotationMatrix));

				glUniform2fv(glGetUniformLocation(m_textShaderProgram, "point"), 1, glm::value_ptr(point));

				// Set the text color
				glUniform3f(glGetUniformLocation(m_textShaderProgram, "textColor"), textData.m_color.x, textData.m_color.y, textData.m_color.z);

				glActiveTexture(GL_TEXTURE0);
				glBindVertexArray(m_textMesh.m_vaoId);

				// Iterate through all characters
				assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
				float origin{textData.m_x};
				float height = GraphicsCamera::m_windowHeight;
				for (const char& c : textData.m_text) 
				{
					text::CharacterData ch = assetmanager->m_fontManager.m_fonts[textData.m_fileName][c];
					if (!height)
					{
						break;
					}
					// Calculate position and size for each character quad
					float xpos = (textData.m_x + ch.m_bearing.x / GraphicsCamera::m_windowHeight * textData.m_scale);
					float ypos = (textData.m_y - (ch.m_size.y - ch.m_bearing.y )/GraphicsCamera::m_windowHeight * textData.m_scale);
					float w = ch.m_size.x * textData.m_scale / GraphicsCamera::m_windowHeight;
					float h = ch.m_size.y * textData.m_scale / GraphicsCamera::m_windowHeight;
		

					// Update VBO for each character with texture coordinates from the atlas
					float vertices[6][4] = 
					{
					{ xpos,     ypos + h,   ch.m_topLeftTexCoords.x, ch.m_topLeftTexCoords.y },
					{ xpos,     ypos,       ch.m_topLeftTexCoords.x, ch.m_bottomRightTexCoords.y },
					{ xpos + w, ypos,       ch.m_bottomRightTexCoords.x, ch.m_bottomRightTexCoords.y },

					{ xpos,     ypos + h,   ch.m_topLeftTexCoords.x, ch.m_topLeftTexCoords.y },
					{ xpos + w, ypos,       ch.m_bottomRightTexCoords.x, ch.m_bottomRightTexCoords.y },
					{ xpos + w, ypos + h,   ch.m_bottomRightTexCoords.x, ch.m_topLeftTexCoords.y }
					};

					// Bind the texture atlas (once for all characters)
					glBindTexture(GL_TEXTURE_2D, ch.m_textureID);

					// Update the content of the VBO memory
					glBindBuffer(GL_ARRAY_BUFFER, m_textBuffer);
					glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
					glBindBuffer(GL_ARRAY_BUFFER, 0);

					// Render quad
					glDrawArrays(GL_TRIANGLES, 0, 6);

					// Advance cursor for next glyph
					textData.m_x += ((ch.m_advance >> 6) * textData.m_scale) / GraphicsCamera::m_windowHeight ;
					
				}
				textData.m_x = origin;

				// Unbind for safety
				glBindVertexArray(0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
	}

	void GraphicsPipe::m_funcDrawGrid() 
	{
		glUseProgram(m_gridShaderProgram);

		glUniformMatrix3fv(glGetUniformLocation(m_gridShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currViewMatrix));
		glUniformMatrix3fv(glGetUniformLocation(m_gridShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currOrthoMatrix));
		glBindVertexArray(m_lineMesh.m_vaoId);
		glDrawArrays(GL_LINES, 0, 1000 * 8 + 12); // Number of vertices
		glBindVertexArray(0);
			
	}

	void GraphicsPipe::m_funcSetDrawMode(GLenum mode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, mode);
	}

}


