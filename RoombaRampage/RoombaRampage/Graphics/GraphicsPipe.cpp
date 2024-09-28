#include "../Graphics/GraphicsPipe.h"
#include "../Assets/AssetManager.h"
#include "../Application/Application.h"
#include "../Application/Helper.h"
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <gtc/type_ptr.hpp>

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

std::unique_ptr<GraphicsPipe> GraphicsPipe::instancePtr = nullptr;

void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cerr << "OpenGL Debug Message: " << message << std::endl;
}

void GraphicsPipe::funcInit()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(4.f);

	squareMesh.shapeType = SQUARE;
	squareLinesMesh.shapeType = SQUARE_LINES;
	testMatrix = { 1,0,0,0,1,0,0,0,1 };
	modelData.reserve(2500);
	debugBoxData.reserve(2500);
	modelToNDCMatrix.reserve(2500);
	debugToNDCMatrix.reserve(2500);
	debugBoxCollisionChecks.reserve(2500);

	funcSetupVao(squareMesh);
	funcSetupSquareLinesVao();
	funcSetupFboVao();
	funcSetDrawMode(GL_FILL);

	genericShaderProgram = funcSetupShader(genericVertexShader, genericFragmentShader);
	frameBufferShaderProgram = funcSetupShader(frameBufferVertexShader, frameBufferFragmentShader);
	debugShaderProgram = funcSetupShader(debugVertexShader, debugFragmentShader);

	modelToNDCMatrix.push_back(testMatrix);
	textureOrder.push_back(0);
	debugToNDCMatrix.push_back(testMatrix);
	debugBoxCollisionChecks.push_back(false);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	windowWidth = Helper::Helpers::GetInstance()->WindowWidth;
	windowHeight = Helper::Helpers::GetInstance()->WindowHeight;
	aspectRatio = static_cast<float>(static_cast<float>(windowHeight) / static_cast<float>(windowWidth));


	funcSetupArrayBuffer();
	funcBindImageDatafromAssetManager();


	funcSetupFrameBuffer();
	
	debugToNDCMatrix.clear();
	debugBoxCollisionChecks.clear();
	modelToNDCMatrix.clear();
	textureOrder.clear();

	/*glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(DebugCallback, nullptr);*/
}

GraphicsPipe::~GraphicsPipe() 
{
	//delete instancePtr;
	if (frameBufferObject)
	{
		glDeleteFramebuffers(1, &frameBufferObject);
	}
	funcDeleteShader();
}

GraphicsPipe* GraphicsPipe::funcGetInstance()
{
	if (!instancePtr) 
	{
		instancePtr.reset(new GraphicsPipe{});
	}
	return instancePtr.get();
}

