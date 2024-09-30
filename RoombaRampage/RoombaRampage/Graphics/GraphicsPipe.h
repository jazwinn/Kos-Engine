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
	int shapeType;
	int layer;
};

struct DebugDrawData
{
	float rotate;
	glm::vec2 scale;
	glm::vec3 worldCoordinates;
	bool isCollided;
	int shapeType;
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
	void funcSetupSquareLinesVao();
	void funcSetupArrayBuffer();
	void funcSetupFrameBuffer();
	void funcBindImageDatafromAssetManager();
	unsigned int funcSetupShader(const std::string& vertexShader, const std::string& fragmentShader);
	void funcDeleteShader();

	static std::unique_ptr<GraphicsPipe> instancePtr;

	//Shader Programs
	unsigned int genericShaderProgram;
	unsigned int frameBufferShaderProgram;
	unsigned int debugShaderProgram;

	//Buffers
	unsigned int modelMatrixArrayBuffer;
	unsigned int debugMatrixArrayBuffer;
	unsigned int textureOrderBuffer;
	unsigned int debugCollisionCheckBuffer;
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
	std::vector<glm::mat3> debugToNDCMatrix;
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
	std::vector<DebugDrawData> debugBoxData;
	std::vector<float> debugBoxCollisionChecks;
	std::vector<int> textureOrder;
	

	//Array of the texture IDs
	std::vector<unsigned int> textureIDs;
	std::vector<AssetManager::Image> imageData;

	unsigned int screenTexture;
	unsigned int proxyBackgroundTexture;
};


#endif GRAPHPIPE_H