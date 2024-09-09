#include "Graphics.h"
#include <iostream>
#include <vector>
#include <array>

//Class Constructor with Parameter
GraphicsObject::GraphicsObject(GLuint shape) : shapeType(shape), primitiveType(0), vaoid(0), vboid(0), indexElementCount(0), shaderProgram(0), 
							   modelParams() {}

void GraphicsObject::setup_vao()
{

	std::vector<glm::vec2> pos_vtx;
	std::vector<glm::vec3> clr_vtx;
	std::vector<GLushort>idx_vtx;

	if (shapeType == RECTANGLE || RECTANGLE_LINES)
	{
		pos_vtx = { glm::vec2(0.5f, -0.5f),
					glm::vec2(0.5f, 0.5f),
					glm::vec2(-0.5f, 0.5f),
					glm::vec2(-0.5f, -0.5f) };
		clr_vtx = { glm::vec3(1.f, 1.f, 1.f),
					glm::vec3(1.f, 1.f, 1.f),
					glm::vec3(1.f, 1.f, 1.f),
					glm::vec3(1.f, 1.f, 1.f) };
	}


	GLsizei position_data_offset = 0;
	GLsizei position_attribute_size = sizeof(glm::vec2);
	GLsizei position_data_size = position_attribute_size * static_cast<GLsizei>(pos_vtx.size());
	GLsizei color_data_offset = position_data_size;
	GLsizei color_attribute_size = sizeof(glm::vec3);
	GLsizei color_data_size = color_attribute_size * static_cast<GLsizei>(clr_vtx.size());

	glCreateBuffers(1, &vboid);

	glNamedBufferStorage(vboid,
		position_data_size + color_data_size,
		nullptr,
		GL_DYNAMIC_STORAGE_BIT);

	glNamedBufferSubData(vboid, position_data_offset, position_data_size, pos_vtx.data());
	glNamedBufferSubData(vboid, color_data_offset, color_data_size, clr_vtx.data());

	glCreateVertexArrays(1, &vaoid);
	glEnableVertexArrayAttrib(vaoid, 0);
	glVertexArrayVertexBuffer(vaoid, 0, vboid,
		position_data_offset, position_attribute_size);
	glVertexArrayAttribFormat(vaoid, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoid, 0, 0);


	glEnableVertexArrayAttrib(vaoid, 1);
	glVertexArrayVertexBuffer(vaoid, 1, vboid,
		color_data_offset, color_attribute_size);
	glVertexArrayAttribFormat(vaoid, 1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoid, 1, 1);

	if (shapeType == RECTANGLE)
	{
		primitiveType = GL_TRIANGLES;
		idx_vtx = { 0, 1, 2, 2, 3, 0 };
	}
	else if (shapeType == RECTANGLE_LINES)
	{
		primitiveType = GL_LINE_LOOP;
		idx_vtx = { 0, 1, 2, 3 };
	}

	indexElementCount = static_cast<GLuint>(idx_vtx.size());
	GLuint ebo_hdl;
	glCreateBuffers(1, &ebo_hdl);
	glNamedBufferStorage(ebo_hdl, sizeof(GLushort) * indexElementCount,
		reinterpret_cast<GLvoid*>(idx_vtx.data()),
		GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(vaoid, ebo_hdl);
	glBindVertexArray(0);
}

void GraphicsObject::init(GLfloat xPos, GLfloat yPos, GLfloat xScale, GLfloat yScale, GLfloat angle)
{
	modelParams.translate = { xPos , yPos };
	modelParams.scale = { xScale, yScale };
	modelParams.rotate = angle;

	glm::mat3 scale{ modelParams.scale[0], 0, 0, 0, modelParams.scale[1], 0, 0 , 0 ,1 };
	glm::mat3 rotate{ cos(modelParams.rotate * 3.1415f / 180.f), sin(modelParams.rotate * 3.1415f / 180.f), 0.f,
					-sin(modelParams.rotate * 3.1415f / 180.f), cos(modelParams.rotate * 3.1415f / 180.f), 0.f,
					0.f , 0.f ,1.f };
	glm::mat3 translate{ 1, 0, 0, 0, 1, 0, modelParams.translate[0] , modelParams.translate[1] ,1 };
	glm::mat3 ndcscale{ 1.f, 0, 0, 0, 1.f, 0, 0 , 0 ,1 };
	modelParams.modelToNDCMatrix = ndcscale * translate * rotate * scale;
}

void GraphicsObject::update()
{
	glm::mat3 scale{ modelParams.scale[0], 0, 0, 0, modelParams.scale[1], 0, 0 , 0 ,1 };
	glm::mat3 rotate{ cos(modelParams.rotate * 3.1415f / 180.f), sin(modelParams.rotate * 3.1415f / 180.f), 0.f,
					-sin(modelParams.rotate * 3.1415f / 180.f), cos(modelParams.rotate * 3.1415f / 180.f), 0.f,
					0.f , 0.f ,1.f };
	glm::mat3 translate{ 1, 0, 0, 0, 1, 0, modelParams.translate[0] , modelParams.translate[1] ,1 };
	glm::mat3 ndcscale{ 1.f, 0, 0, 0, 1.f, 0, 0 , 0 ,1 };
	modelParams.modelToNDCMatrix = ndcscale * translate * rotate * scale;
}

void GraphicsObject::draw()
{

	glUseProgram(get_shader_pgm());

	GLint uniform_var_loc1 =
		glGetUniformLocation(shaderProgram,
			"modelToNDCMatrix");
	if (uniform_var_loc1 >= 0) 
	{
		glUniformMatrix3fv(uniform_var_loc1, 1, GL_FALSE,
			glm::value_ptr(modelParams.modelToNDCMatrix));
	}
	else 
	{
		std::cout << "Uniform variable doesn't exist!!!\n";
		std::exit(EXIT_FAILURE);
	}

	glBindVertexArray(vaoid);
	glDrawElements(primitiveType, indexElementCount, GL_UNSIGNED_SHORT, NULL);
	glBindVertexArray(0);
}

GLuint GraphicsObject::compile_shader(GLuint type, const std::string& shader)
{
	GLuint id = glCreateShader(type);
	const char* source = shader.c_str();
	glShaderSource(id, 1, &source, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to Compile Shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

void GraphicsObject::setup_shader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLuint program = glCreateProgram();
	GLuint vertexShaderID = compile_shader(GL_VERTEX_SHADER, vertexShader);
	GLuint fragmentShaderID = compile_shader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vertexShaderID);
	glAttachShader(program, fragmentShaderID);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	
	shaderProgram = program;

	std::cout << "Shader compiled successfully" << std::endl;
}

void GraphicsObject::delete_shader()
{
	glDeleteProgram(get_shader_pgm());
}

void GraphicsObject::add_scale(GLfloat x, GLfloat y)
{
	modelParams.scale.x += x;
	modelParams.scale.y += y;
}

void GraphicsObject::add_rotate(GLfloat angle)
{
	modelParams.rotate += angle;
}

void GraphicsObject::add_translate(GLfloat x, GLfloat y)
{
	modelParams.translate.x += x;
	modelParams.translate.y += y;
}



void GraphicsObject::set_scale(GLfloat x, GLfloat y)
{
	modelParams.scale = { x, y };
}

void GraphicsObject::set_rotate(GLfloat angle)
{
	modelParams.rotate = angle;
}

void GraphicsObject::set_translate(GLfloat x, GLfloat y)
{
	modelParams.translate = { x, y };
}


GLuint GraphicsObject::get_shader_pgm() const
{
	return shaderProgram;
}

void GraphicsObject::set_draw_mode(GLenum mode)
{
	glPolygonMode(GL_FRONT_AND_BACK, mode);
}