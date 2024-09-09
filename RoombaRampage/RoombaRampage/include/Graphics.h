#pragma once
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <string>


class GraphicsObject
{
private:
	GLuint shapeType;
	GLuint primitiveType;
	GLuint vaoid;
	GLuint vboid;
	GLushort indexElementCount;
	GLuint shaderProgram;
	
	struct ObjectParameters
	{
		GLfloat rotate;
		glm::vec2 scale;
		glm::vec2 translate;
		glm::mat3 modelMatrix{ 1,0,0,0,1,0,0,0,1 };
		glm::mat3 modelToNDCMatrix{ 1,0,0,0,1,0,0,0,1 };
	};
	
	ObjectParameters modelParams;
	

	static GLuint compile_shader(GLuint type, const std::string& shader);

public:

	enum 
	{
		TRIANGLE,
		RECTANGLE,
		CIRCLE,
		TRIANGLE_LINES,
		RECTANGLE_LINES,
		CIRCLE_LINES
	};

	//Constructor
	GraphicsObject(GLuint shape);

	void setup_vao();
	void setup_shader(const std::string& vertexShader, const std::string& fragmentShader);
	void delete_shader();

	//
	void add_scale(GLfloat x, GLfloat y);
	void add_rotate(GLfloat angle);
	void add_translate(GLfloat x, GLfloat y);


	//Functions to set the value of the objects model parameters
	void set_scale(GLfloat x, GLfloat y);
	void set_rotate(GLfloat angle);
	void set_translate(GLfloat x, GLfloat y);

	//Functions to initialize, update and draw the object's graphics.
	void init(GLfloat xPos = 0 , GLfloat yPos = 0, GLfloat xScale = 1, GLfloat yScale = 1, GLfloat angle = 0);
	void update();
	void draw();

	GLuint get_shader_pgm() const;

	static void set_draw_mode(GLenum mode);

};