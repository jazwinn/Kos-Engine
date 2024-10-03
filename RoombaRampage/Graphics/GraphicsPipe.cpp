#include "../Graphics/GraphicsPipe.h"
#include "../Assets/AssetManager.h"
#include "../Application/Application.h"
#include "../Application/Helper.h"
#include "../Assets/Font.h"
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include "../ECS/ECSList.h"

namespace graphicpipe {


	const std::string debugVertexShader =
	{
	#include "../Graphics/debugVertexShader.vert"
	};

	const std::string debugFragmentShader =
	{
	#include  "../Graphics/debugFragmentShader.frag"

	};

	const std::string frameBufferVertexShader =
	{
	 #include "../Graphics/frameBufferVertexShader.vert"
	};

	const std::string frameBufferFragmentShader =
	{
	  #include "../Graphics/frameBufferFragmentShader.frag"
	};


	const std::string genericVertexShader =
	{
	 #include "../Graphics/genericVertexShader.vert"
	};

	const std::string genericFragmentShader =
	{
	  #include "../Graphics/genericFragmentShader.frag"
	};

	const std::string textVertexShader =
	{
	 #include "../Graphics/textVertexShader.vert"
	};

	const std::string textFragmentShader =
	{
	  #include "../Graphics/textFragmentShader.frag"
	};

	std::unique_ptr<GraphicsPipe> GraphicsPipe::m_instancePtr = nullptr;

