/******************************************************************/
/*!
\file      GraphicsDraw.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th Nov, 2024
\brief     Implementation of rendering functions for the GraphicsPipe class,
		   which manages various draw calls and prepares OpenGL resources
		   for rendering complex scenes, including 2D shapes, text, and debug views.

\details   This file includes functions for:
		   - Rendering primary and debug elements in separate framebuffers.
		   - Binding and updating buffer data, including matrices, texture orders,
			 and colors, for instanced drawing.
		   - Text rendering, including character positioning, scaling, and color.
		   - Tilemap rendering for rendering multiple tiles as 1 object.
		   - Utility functions for setting polygon modes and drawing grid overlays.

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
	void GraphicsPipe::m_funcDraw()
	{
		if (!m_modelToNDCMatrix.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_modelMatrixArrayBuffer);
			glNamedBufferData(m_modelMatrixArrayBuffer, m_modelToNDCMatrix.size() * sizeof(glm::mat3), &m_modelToNDCMatrix[0], GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, m_iVec3Buffer);
			glNamedBufferData(m_iVec3Buffer, m_iVec3Array.size() * sizeof(glm::ivec3), &m_iVec3Array[0], GL_DYNAMIC_DRAW); //Strip Count, FrameNumber, Texture Order

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
				LOGGING_ERROR("Uniform variable location: %d", lvUniformVarLoc1);
				LOGGING_ERROR("Uniform variable 'textures' doesn't exist!");
				std::exit(EXIT_FAILURE);
			}

			for (int i = 0; i < m_textureIDs.size(); ++i)
			{
				glActiveTexture(GL_TEXTURE0 + m_textureIDs[i]);
				glBindTexture(GL_TEXTURE_2D, m_textureIDs[i]);
			
			}
			
			glUniform1f(glGetUniformLocation(m_genericShaderProgram, "globalBrightness"), m_globalLightIntensity);

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

	void GraphicsPipe::m_funcDrawGameFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_funcDrawTilemap();
		m_funcDraw();
		m_funcDrawText();
		m_funcRenderLighting();

		// Switch back to the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

		// Use the framebuffer shader program
		glUseProgram(m_frameBufferShaderProgram);

		// Draw the framebuffer texture to the screen
		glBindVertexArray(m_screenTextureVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_screenTexture);

		glUniform1f(glGetUniformLocation(m_frameBufferShaderProgram, "globalBrightness"), m_globalLightIntensity);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Check for OpenGL errors
		//GLenum err;
		//while ((err = glGetError()) != GL_NO_ERROR) {
		//	LOGGING_ERROR("OpenGL Error: 0x%X", err);
		//}
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
		m_funcRenderLighting();
		

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(m_frameBufferShaderProgram);



		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			//LOGGING_ERROR("First OpenGL Error: 0x%X", err);
			std::cout << "Draw Window OpenGL Error: " << err << std::endl;
		}
	}

	void GraphicsPipe::m_drawLightingTexture()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_lightingFrameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

		m_funcRenderLighting();

		// Switch back to the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

		// Check for OpenGL errors
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			LOGGING_ERROR("#01 OpenGL Error: 0x%X", err);
		}
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

		glUseProgram(m_frameBufferShaderProgram);

	}

	void GraphicsPipe::m_funcDrawText()
	{

		if (!m_textData.empty() && GraphicsCamera::m_windowHeight > 0 && GraphicsCamera::m_windowWidth > 0) {
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
				float origin{ textData.m_x };

				assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
				float originX{ textData.m_x };
				float originY{ textData.m_y };

				// Step 1: Calculate total width and height of the text
				float totalWidth = 0.0f;
				float maxAscent = 0.0f;
				float maxDescent = 0.0f;

				for (const char& c : textData.m_text)
				{
					text::CharacterData ch = assetmanager->m_fontManager.m_fonts[textData.m_fileName][c];
					totalWidth += ((ch.m_advance >> 6) * textData.m_scale * textData.m_xyScale.x) / GraphicsCamera::m_windowHeight;
					maxAscent = std::max(maxAscent, (ch.m_bearing.y * textData.m_scale * textData.m_xyScale.y) / GraphicsCamera::m_windowHeight);
					maxDescent = std::max(maxDescent, ((ch.m_size.y - ch.m_bearing.y) * textData.m_scale * textData.m_xyScale.y) / GraphicsCamera::m_windowHeight);
				}
				float totalHeight = maxAscent + maxDescent;

				// Adjust starting position to center the text
				textData.m_x = originX - totalWidth / 2.0f;  // Horizontal centering
				textData.m_y = originY + maxAscent / 2.0f - totalHeight / 2.0f;  // Vertical centering

				// Step 2: Render the text
				for (const char& c : textData.m_text)
				{
					text::CharacterData ch = assetmanager->m_fontManager.m_fonts[textData.m_fileName][c];

					// Calculate position and size for each character quad
					float xpos = (textData.m_x + ch.m_bearing.x / GraphicsCamera::m_windowHeight * (textData.m_scale * textData.m_xyScale.x));
					float ypos = (textData.m_y - ((float)ch.m_size.y - (float)ch.m_bearing.y) / (float)GraphicsCamera::m_windowHeight * (textData.m_scale * textData.m_xyScale.y));
					float w = ch.m_size.x * textData.m_scale * textData.m_xyScale.x / GraphicsCamera::m_windowHeight;
					float h = ch.m_size.y * textData.m_scale * textData.m_xyScale.y / GraphicsCamera::m_windowHeight;

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
					textData.m_x += ((ch.m_advance >> 6) * textData.m_scale * textData.m_xyScale.x) / GraphicsCamera::m_windowHeight;
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
		glUniform3f(glGetUniformLocation(m_gridShaderProgram, "debugColor"), 0.f, 0.f, 0.f);
		glBindVertexArray(m_lineMesh.m_vaoId);
		glDrawArrays(GL_LINES, 0, 1000 * 8 + 12); // Number of vertices of the grid
		glBindVertexArray(0);
			
	}

	void GraphicsPipe::m_funcDrawLine(glm::vec3 p0, glm::vec3 p1)
	{

		std::vector<glm::vec3> vertices;

		vertices.push_back(p0);
		vertices.push_back(p1);

		unsigned int vaoid;
		unsigned int vboid;
		glGenVertexArrays(1, &vaoid);
		glGenBuffers(1, &vboid);

		glBindVertexArray(vaoid);

		glBindBuffer(GL_ARRAY_BUFFER, vboid);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		glUseProgram(m_gridShaderProgram);

		glUniformMatrix3fv(glGetUniformLocation(m_gridShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currViewMatrix));
		glUniformMatrix3fv(glGetUniformLocation(m_gridShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currOrthoMatrix));
		glUniform3f(glGetUniformLocation(m_gridShaderProgram, "debugColor"), 0.5f , 0.1f, 0.1f);
		glDrawArrays(GL_LINES, 0, 2); // Number of vertices
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	void GraphicsPipe::m_funcDrawGridCollider()
	{

		glUseProgram(m_gridDebugShaderProgram);

		for (int i{}; i < m_colliderGridData.size() && !m_gridColliderChecks.empty(); ++i)
		{
			if (m_gridColliderChecks[i].empty())
			{
				continue;
			}

			GLenum err = glGetError();
			if (err != GL_NO_ERROR) {
				//LOGGING_ERROR("First OpenGL Error: 0x%X", err);
				std::cout << "#02 OpenGL Error: " << err << std::endl;
			}

			glBindBuffer(GL_ARRAY_BUFFER, m_gridColliderBuffer);
			glNamedBufferData(m_gridColliderBuffer, m_gridColliderChecks[i].size() * sizeof(int), &m_gridColliderChecks[i][0], GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glUniform1i(glGetUniformLocation(m_gridDebugShaderProgram, "tilemapRows"), m_colliderGridData[i].m_gridDimensions.x);

			GLenum err2 = glGetError();
			if (err2 != GL_NO_ERROR) {
				//LOGGING_ERROR("First OpenGL Error: 0x%X", err);
				std::cout << "#03 OpenGL Error: " << err2 << std::endl;
			}

			glUniform1i(glGetUniformLocation(m_gridDebugShaderProgram, "tilemapColumns"), m_colliderGridData[i].m_gridDimensions.y);

			glUniformMatrix3fv(glGetUniformLocation(m_gridDebugShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currViewMatrix));

			glUniformMatrix3fv(glGetUniformLocation(m_gridDebugShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currOrthoMatrix));

			glUniformMatrix3fv(glGetUniformLocation(m_gridDebugShaderProgram, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(m_colliderGridData[i].m_transformation));

			glBindVertexArray(m_squareLinesMesh.m_vaoId);
			glDrawElementsInstanced(m_squareLinesMesh.m_primitiveType, m_squareLinesMesh.m_indexElementCount, GL_UNSIGNED_SHORT, NULL, static_cast<GLsizei>(m_colliderGridData[i].m_gridDimensions.x * m_colliderGridData[i].m_gridDimensions.y));
			glBindVertexArray(0);
		}
	}

	void GraphicsPipe::m_funcDrawTilemap()
	{
		glUseProgram(m_tilemapShaderProgram);

		for (int i{}; i < m_transformedTilemaps.size() && !m_tileIndexes.empty(); ++i)
		{
			if (m_tileIndexes[i].empty())
			{
				continue;
			}
			glBindBuffer(GL_ARRAY_BUFFER, m_tileIndexBuffer);
			glNamedBufferData(m_tileIndexBuffer, m_tileIndexes[i].size() * sizeof(int), &m_tileIndexes[i][0], GL_DYNAMIC_DRAW);

			//glBindBuffer(GL_ARRAY_BUFFER, m_gridColliderBuffer);
			//glNamedBufferData(m_gridColliderBuffer, m_gridColliderChecks[i].size() * sizeof(int), &m_gridColliderChecks[i][0], GL_DYNAMIC_DRAW);
			//glBindBuffer(GL_ARRAY_BUFFER, 0);


			glActiveTexture(GL_TEXTURE0 + m_textureIDs[m_transformedTilemaps[i].m_textureID]); // Activate each texture unit
			glBindTexture(GL_TEXTURE_2D, m_textureIDs[m_transformedTilemaps[i].m_textureID]);  // Unbind the 2D texture from that unit
			
			glUniform1i(glGetUniformLocation(m_tilemapShaderProgram, "textureID"), m_textureIDs[m_transformedTilemaps[i].m_textureID]);

			glUniform1i(glGetUniformLocation(m_tilemapShaderProgram, "layer"), m_transformedTilemaps[i].m_layer);

			glUniform1i(glGetUniformLocation(m_tilemapShaderProgram, "index"), m_transformedTilemaps[i].m_tileIndex);
		
			glUniform1i(glGetUniformLocation(m_tilemapShaderProgram, "tilemapRows"), m_transformedTilemaps[i].m_tilemapDimensions.x);

			glUniform1i(glGetUniformLocation(m_tilemapShaderProgram, "tilemapColumns"), m_transformedTilemaps[i].m_tilemapDimensions.y);

			glUniform1i(glGetUniformLocation(m_tilemapShaderProgram, "tilePicSizeX"), m_transformedTilemaps[i].m_tilemapPictureSize.x);

			glUniform1i(glGetUniformLocation(m_tilemapShaderProgram, "tilePicSizeY"), m_transformedTilemaps[i].m_tilemapPictureSize.y);

			glUniform4f(glGetUniformLocation(m_tilemapShaderProgram, "modelColor"), m_transformedTilemaps[i].m_color.r, m_transformedTilemaps[i].m_color.g, m_transformedTilemaps[i].m_color.b, m_transformedTilemaps[i].m_color.a);

			glUniformMatrix3fv(glGetUniformLocation(m_tilemapShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currViewMatrix));

			glUniformMatrix3fv(glGetUniformLocation(m_tilemapShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currOrthoMatrix));

			glUniformMatrix3fv(glGetUniformLocation(m_tilemapShaderProgram, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(m_transformedTilemaps[i].m_transformation));

			glUniform1f(glGetUniformLocation(m_tilemapShaderProgram, "globalBrightness"), m_globalLightIntensity);

			

			glBindVertexArray(m_squareMesh.m_vaoId);
			glDrawElementsInstanced(m_squareMesh.m_primitiveType, m_squareMesh.m_indexElementCount, GL_UNSIGNED_SHORT, NULL, static_cast<GLsizei>(m_transformedTilemaps[i].m_tilemapDimensions.x * m_transformedTilemaps[i].m_tilemapDimensions.y));
			glBindVertexArray(0);
			
		}

	}

	

	void GraphicsPipe::m_funcRenderLighting()
	{
		if (!m_lightingTransforms.empty())
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE); //USE THIS BLENDING FOR LIGHTS

			glUseProgram(m_lightingShaderProgram);

			glBindBuffer(GL_ARRAY_BUFFER, m_modelMatrixArrayBuffer);
			glNamedBufferData(m_modelMatrixArrayBuffer, m_lightingTransforms.size() * sizeof(glm::mat3), &m_lightingTransforms[0], GL_DYNAMIC_DRAW);


			glBindBuffer(GL_ARRAY_BUFFER, m_vec3Buffer);
			glNamedBufferData(m_vec3Buffer, m_lightingParams.size() * sizeof(glm::vec3), &m_lightingParams[0], GL_DYNAMIC_DRAW); // Light Intensity, Inner/Outer Radius

			glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
			glNamedBufferData(m_colorBuffer, m_lightingColors.size() * sizeof(glm::vec4), &m_lightingColors[0], GL_DYNAMIC_DRAW);
	

			glUniformMatrix3fv(glGetUniformLocation(m_lightingShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currViewMatrix));

			glUniformMatrix3fv(glGetUniformLocation(m_lightingShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currOrthoMatrix));

			GLenum err = glGetError();
			if (err != GL_NO_ERROR) {
				//LOGGING_ERROR("First OpenGL Error: 0x%X", err);
				std::cout << "#04 OpenGL Error: " << err << std::endl;
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			

			glBindVertexArray(m_squareMesh.m_vaoId);
			glDrawElementsInstanced(m_squareMesh.m_primitiveType, m_squareMesh.m_indexElementCount, GL_UNSIGNED_SHORT, NULL, static_cast<GLsizei>(m_lightingTransforms.size()));
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			GLenum err2 = glGetError();
			if (err2 != GL_NO_ERROR) {
				//LOGGING_ERROR("First OpenGL Error: 0x%X", err);
				std::cout << "#05 OpenGL Error : " << err2 << std::endl;
			}

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	}

	void GraphicsPipe::m_funcSetDrawMode(GLenum mode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, mode);
	}

}


