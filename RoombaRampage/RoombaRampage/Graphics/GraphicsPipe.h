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
	unsigned short textureLayer;
	//glm::vec4 color;
};

class GraphicsPipe
{
private:

	static GraphicsPipe* instancePtr;

	unsigned int genericShaderProgram;
	unsigned int modelMatrixArrayBuffer;
	unsigned int textureArrayBuffer;

	float aspectRatio;

	struct Camera
	{
		glm::vec2 coordinates;
		float angle;
	};

	std::vector<std::string> textureContainer;
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


	~GraphicsPipe();

	static GraphicsPipe* funcGetInstance();

	void funcSetupVao(Mesh &shape);
	void funcSetupArrayBuffer();
	void funcSetupTextureArray();
	void funcSetupShader(const std::string& vertexShader, const std::string& fragmentShader);
	void funcDeleteShader();

	void funcInit();
	void funcUpdate();
	void funcDraw(Mesh shape);

	void funcSortDrawOrder();
	static void funcSetDrawMode(GLenum mode);


	Mesh squareMesh;
	Mesh squareLinesMesh;
	std::vector<GraphicsData> modelData;
	

};


#endif GRAPHPIPE_H