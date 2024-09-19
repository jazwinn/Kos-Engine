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
	unsigned int textureID;
	//glm::vec4 color;
};

class GraphicsPipe
{
private:

	struct Mesh
	{
		int shapeType;
		unsigned int vaoId;
		unsigned int primitiveType;
		unsigned short indexElementCount;

	};

	void funcSetupVao(Mesh& shape);
	void funcSetupArrayBuffer();
	void funcBindImageDatafromAssetManager();
	void funcSetupShader(const std::string& vertexShader, const std::string& fragmentShader);
	void funcDeleteShader();

	static GraphicsPipe* instancePtr;

	unsigned int genericShaderProgram;

	//Buffers
	unsigned int modelMatrixArrayBuffer;
	unsigned int textureOrderBuffer;

	float aspectRatio;

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

	


	~GraphicsPipe();

	static GraphicsPipe* funcGetInstance();

	

	void funcInit();
	void funcUpdate();
	void funcDraw(Mesh shape);

	void funcSortDrawOrder();
	static void funcSetDrawMode(GLenum mode);

	Mesh squareMesh;
	Mesh squareLinesMesh;
	std::vector<GraphicsData> modelData;
	std::vector<int> textureOrder;

	//Array of the texture IDs
	std::vector<unsigned int> textureIDs;
	

};


#endif GRAPHPIPE_H