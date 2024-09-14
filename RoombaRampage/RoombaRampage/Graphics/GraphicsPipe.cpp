#include "../Graphics/GraphicsPipe.h"
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

GraphicsPipe* GraphicsPipe::instancePtr = new GraphicsPipe{};

//Class Constructor with Parameter
//GraphicsPipe::GraphicsPipe()
//{
//	/*squareMesh.shapeType = SQUARE;
//	squareLinesMesh.shapeType = SQUARE_LINES;
//
//	funcSetupVao(squareMesh);
//	funcSetupVao(squareLinesMesh);
//	funcSetDrawMode(GL_FILL);
//	funcSetupShader(genericVertexShader, genericFragmentShader);*/
//
//	//modelToNDCMatrix.push_back({ 1,0,0,0,1,0,0,0,1 });
//
//}

GraphicsPipe::~GraphicsPipe() {
	delete instancePtr;
	funcDeleteShader();
}

GraphicsPipe* GraphicsPipe::funcGetInstance()
{
	return instancePtr;
}

void GraphicsPipe::funcSetupVao(Mesh &shape)
{
	std::vector<glm::vec2> lvPosVtx;
	std::vector<glm::vec3> lvClrVtx;
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
	}


	GLsizei position_data_offset = 0;
	GLsizei position_attribute_size = sizeof(glm::vec2);
	GLsizei position_data_size = position_attribute_size * static_cast<GLsizei>(lvPosVtx.size());
	GLsizei color_data_offset = position_data_size;
	GLsizei color_attribute_size = sizeof(glm::vec3);
	GLsizei color_data_size = color_attribute_size * static_cast<GLsizei>(lvClrVtx.size());

	unsigned int lvVboId{};

	glCreateBuffers(1, &lvVboId);

	glNamedBufferStorage(lvVboId,
		position_data_size + color_data_size,
		nullptr,
		GL_DYNAMIC_STORAGE_BIT);

	glNamedBufferSubData(lvVboId, position_data_offset, position_data_size, lvPosVtx.data());
	glNamedBufferSubData(lvVboId, color_data_offset, color_data_size, lvClrVtx.data());

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

void GraphicsPipe::funcInit()
{
	squareMesh.shapeType = SQUARE;
	squareLinesMesh.shapeType = SQUARE_LINES;

	funcSetupVao(squareMesh);
	funcSetupVao(squareLinesMesh);
	funcSetDrawMode(GL_FILL);
	funcSetupShader(genericVertexShader, genericFragmentShader);

	testMatrix = { 1,0,0,0,1,0,0,0,1 };
}

void GraphicsPipe::funcUpdate()
{
	if (modelData.size() > 0) {
		for (int n{}; n < modelData.size(); n++) {

			glm::mat3 lvScale{ modelData[n].scale.x, 0, 0, 0, modelData[n].scale.y, 0, 0 , 0 ,1 };
			glm::mat3 lvRotate{ cos(modelData[n].rotate * 3.1415f / 180.f), sin(modelData[n].rotate * 3.1415f / 180.f), 0.f,
							-sin(modelData[n].rotate * 3.1415f / 180.f), cos(modelData[n].rotate * 3.1415f / 180.f), 0.f,
							0.f , 0.f ,1.f };
			glm::mat3 lvTranslate{ 1, 0, 0, 0, 1, 0, modelData[n].worldCoordinates.x , modelData[n].worldCoordinates.y ,1 };
			glm::mat3 lvNDCScale{ 1.f, 0, 0, 0, 1.f, 0, 0 , 0 ,1 };
			modelToNDCMatrix.push_back(lvNDCScale * lvTranslate * lvRotate * lvScale);
		}
	}

}



void GraphicsPipe::funcDraw(Mesh shape)
{

	if (!modelToNDCMatrix.empty())
	{
		glUseProgram(genericShaderProgram);

		GLint lvUniformVarLoc1 =
			glGetUniformLocation(genericShaderProgram,
				"modelToNDCMatrix");
		if (lvUniformVarLoc1 >= 0)
		{
			glUniformMatrix3fv(lvUniformVarLoc1, 1, GL_FALSE,
				glm::value_ptr(modelToNDCMatrix[0]));
		}
		else
		{
			std::cout << "Uniform variable doesn't exist!!!\n";
			std::exit(EXIT_FAILURE);
		}

		glBindVertexArray(shape.vaoId);
		glDrawElements(shape.primitiveType, shape.indexElementCount, GL_UNSIGNED_SHORT, NULL);
		glBindVertexArray(0);
		modelToNDCMatrix.clear();
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

	std::cout << "Shader compiled successfully" << std::endl;
}

void GraphicsPipe::funcDeleteShader()
{
	glDeleteProgram(genericShaderProgram);
}


void GraphicsPipe::funcSetDrawMode(GLenum mode)
{
	glPolygonMode(GL_FRONT_AND_BACK, mode);
}