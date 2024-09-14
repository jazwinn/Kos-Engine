//#pragma once
//#include <glew.h>
//#include <glfw3.h>
//#include <glm.hpp>
//#include <gtc/type_ptr.hpp>
//#include <string>
//
//namespace Graphics
//{
//	class classGraphicsObject
//	{
//	private:
//		GLuint lvShapeType;
//		GLuint lvPrimitiveType;
//		GLuint lvVaoId;
//		GLuint lvVboId;
//		GLushort lvIndexElementCount;
//		GLuint lvShaderProgram;
//
//		struct ObjectParameters
//		{
//			GLfloat lvRotate;
//			glm::vec2 lvScale;
//			glm::vec2 lvTranslate;
//			glm::mat3 lvModelMatrix{ 1,0,0,0,1,0,0,0,1 };
//			glm::mat3 lvModelToNDCMatrix{ 1,0,0,0,1,0,0,0,1 };
//		};
//
//		ObjectParameters lvModelParams;
//
//
//		static GLuint funcCompileShader(GLuint type, const std::string& shader);
//
//	public:
//
//		enum
//		{
//			TRIANGLE,
//			RECTANGLE,
//			CIRCLE,
//			TRIANGLE_LINES,
//			RECTANGLE_LINES,
//			CIRCLE_LINES
//		};
//
//		//Constructor
//		classGraphicsObject(GLuint shape);
//		//destructor
//		~classGraphicsObject();
//
//		void funcSetupVao();
//		void funcSetupShader(const std::string& vertexShader, const std::string& fragmentShader);
//		void funcDeleteShader();
//
//		//
//		void funcAddScale(GLfloat x, GLfloat y);
//		void funcAddRotate(GLfloat angle);
//		void funcAddTranslate(GLfloat x, GLfloat y);
//
//
//		//Functions to set the value of the objects model parameters
//		void funcSetScale(GLfloat x, GLfloat y);
//		void funcSetRotate(GLfloat angle);
//		void funcSetTranslate(GLfloat x, GLfloat y);
//
//		//Functions to initialize, update and draw the object's graphics.
//		void funcInit(GLfloat xPos = 0, GLfloat yPos = 0, GLfloat xScale = 1, GLfloat yScale = 1, GLfloat angle = 0);
//		void funcUpdate();
//		void funcDraw();
//
//		GLuint funcGetShaderPgm() const;
//
//		static void funcSetDrawMode(GLenum mode);
//
//	};
//}

