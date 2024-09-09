#include "Graphics.h"
#include <iostream>
#include <vector>
#include <array>


namespace Graphics
{
	//Class Constructor with Parameter
	classGraphicsObject::classGraphicsObject(GLuint shape) : lvShapeType(shape), lvPrimitiveType(0), lvVaoId(0), lvVboId(0), lvIndexElementCount(0), lvShaderProgram(0),
		lvModelParams() {}

	void classGraphicsObject::funcSetupVao()
	{

		std::vector<glm::vec2> lvPosVtx;
		std::vector<glm::vec3> lvClrVtx;
		std::vector<GLushort>idx_vtx;

		if (lvShapeType == RECTANGLE || RECTANGLE_LINES)
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

		glCreateBuffers(1, &lvVboId);

		glNamedBufferStorage(lvVboId,
			position_data_size + color_data_size,
			nullptr,
			GL_DYNAMIC_STORAGE_BIT);

		glNamedBufferSubData(lvVboId, position_data_offset, position_data_size, lvPosVtx.data());
		glNamedBufferSubData(lvVboId, color_data_offset, color_data_size, lvClrVtx.data());

		glCreateVertexArrays(1, &lvVaoId);
		glEnableVertexArrayAttrib(lvVaoId, 0);
		glVertexArrayVertexBuffer(lvVaoId, 0, lvVboId,
			position_data_offset, position_attribute_size);
		glVertexArrayAttribFormat(lvVaoId, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(lvVaoId, 0, 0);


		glEnableVertexArrayAttrib(lvVaoId, 1);
		glVertexArrayVertexBuffer(lvVaoId, 1, lvVboId,
			color_data_offset, color_attribute_size);
		glVertexArrayAttribFormat(lvVaoId, 1, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(lvVaoId, 1, 1);

		if (lvShapeType == RECTANGLE)
		{
			lvPrimitiveType = GL_TRIANGLES;
			idx_vtx = { 0, 1, 2, 2, 3, 0 };
		}
		else if (lvShapeType == RECTANGLE_LINES)
		{
			lvPrimitiveType = GL_LINE_LOOP;
			idx_vtx = { 0, 1, 2, 3 };
		}

		lvIndexElementCount = static_cast<GLuint>(idx_vtx.size());
		GLuint ebo_hdl;
		glCreateBuffers(1, &ebo_hdl);
		glNamedBufferStorage(ebo_hdl, sizeof(GLushort) * lvIndexElementCount,
			reinterpret_cast<GLvoid*>(idx_vtx.data()),
			GL_DYNAMIC_STORAGE_BIT);
		glVertexArrayElementBuffer(lvVaoId, ebo_hdl);
		glBindVertexArray(0);
	}

	void classGraphicsObject::funcInit(GLfloat xPos, GLfloat yPos, GLfloat xScale, GLfloat yScale, GLfloat angle)
	{
		lvModelParams.lvTranslate = { xPos , yPos };
		lvModelParams.lvScale = { xScale, yScale };
		lvModelParams.lvRotate = angle;

		glm::mat3 lvScale{ lvModelParams.lvScale[0], 0, 0, 0, lvModelParams.lvScale[1], 0, 0 , 0 ,1 };
		glm::mat3 lvRotate{ cos(lvModelParams.lvRotate * 3.1415f / 180.f), sin(lvModelParams.lvRotate * 3.1415f / 180.f), 0.f,
						-sin(lvModelParams.lvRotate * 3.1415f / 180.f), cos(lvModelParams.lvRotate * 3.1415f / 180.f), 0.f,
						0.f , 0.f ,1.f };
		glm::mat3 lvTranslate{ 1, 0, 0, 0, 1, 0, lvModelParams.lvTranslate[0] , lvModelParams.lvTranslate[1] ,1 };
		glm::mat3 lvNDCScale{ 1.f, 0, 0, 0, 1.f, 0, 0 , 0 ,1 };
		lvModelParams.lvModelToNDCMatrix = lvNDCScale * lvTranslate * lvRotate * lvScale;
	}

	void classGraphicsObject::funcUpdate()
	{
		glm::mat3 lvScale{ lvModelParams.lvScale[0], 0, 0, 0, lvModelParams.lvScale[1], 0, 0 , 0 ,1 };
		glm::mat3 lvRotate{ cos(lvModelParams.lvRotate * 3.1415f / 180.f), sin(lvModelParams.lvRotate * 3.1415f / 180.f), 0.f,
						-sin(lvModelParams.lvRotate * 3.1415f / 180.f), cos(lvModelParams.lvRotate * 3.1415f / 180.f), 0.f,
						0.f , 0.f ,1.f };
		glm::mat3 lvTranslate{ 1, 0, 0, 0, 1, 0, lvModelParams.lvTranslate[0] , lvModelParams.lvTranslate[1] ,1 };
		glm::mat3 lvNDCScale{ 1.f, 0, 0, 0, 1.f, 0, 0 , 0 ,1 };
		lvModelParams.lvModelToNDCMatrix = lvNDCScale * lvTranslate * lvRotate * lvScale;
	}

	void classGraphicsObject::funcDraw()
	{

		glUseProgram(funcGetShaderPgm());

		GLint lvUniformVarLoc1 =
			glGetUniformLocation(lvShaderProgram,
				"modelToNDCMatrix");
		if (lvUniformVarLoc1 >= 0)
		{
			glUniformMatrix3fv(lvUniformVarLoc1, 1, GL_FALSE,
				glm::value_ptr(lvModelParams.lvModelToNDCMatrix));
		}
		else
		{
			std::cout << "Uniform variable doesn't exist!!!\n";
			std::exit(EXIT_FAILURE);
		}

		glBindVertexArray(lvVaoId);
		glDrawElements(lvPrimitiveType, lvIndexElementCount, GL_UNSIGNED_SHORT, NULL);
		glBindVertexArray(0);
	}

	GLuint classGraphicsObject::funcCompileShader(GLuint type, const std::string& shader)
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

	void classGraphicsObject::funcSetupShader(const std::string& vertexShader, const std::string& fragmentShader)
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

		lvShaderProgram = lvProgram;

		std::cout << "Shader compiled successfully" << std::endl;
	}

	void classGraphicsObject::funcDeleteShader()
	{
		glDeleteProgram(funcGetShaderPgm());
	}

	void classGraphicsObject::funcAddScale(GLfloat x, GLfloat y)
	{
		lvModelParams.lvScale.x += x;
		lvModelParams.lvScale.y += y;
	}

	void classGraphicsObject::funcAddRotate(GLfloat angle)
	{
		lvModelParams.lvRotate += angle;
	}

	void classGraphicsObject::funcAddTranslate(GLfloat x, GLfloat y)
	{
		lvModelParams.lvTranslate.x += x;
		lvModelParams.lvTranslate.y += y;
	}



	void classGraphicsObject::funcSetScale(GLfloat x, GLfloat y)
	{
		lvModelParams.lvScale = { x, y };
	}

	void classGraphicsObject::funcSetRotate(GLfloat angle)
	{
		lvModelParams.lvRotate = angle;
	}

	void classGraphicsObject::funcSetTranslate(GLfloat x, GLfloat y)
	{
		lvModelParams.lvTranslate = { x, y };
	}


	GLuint classGraphicsObject::funcGetShaderPgm() const
	{
		return lvShaderProgram;
	}

	void classGraphicsObject::funcSetDrawMode(GLenum mode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, mode);
	}
}
