#include "../Graphics/GraphicsPipe.h"
#include "../Assets/AssetManager.h"
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <gtc/type_ptr.hpp>



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
	
	squareMesh.shapeType = SQUARE;
	squareLinesMesh.shapeType = SQUARE_LINES;
	testMatrix = { 1,0,0,0,1,0,0,0,1 };
	modelData.reserve(2500);
	modelToNDCMatrix.reserve(2500);

	funcSetupVao(squareMesh);
	funcSetupVao(squareLinesMesh);
	funcSetDrawMode(GL_FILL);
	funcSetupShader(genericVertexShader, genericFragmentShader);

	modelToNDCMatrix.push_back(testMatrix);
	textureOrder.push_back(0);

	funcSetupArrayBuffer();
	funcBindImageDatafromAssetManager();

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	int windowWidth = mode->width;
	int windowHeight = mode->height;
	aspectRatio = static_cast<float>(static_cast<float>(windowHeight) / static_cast<float>(windowWidth));

	
	modelToNDCMatrix.clear();
	textureOrder.clear();

	/*glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(DebugCallback, nullptr);*/
}

GraphicsPipe::~GraphicsPipe() 
{
	//delete instancePtr;
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

	glNamedBufferStorage(lvVboId,
		position_data_size + color_data_size + texcoord_data_size,
		nullptr,
		GL_DYNAMIC_STORAGE_BIT);

	glNamedBufferSubData(lvVboId, position_data_offset, position_data_size, lvPosVtx.data());
	glNamedBufferSubData(lvVboId, color_data_offset, color_data_size, lvClrVtx.data());
	glNamedBufferSubData(lvVboId, texcoord_data_offset, texcoord_data_size, lvTexCoords.data());

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

	glEnableVertexArrayAttrib(shape.vaoId, 2);
	glVertexArrayVertexBuffer(shape.vaoId, 2, lvVboId, texcoord_data_offset, 
		texcoord_attribute_size);
	glVertexArrayAttribFormat(shape.vaoId, 2, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(shape.vaoId, 2, 2);

	if (shape.shapeType == SQUARE)
	{
		shape.primitiveType = GL_TRIANGLES;
		idx_vtx = { 0, 1, 2, 2, 3, 0 };
	}
	else if (shape.shapeType == SQUARE_LINES)
	{
		shape.primitiveType = GL_LINE_LOOP;
		idx_vtx = { 0, 1, 2, 3 };
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
}

void GraphicsPipe::funcBindImageDatafromAssetManager()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	AssetManager* assets = AssetManager::funcGetInstance();
	for (int i = 0; i < assets->imageContainer.size(); ++i)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, assets->imageContainer[i].width, assets->imageContainer[i].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, assets->imagedataArray[i]);
		glGenerateMipmap(GL_TEXTURE_2D);
		textureIDs.push_back(textureID);
		std::cout << "Texture Binded, Texture ID: " << textureID << std::endl;
	}
	
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
			glm::mat3 lvScale{ modelData[n].scale.x, 0, 0, 0, modelData[n].scale.y, 0, 0 , 0 ,1 };
			glm::mat3 lvRotate{ cos(modelData[n].rotate * 3.1415f / 180.f), sin(modelData[n].rotate * 3.1415f / 180.f), 0.f,
							   -sin(modelData[n].rotate * 3.1415f / 180.f), cos(modelData[n].rotate * 3.1415f / 180.f), 0.f,
							    0.f , 0.f ,1.f };
			glm::mat3 lvTranslate{ 1, 0, 0, 0, 1, 0, modelData[n].worldCoordinates.x , modelData[n].worldCoordinates.y ,1 };
			glm::mat3 lvNDCScale{ aspectRatio, 0, 0, 0, 1.f, 0, 0 , 0 ,1.f };
			modelToNDCMatrix.push_back(lvNDCScale * lvTranslate * lvRotate * lvScale);
			textureOrder.push_back(modelData[n].textureID);
			//textureOrder.push_back(1);
		}
	}

}



void GraphicsPipe::funcDraw(Mesh shape)
{

	if (!modelToNDCMatrix.empty())
	{
		glBindBuffer(GL_ARRAY_BUFFER, modelMatrixArrayBuffer);
		glNamedBufferData(modelMatrixArrayBuffer, modelToNDCMatrix.size() * sizeof(glm::mat3), &modelToNDCMatrix[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, textureOrderBuffer);
		glNamedBufferData(textureOrderBuffer, textureOrder.size() * sizeof(int), &textureOrder[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(genericShaderProgram);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textureIDs[1]);

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

		glBindVertexArray(shape.vaoId);
		glDrawElementsInstanced(shape.primitiveType, shape.indexElementCount, GL_UNSIGNED_SHORT, NULL, static_cast<GLsizei>(modelToNDCMatrix.size()));
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
		return 0;
	}

	return lvID;
}

void GraphicsPipe::funcSetupShader(const std::string& vertexShader, const std::string& fragmentShader)
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

	genericShaderProgram = lvProgram;

	GLint success;
	glGetProgramiv(genericShaderProgram, GL_LINK_STATUS, &success);
	if (!success) 
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(genericShaderProgram, 512, NULL, infoLog);
		std::cout << "Error linking shader program:\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "Shader compiled successfully" << std::endl;
	}
}

void GraphicsPipe::funcDeleteShader()
{
	glDeleteProgram(genericShaderProgram);
}


void GraphicsPipe::funcSetDrawMode(GLenum mode)
{
	glPolygonMode(GL_FRONT_AND_BACK, mode);
}