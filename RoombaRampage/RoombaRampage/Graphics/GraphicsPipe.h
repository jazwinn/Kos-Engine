#ifndef GRAPHPIPE_H
#define GRAPHPIPE_H

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <vector>

struct GraphicsData
{
	float rotate;
	glm::vec2 scale;
	glm::vec3 worldCoordinates;
	//glm::vec4 color;
	//std::string textureFile;
};

class GraphicsPipe
{
private:

	static GraphicsPipe* instancePtr;

	unsigned int genericShaderProgram;

	

	struct Camera
	{
		glm::vec2 coordinates;
		float angle;
	};


	std::vector<glm::mat3> modelToNDCMatrix;
	glm::mat3 testMatrix;
	


	static GLuint funcCompileShader(GLuint type, const std::string& shader);

public:

	enum
	{
		SQUARE,
		CIRCLE,
		SQUARE_LINES,
		CIRCLE_LINES
	};

	struct Mesh
	{
		int shapeType;
		unsigned int vaoId;
		unsigned int primitiveType;
		unsigned short indexElementCount;

	};

	Mesh squareMesh;
	Mesh squareLinesMesh;

	//GraphicsPipe();

	~GraphicsPipe();

	static GraphicsPipe* funcGetInstance();

	void funcSetupVao(Mesh &shape);
	void funcSetupShader(const std::string& vertexShader, const std::string& fragmentShader);
	void funcDeleteShader();

	void funcSortDrawOrder();

	void funcInit();
	void funcUpdate();
	void funcDraw(Mesh shape);

	static void funcSetDrawMode(GLenum mode);

	std::vector<GraphicsData> modelData;

};


#endif GRAPHPIPE_H