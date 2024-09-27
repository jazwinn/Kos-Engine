#ifndef GRAPHPIPE_H
#define GRAPHPIPE_H

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <memory>
#include "../Assets/AssetManager.h"

struct GraphicsData
{
	float rotate;
	glm::vec2 scale;
	glm::vec3 worldCoordinates;
	unsigned int textureID;
	bool debug;
	//glm::vec4 color;
};

class GraphicsPipe
{
private:

	int unitWidth{ 512 };
	int unitHeight{ 512 };

	struct Mesh
	{
		int shapeType;
		unsigned int vaoId;
		unsigned int primitiveType;
		unsigned short indexElementCount;

	};

	void funcSetupVao(Mesh& shape);
	void funcSetupFboVao();
	void funcSetupArrayBuffer();
	void funcSetupFrameBuffer();
	void funcBindImageDatafromAssetManager();
	unsigned int funcSetupShader(const std::string& vertexShader, const std::string& fragmentShader);
	void funcDeleteShader();

	static std::unique_ptr<GraphicsPipe> instancePtr;

	//Shader Programs
	unsigned int genericShaderProgram;
	unsigned int frameBufferShaderProgram;

	//Buffers
	unsigned int modelMatrixArrayBuffer;
	unsigned int textureOrderBuffer;
	unsigned int debugOrderBuffer;
	unsigned int frameBufferObject;
	

	int windowWidth;
	int windowHeight;
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

	void funcDrawWindow();
	void funcDraw();
	void funcDrawDebug();


	void funcSortDrawOrder();
	static void funcSetDrawMode(GLenum mode);

	Mesh squareMesh;
	Mesh screenMesh;
	Mesh squareLinesMesh;
	std::vector<GraphicsData> modelData;
	std::vector<int> textureOrder;
	std::vector<int> debugDrawOrder;

	//Array of the texture IDs
	std::vector<unsigned int> textureIDs;
	std::vector<AssetManager::Image> imageData;

	unsigned int screenTexture;
	unsigned int proxyBackgroundTexture;
};


#endif GRAPHPIPE_H