	void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		//TOCHECK
		std::cerr << "OpenGL Debug Message: " << message << " Source " << source << " Type " << type << " ID " << id << "Severity " << severity << " Length " << length << " userParam " << userParam << std::endl;
	}

	void GraphicsPipe::m_funcInit()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glLineWidth(4.f);

		m_squareMesh.m_shapeType = SQUARE;
		m_squareLinesMesh.m_shapeType = SQUARE_LINES;
		m_testMatrix = { 1,0,0,0,1,0,0,0,1 };
		m_modelData.reserve(ecs::MaxEntity);
		m_debugBoxData.reserve(ecs::MaxEntity);
		m_textData.reserve(ecs::MaxEntity);
		m_modelToNDCMatrix.reserve(ecs::MaxEntity);
		m_debugToNDCMatrix.reserve(ecs::MaxEntity);
		m_debugBoxCollisionChecks.reserve(ecs::MaxEntity);
		m_frameNumbers.reserve(ecs::MaxEntity);
		m_stripCounts.reserve(ecs::MaxEntity);

		m_funcSetupVao(m_squareMesh);
		m_funcSetupSquareLinesVao();
		m_funcSetupFboVao();
		m_funcSetupTextVao();
		m_funcSetDrawMode(GL_FILL);

		m_genericShaderProgram = m_funcSetupShader(genericVertexShader, genericFragmentShader);
		m_frameBufferShaderProgram = m_funcSetupShader(frameBufferVertexShader, frameBufferFragmentShader);
		m_debugShaderProgram = m_funcSetupShader(debugVertexShader, debugFragmentShader);
		m_textShaderProgram = m_funcSetupShader(textVertexShader, textFragmentShader);

		m_modelToNDCMatrix.push_back(m_testMatrix);
		m_textureOrder.push_back(0);
		m_frameNumbers.push_back(0);
		m_stripCounts.push_back(0);
		m_debugToNDCMatrix.push_back(m_testMatrix);
		m_debugBoxCollisionChecks.push_back(false);
		//TOCHECK
		//COMMENT THIS OUT?
		//GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		//const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		m_windowWidth = static_cast<int>(Helper::Helpers::GetInstance()->m_windowWidth);
		m_windowHeight = static_cast<int>(Helper::Helpers::GetInstance()->m_windowHeight);
		m_aspectRatio = static_cast<float>(static_cast<float>(m_windowHeight) / static_cast<float>(m_windowWidth));

		m_funcSetupArrayBuffer();
		//m_funcBindImageDatafromAssetManager();


		m_funcSetupFrameBuffer();

		m_debugToNDCMatrix.clear();
		m_debugBoxCollisionChecks.clear();
		m_modelToNDCMatrix.clear();
		m_textureOrder.clear();
		m_frameNumbers.clear();
		m_stripCounts.clear();

		glEnable(GL_SCISSOR_TEST);
		glScissor(0, 0, m_windowWidth, m_windowHeight);

		/*glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(DebugCallback, nullptr);*/
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

	void GraphicsPipe::m_funcSetupSquareLinesVao()
	{
		std::vector<glm::vec2> lvPosVtx;
		std::vector<GLushort>idx_vtx;

		lvPosVtx = { glm::vec2(0.5f, -0.5f),
					glm::vec2(0.5f, 0.5f),
					glm::vec2(-0.5f, 0.5f),
					glm::vec2(-0.5f, -0.5f) };



		GLsizei position_data_offset = 0;
		GLsizei position_attribute_size = sizeof(glm::vec2);
		GLsizei position_data_size = position_attribute_size * static_cast<GLsizei>(lvPosVtx.size());


		unsigned int lvVboId{};

		glCreateBuffers(1, &lvVboId);


		glNamedBufferStorage(lvVboId, position_data_size, nullptr, GL_DYNAMIC_STORAGE_BIT);
		glNamedBufferSubData(lvVboId, position_data_offset, position_data_size, lvPosVtx.data());

		glCreateVertexArrays(1, &m_squareLinesMesh.m_vaoId);
		glEnableVertexArrayAttrib(m_squareLinesMesh.m_vaoId, 0);
		glVertexArrayVertexBuffer(m_squareLinesMesh.m_vaoId, 0, lvVboId,
			position_data_offset, position_attribute_size);
		glVertexArrayAttribFormat(m_squareLinesMesh.m_vaoId, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(m_squareLinesMesh.m_vaoId, 0, 0);

		m_squareLinesMesh.m_primitiveType = GL_LINE_LOOP;
		idx_vtx = { 0, 1, 2, 3 };


		m_squareLinesMesh.m_indexElementCount = static_cast<unsigned short>(idx_vtx.size());
		unsigned int ebo_hdl;
		glCreateBuffers(1, &ebo_hdl);
		glNamedBufferStorage(ebo_hdl, sizeof(unsigned short) * m_squareLinesMesh.m_indexElementCount,
			reinterpret_cast<GLvoid*>(idx_vtx.data()),
			GL_DYNAMIC_STORAGE_BIT);
		glVertexArrayElementBuffer(m_squareLinesMesh.m_vaoId, ebo_hdl);
		glBindVertexArray(0);
	}

	void GraphicsPipe::m_funcSetupFboVao()
	{
		float quadVertices[] =
		{
			// Positions        // Texture Coords
			-1.0f,  1.0f,       0.0f, 1.0f,  // Top-left
			-1.0f, -1.0f,       0.0f, 0.0f,  // Bottom-left
			 1.0f, -1.0f,       1.0f, 0.0f,  // Bottom-right
			 1.0f, -1.0f,       1.0f, 0.0f,  // Bottom-right
			 1.0f,  1.0f,       1.0f, 1.0f,  // Top-right
			-1.0f,  1.0f,       0.0f, 1.0f   // Top-left
		};

		unsigned int vboID;
		glGenVertexArrays(1, &m_screenMesh.m_vaoId);
		glGenBuffers(1, &vboID);

		glBindVertexArray(m_screenMesh.m_vaoId);

		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	void GraphicsPipe::m_funcSetupTextVao()
	{
		
		glGenVertexArrays(1, &m_textMesh.m_vaoId);
		glGenBuffers(1, &m_textBuffer);

		glBindVertexArray(m_textMesh.m_vaoId);

		glBindBuffer(GL_ARRAY_BUFFER, m_textBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	void GraphicsPipe::m_funcSetupVao(Mesh& shape)
	{
		std::vector<glm::vec2> lvPosVtx;
		std::vector<glm::vec3> lvClrVtx;
		std::vector<glm::vec2> lvTexCoords;
		std::vector<GLushort>idx_vtx;

		
		lvPosVtx	= { glm::vec2(0.5f, -0.5f),
						glm::vec2(0.5f, 0.5f),
						glm::vec2(-0.5f, 0.5f),
						glm::vec2(-0.5f, -0.5f) };
		lvClrVtx	= { glm::vec3(1.f, 1.f, 1.f),
						glm::vec3(1.f, 1.f, 1.f),
						glm::vec3(1.f, 1.f, 1.f),
						glm::vec3(1.f, 1.f, 1.f) };
		lvTexCoords	= { glm::vec2(1.f, 0.f),
						glm::vec2(1.f, 1.f),
						glm::vec2(0.f, 1.f),
						glm::vec2(0.f, 0.f) };



		GLsizei position_data_offset = 0;
		GLsizei position_attribute_size = sizeof(glm::vec2);
		GLsizei position_data_size = position_attribute_size * static_cast<GLsizei>(lvPosVtx.size());
		GLsizei color_data_offset = position_data_size;
		GLsizei color_attribute_size = sizeof(glm::vec3);
		GLsizei color_data_size = color_attribute_size * static_cast<GLsizei>(lvClrVtx.size());
		GLsizei texcoord_data_offset = position_data_size + color_data_size;
		GLsizei texcoord_attribute_size = sizeof(glm::vec2);
		GLsizei texcoord_data_size = texcoord_attribute_size * static_cast<GLsizei>(lvTexCoords.size());


		unsigned int lvVboId{};

		glCreateBuffers(1, &lvVboId);

		glNamedBufferStorage(lvVboId, position_data_size + color_data_size + texcoord_data_size, nullptr, GL_DYNAMIC_STORAGE_BIT);

		glNamedBufferSubData(lvVboId, position_data_offset, position_data_size, lvPosVtx.data());
		glNamedBufferSubData(lvVboId, color_data_offset, color_data_size, lvClrVtx.data());
		glNamedBufferSubData(lvVboId, texcoord_data_offset, texcoord_data_size, lvTexCoords.data());
	
		glCreateVertexArrays(1, &shape.m_vaoId);
		glEnableVertexArrayAttrib(shape.m_vaoId, 0);
		glVertexArrayVertexBuffer(shape.m_vaoId, 0, lvVboId,
			position_data_offset, position_attribute_size);
		glVertexArrayAttribFormat(shape.m_vaoId, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(shape.m_vaoId, 0, 0);

		glEnableVertexArrayAttrib(shape.m_vaoId, 1);
		glVertexArrayVertexBuffer(shape.m_vaoId, 1, lvVboId,
			color_data_offset, color_attribute_size);
		glVertexArrayAttribFormat(shape.m_vaoId, 1, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(shape.m_vaoId, 1, 1);


		glEnableVertexArrayAttrib(shape.m_vaoId, 2);
		glVertexArrayVertexBuffer(shape.m_vaoId, 2, lvVboId, texcoord_data_offset,
			texcoord_attribute_size);
		glVertexArrayAttribFormat(shape.m_vaoId, 2, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(shape.m_vaoId, 2, 2);
	


		shape.m_primitiveType = GL_TRIANGLES;
		idx_vtx = { 0, 1, 2, 2, 3, 0 };
		

		shape.m_indexElementCount = static_cast<unsigned short>(idx_vtx.size());
		unsigned int ebo_hdl;
		glCreateBuffers(1, &ebo_hdl);
		glNamedBufferStorage(ebo_hdl, sizeof(unsigned short) * shape.m_indexElementCount,
			reinterpret_cast<GLvoid*>(idx_vtx.data()),
			GL_DYNAMIC_STORAGE_BIT);
		glVertexArrayElementBuffer(shape.m_vaoId, ebo_hdl);
		glBindVertexArray(0);
	}

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
		glBufferData(GL_ARRAY_BUFFER, m_debugToNDCMatrix.size() * sizeof(glm::mat3), &m_debugToNDCMatrix[0], GL_DYNAMIC_DRAW);
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

		
		err = glGetError();
		if (err != GL_NO_ERROR) {
			std::cout << "Third OpenGL Error: " << err << std::endl;
		}

	}

	void GraphicsPipe::m_funcSetupFrameBuffer()
	{
		unsigned int fbo;
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_windowWidth, m_windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_windowWidth, m_windowHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);


		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Framebuffer successfully created" << std::endl;
			m_frameBufferObject = fbo;
			m_screenTexture = texture;
		}
		else
		{
			std::cout << "Framebuffer has not been created" << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

				glm::mat3 lvScale{ m_modelData[n].m_scale.x * widthRatio / imageAspectRatio, 0, 0, 0, m_modelData[n].m_scale.y * heightRatio , 0, 0 , 0 ,1 };
				glm::mat3 lvRotate{ cos(m_modelData[n].m_rotate * 3.1415f / 180.f), -sin(m_modelData[n].m_rotate * 3.1415f / 180.f), 0.f,
								   sin(m_modelData[n].m_rotate * 3.1415f / 180.f), cos(m_modelData[n].m_rotate * 3.1415f / 180.f), 0.f,
									0.f , 0.f ,1.f };
				glm::mat3 lvTranslate{ 1, 0, 0, 0, 1, 0, m_modelData[n].m_worldCoordinates.x , m_modelData[n].m_worldCoordinates.y ,1 };

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
			
				

				float totalFrameTime = m_frameTime * m_imageData[m_modelData[n].m_textureID].m_stripCount;
				float frameTime = static_cast<float>(fmod(m_modelData[n].m_animationTimer, totalFrameTime));
				int frameNumber = static_cast<int>(frameTime / m_frameTime);

				m_modelToNDCMatrix.push_back(lvNDCScale * lvTranslate * lvRotate * lvScale);
				m_textureOrder.push_back(m_modelData[n].m_textureID);
				m_stripCounts.push_back(m_imageData[m_modelData[n].m_textureID].m_stripCount);
				m_frameNumbers.push_back(frameNumber);			
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



	GLuint GraphicsPipe::m_funcCompileShader(GLuint type, const std::string& shader)
	{
		GLuint lvID = glCreateShader(type);
		const char* source = shader.c_str();
		glShaderSource(lvID, 1, &source, nullptr);
		glCompileShader(lvID);

		int result;
		glGetShaderiv(lvID, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			int length;
			glGetShaderiv(lvID, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)malloc(length * sizeof(char));
			glGetShaderInfoLog(lvID, length, &length, message);
			std::cout << "Failed to Compile Shader" << std::endl;
			std::cout << message << std::endl;
			glDeleteShader(lvID);
			free(message);
			//TOCHECK
			//std::exit;
			return 0;
		}

		return lvID;
	}

	unsigned int GraphicsPipe::m_funcSetupShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		GLuint lvProgram = glCreateProgram();
		GLuint lvVertexShaderID = m_funcCompileShader(GL_VERTEX_SHADER, vertexShader);
		GLuint lvFragmentShaderID = m_funcCompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(lvProgram, lvVertexShaderID);
		glAttachShader(lvProgram, lvFragmentShaderID);
		glLinkProgram(lvProgram);
		glValidateProgram(lvProgram);

		glDeleteShader(lvVertexShaderID);
		glDeleteShader(lvFragmentShaderID);

		GLint success;
		glGetProgramiv(lvProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLchar infoLog[512];
			glGetProgramInfoLog(lvProgram, 512, NULL, infoLog);
			std::cout << "Error linking shader program:\n" << infoLog << std::endl;
		}
		else
		{
			std::cout << "Shader compiled successfully" << std::endl;
		}

		return lvProgram;
	}


	void GraphicsPipe::m_funcDeleteShader()
	{
		glDeleteProgram(m_genericShaderProgram);
	}


	void GraphicsPipe::m_funcSetDrawMode(GLenum mode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, mode);
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

		//glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void GraphicsPipe::m_funcDrawText()
	{

		if (!m_textData.empty()) {

			for (auto& textData : m_textData) {
				// activate corresponding render state	
				glUseProgram(m_textShaderProgram);
				glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_windowWidth), 0.0f, static_cast<float>(m_windowHeight));
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
}