void GraphicsPipe::funcSetupSquareLinesVao()
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


	glNamedBufferStorage(lvVboId,
	position_data_size,
	nullptr,
	GL_DYNAMIC_STORAGE_BIT);

	glNamedBufferSubData(lvVboId, position_data_offset, position_data_size, lvPosVtx.data());


	glCreateVertexArrays(1, &squareLinesMesh.vaoId);
	glEnableVertexArrayAttrib(squareLinesMesh.vaoId, 0);
	glVertexArrayVertexBuffer(squareLinesMesh.vaoId, 0, lvVboId,
		position_data_offset, position_attribute_size);
	glVertexArrayAttribFormat(squareLinesMesh.vaoId, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(squareLinesMesh.vaoId, 0, 0);

	squareLinesMesh.primitiveType = GL_LINE_LOOP;
	idx_vtx = { 0, 1, 2, 3};


	squareLinesMesh.indexElementCount = static_cast<unsigned int>(idx_vtx.size());
	unsigned int ebo_hdl;
	glCreateBuffers(1, &ebo_hdl);
	glNamedBufferStorage(ebo_hdl, sizeof(unsigned short) * squareLinesMesh.indexElementCount,
		reinterpret_cast<GLvoid*>(idx_vtx.data()),
		GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(squareLinesMesh.vaoId, ebo_hdl);
	glBindVertexArray(0);
}

void GraphicsPipe::funcSetupFboVao()
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
	glGenVertexArrays(1, &screenMesh.vaoId);
	glGenBuffers(1, &vboID);

	glBindVertexArray(screenMesh.vaoId);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	

	// Texture Coord attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GraphicsPipe::funcSetupVao(Mesh &shape)
{
	std::vector<glm::vec2> lvPosVtx;
	std::vector<glm::vec3> lvClrVtx;
	std::vector<glm::vec2> lvTexCoords;
	std::vector<GLushort>idx_vtx;

	if (shape.shapeType == SQUARE || shape.shapeType == SQUARE_LINES)
	{
		lvPosVtx = { glm::vec2(0.5f, -0.5f),
					glm::vec2(0.5f, 0.5f),
					glm::vec2(-0.5f, 0.5f),
					glm::vec2(-0.5f, -0.5f) };
		lvClrVtx = { glm::vec3(1.f, 1.f, 1.f),
					glm::vec3(1.f, 1.f, 1.f),
					glm::vec3(1.f, 1.f, 1.f),
					glm::vec3(1.f, 1.f, 1.f) };
		lvTexCoords = { glm::vec2(1.f, 0.f),
					   glm::vec2(1.f, 1.f),
					   glm::vec2(0.f, 1.f),
					   glm::vec2(0.f, 0.f) };
	}


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

	if (shape.shapeType == SQUARE)
	{
		glNamedBufferStorage(lvVboId,
			position_data_size + color_data_size + texcoord_data_size,
			nullptr,
			GL_DYNAMIC_STORAGE_BIT);
	}
	else if (shape.shapeType == SQUARE_LINES)
	{

		glNamedBufferStorage(lvVboId,
			position_data_size + color_data_size,
			nullptr,
			GL_DYNAMIC_STORAGE_BIT);
	}

	

	glNamedBufferSubData(lvVboId, position_data_offset, position_data_size, lvPosVtx.data());
	glNamedBufferSubData(lvVboId, color_data_offset, color_data_size, lvClrVtx.data());
	if (shape.shapeType == SQUARE)
	{
		glNamedBufferSubData(lvVboId, texcoord_data_offset, texcoord_data_size, lvTexCoords.data());
	}
	glCreateVertexArrays(1, &shape.vaoId);
	glEnableVertexArrayAttrib(shape.vaoId, 0);
	glVertexArrayVertexBuffer(shape.vaoId, 0, lvVboId,
		position_data_offset, position_attribute_size);
	glVertexArrayAttribFormat(shape.vaoId, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(shape.vaoId, 0, 0);

	glEnableVertexArrayAttrib(shape.vaoId, 1);
	glVertexArrayVertexBuffer(shape.vaoId, 1, lvVboId,
		color_data_offset, color_attribute_size);
	glVertexArrayAttribFormat(shape.vaoId, 1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(shape.vaoId, 1, 1);


	if (shape.shapeType == SQUARE)
	{
		glEnableVertexArrayAttrib(shape.vaoId, 2);
		glVertexArrayVertexBuffer(shape.vaoId, 2, lvVboId, texcoord_data_offset,
			texcoord_attribute_size);
		glVertexArrayAttribFormat(shape.vaoId, 2, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(shape.vaoId, 2, 2);
	}
	

	if (shape.shapeType == SQUARE)
	{
		shape.primitiveType = GL_TRIANGLES;
		idx_vtx = { 0, 1, 2, 2, 3, 0 };
	}
	else if (shape.shapeType == SQUARE_LINES)
	{/*
		shape.primitiveType = GL_LINE_LOOP;
		idx_vtx = { 0, 1, 2, 3 };*/
		shape.primitiveType = GL_TRIANGLES;
		idx_vtx = { 0, 1, 2, 2, 3, 0 };
	}

	shape.indexElementCount = static_cast<unsigned int>(idx_vtx.size());
	unsigned int ebo_hdl;
	glCreateBuffers(1, &ebo_hdl);
	glNamedBufferStorage(ebo_hdl, sizeof(unsigned short) * shape.indexElementCount,
		reinterpret_cast<GLvoid*>(idx_vtx.data()),
		GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(shape.vaoId, ebo_hdl);
	glBindVertexArray(0);
}

void GraphicsPipe::funcSetupArrayBuffer()
{
	//Square Mesh Buffer Setup
	glBindVertexArray(squareMesh.vaoId);
	glGenBuffers(1, &modelMatrixArrayBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, modelMatrixArrayBuffer);
	glBufferData(GL_ARRAY_BUFFER, modelToNDCMatrix.size() * sizeof(glm::mat3), &modelToNDCMatrix[0], GL_DYNAMIC_DRAW);
	glBindVertexArray(squareMesh.vaoId);
	unsigned int location = 6;
	for (int i = 0; i < 3; ++i)
	{
		glEnableVertexAttribArray(location + i);
		glVertexAttribPointer(location + i, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), (void*)(sizeof(glm::vec3) * i));
		glVertexAttribDivisor(location + i, 1);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);	

	glGenBuffers(1, &textureOrderBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, textureOrderBuffer);
	glBufferData(GL_ARRAY_BUFFER, textureOrder.size() * sizeof(int), &textureOrder[0], GL_DYNAMIC_DRAW);
	glBindVertexArray(squareMesh.vaoId);
	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, 1, GL_INT, sizeof(int), (void*)0);
	glVertexAttribDivisor(5, 1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Square Lines Mesh Buffer Setup
	glBindVertexArray(squareLinesMesh.vaoId);
	glGenBuffers(1, &debugMatrixArrayBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, debugMatrixArrayBuffer);
	glBufferData(GL_ARRAY_BUFFER, debugToNDCMatrix.size() * sizeof(glm::mat3), &debugToNDCMatrix[0], GL_DYNAMIC_DRAW);
	unsigned int otherLocation = 7; // Location 7
	for (int i = 0; i < 3; ++i)
	{
		glEnableVertexAttribArray(otherLocation + i);
		glVertexAttribPointer(otherLocation + i, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), (void*)(sizeof(glm::vec3) * i));
		glVertexAttribDivisor(otherLocation + i, 1);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &debugCollisionCheckBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, debugCollisionCheckBuffer);
	glBufferData(GL_ARRAY_BUFFER, debugBoxCollisionChecks.size() * sizeof(int), &debugBoxCollisionChecks[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
	glVertexAttribDivisor(4, 1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
}

void GraphicsPipe::funcSetupFrameBuffer()
{
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer successfully created" << std::endl;
		frameBufferObject = fbo;
		screenTexture = texture;
	}
	else
	{
		std::cout << "Framebuffer has not been created" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void GraphicsPipe::funcBindImageDatafromAssetManager()
{
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	AssetManager* assets = AssetManager::funcGetInstance();
	for (int i = 0; i < assets->imageContainer.size(); ++i)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, assets->imageContainer[i].width, assets->imageContainer[i].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, assets->imagedataArray[i]);
		glGenerateMipmap(GL_TEXTURE_2D);
		textureIDs.push_back(textureID);
		std::cout << "Texture Binded, Texture ID: " << textureID << std::endl;
	}

	imageData = assets->imageContainer;
	
}

void GraphicsPipe::funcSortDrawOrder()
{
	if (!modelData.empty())
	{
		std::sort(modelData.begin(), modelData.end(), [](GraphicsData a, GraphicsData b)
			{
				return a.worldCoordinates.z > b.worldCoordinates.z;
			});
	}
	
}


void GraphicsPipe::funcUpdate()
{
	if (modelData.size() > 0) 
	{
		for (int n{}; n < modelData.size(); n++) 
		{
			float heightRatio = static_cast<float>(imageData[modelData[n].textureID].height) / unitHeight;
			float widthRatio = static_cast<float>(imageData[modelData[n].textureID].width) / unitWidth;

	
			glm::mat3 lvScale{ modelData[n].scale.x * widthRatio , 0, 0, 0, modelData[n].scale.y * heightRatio, 0, 0 , 0 ,1};
			glm::mat3 lvRotate{ cos(modelData[n].rotate * 3.1415f / 180.f), -sin(modelData[n].rotate * 3.1415f / 180.f), 0.f,
							   sin(modelData[n].rotate * 3.1415f / 180.f), cos(modelData[n].rotate * 3.1415f / 180.f), 0.f,
							    0.f , 0.f ,1.f };
			glm::mat3 lvTranslate{ 1, 0, 0, 0, 1, 0, modelData[n].worldCoordinates.x , modelData[n].worldCoordinates.y ,1 };
			glm::mat3 lvNDCScale{ aspectRatio, 0, 0, 0, 1.f, 0, 0 , 0 ,1.f };
			modelToNDCMatrix.push_back(lvNDCScale * lvTranslate * lvRotate * lvScale);
			textureOrder.push_back(modelData[n].textureID);
		}
	}

	if (debugBoxData.size() > 0)
	{
		for (int i{}; i < debugBoxData.size(); i++)
		{
			glm::mat3 lvScale{ debugBoxData[i].scale.x, 0, 0, 0, debugBoxData[i].scale.y, 0, 0 , 0 ,1 };
			glm::mat3 lvRotate{ cos(debugBoxData[i].rotate * 3.1415f / 180.f), -sin(debugBoxData[i].rotate * 3.1415f / 180.f), 0.f,
								sin(debugBoxData[i].rotate * 3.1415f / 180.f), cos(debugBoxData[i].rotate * 3.1415f / 180.f), 0.f,
								0.f , 0.f ,1.f };
			glm::mat3 lvTranslate{ 1, 0, 0, 0, 1, 0, debugBoxData[i].worldCoordinates.x , debugBoxData[i].worldCoordinates.y ,1 };
			glm::mat3 lvNDCScale{ aspectRatio, 0, 0, 0, 1.f, 0, 0 , 0 ,1.f };
			debugToNDCMatrix.push_back(lvNDCScale * lvTranslate * lvRotate * lvScale);
			debugBoxCollisionChecks.push_back(static_cast<int>(debugBoxData[i].isCollided));

		}
	}

}



void GraphicsPipe::funcDraw()
{
	if (!modelToNDCMatrix.empty())
	{
		glBindBuffer(GL_ARRAY_BUFFER, modelMatrixArrayBuffer);
		glNamedBufferData(modelMatrixArrayBuffer, modelToNDCMatrix.size() * sizeof(glm::mat3), &modelToNDCMatrix[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, textureOrderBuffer);
		glNamedBufferData(textureOrderBuffer, textureOrder.size() * sizeof(int), &textureOrder[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(genericShaderProgram);

		for (int i = 0; i < textureIDs.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE1+i);
			glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
		}

		GLint lvUniformVarLoc1 = glGetUniformLocation(genericShaderProgram, "textures");
	
		if (lvUniformVarLoc1 >= 0)
		{
			glUniform1iv(lvUniformVarLoc1, static_cast<GLsizei>(textureIDs.size()), (GLint*)&textureIDs[0]);
		}
		else
		{
			std::cout << lvUniformVarLoc1 << std::endl;
			std::cout << "Uniform variable doesn't exist!!!\n";
			std::exit(EXIT_FAILURE);
		}

		glBindVertexArray(squareMesh.vaoId);
		glDrawElementsInstanced(squareMesh.primitiveType, squareMesh.indexElementCount, GL_UNSIGNED_SHORT, NULL, static_cast<GLsizei>(modelToNDCMatrix.size()));
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		modelToNDCMatrix.clear();
		textureOrder.clear();
	}
	if (!modelData.empty())
	{
		modelData.clear();
	}
	
}

void GraphicsPipe::funcDrawDebug()
{
	if (!debugToNDCMatrix.empty())
	{
		glBindBuffer(GL_ARRAY_BUFFER, debugCollisionCheckBuffer);
		glNamedBufferData(debugCollisionCheckBuffer, debugBoxCollisionChecks.size() * sizeof(float), &debugBoxCollisionChecks[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(debugShaderProgram);

		glBindBuffer(GL_ARRAY_BUFFER, debugMatrixArrayBuffer);
		glNamedBufferData(debugMatrixArrayBuffer, debugToNDCMatrix.size() * sizeof(glm::mat3), &debugToNDCMatrix[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		

		

		glBindVertexArray(squareLinesMesh.vaoId);
		glDrawElementsInstanced(squareLinesMesh.primitiveType, squareLinesMesh.indexElementCount, GL_UNSIGNED_SHORT, NULL, static_cast<GLsizei>(debugToNDCMatrix.size()));
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		debugToNDCMatrix.clear();
		debugBoxCollisionChecks.clear();
	}
	if (!debugBoxData.empty())
	{
		debugBoxData.clear();
	}
}











GLuint GraphicsPipe::funcCompileShader(GLuint type, const std::string& shader)
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
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(lvID, length, &length, message);
		std::cout << "Failed to Compile Shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(lvID);
		std::exit;
		return 0;
	}

	return lvID;
}

unsigned int GraphicsPipe::funcSetupShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLuint lvProgram = glCreateProgram();
	GLuint lvVertexShaderID = funcCompileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint lvFragmentShaderID = funcCompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(lvProgram, lvVertexShaderID);
	glAttachShader(lvProgram, lvFragmentShaderID);
	glLinkProgram(lvProgram);
	glValidateProgram(lvProgram);

	glDeleteShader(lvVertexShaderID);
	glDeleteShader(lvFragmentShaderID);

	GLint success;
	glGetProgramiv(debugShaderProgram, GL_LINK_STATUS, &success);
	if (!success) 
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(debugShaderProgram, 512, NULL, infoLog);
		std::cout << "Error linking shader program:\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "Shader compiled successfully" << std::endl;
	}

	return lvProgram;
}


void GraphicsPipe::funcDeleteShader()
{
	glDeleteProgram(genericShaderProgram);
}


void GraphicsPipe::funcSetDrawMode(GLenum mode)
{
	glPolygonMode(GL_FRONT_AND_BACK, mode);
}



void GraphicsPipe::funcDrawWindow()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	funcDrawDebug();
	funcDraw();
	
	

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST); 
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(frameBufferShaderProgram);
	
	glBindVertexArray(screenMesh.vaoId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	int loc = glGetUniformLocation(frameBufferShaderProgram, "screenTexture");

	if (loc != -1)
	{
		glUniform1i(loc,0);
	}
	else
	{
		std::cout << "Uniform not found" << std::endl;
	}
	
	//glDrawArrays(GL_TRIANGLES, 0, 6);
}
