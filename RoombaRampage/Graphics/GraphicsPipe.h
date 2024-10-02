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

namespace graphicpipe {

	//SPLIT TO DIFFERENT FILES!!!!
	struct GraphicsData
	{
		float m_rotate{};
		glm::vec2 m_scale{};
		glm::vec3 m_worldCoordinates{};
		unsigned int m_textureID{};
		int m_shapeType{};
		int m_layer{};
	};

	struct DebugDrawData
	{
		float m_rotate{};
		glm::vec2 m_scale{};
		glm::vec3 m_worldCoordinates{};
		bool m_isCollided{};
		int m_shapeType{};
	};

	struct TextData
	{
		std::string m_text;
		float m_x; 
		float m_y;
		float m_scale;
		glm::vec3 m_color;
	};

	class GraphicsPipe
	{
	private:

		int m_unitWidth{ 512 };
		int m_unitHeight{ 512 };

		struct Mesh
		{
			int m_shapeType{};
			unsigned int m_vaoId{};
			unsigned int m_primitiveType;
			unsigned short m_indexElementCount;

		};

		void m_funcSetupTextVao();
		void m_funcSetupVao(Mesh& shape);
		void m_funcSetupFboVao();
		void m_funcSetupSquareLinesVao();
		void m_funcSetupArrayBuffer();
		void m_funcSetupFrameBuffer();
		unsigned int m_funcSetupShader(const std::string& vertexShader, const std::string& fragmentShader);
		void m_funcDeleteShader();

		static std::unique_ptr<GraphicsPipe> m_instancePtr;

		//Shader Programs
		unsigned int m_genericShaderProgram{};
		unsigned int m_frameBufferShaderProgram{};
		unsigned int m_debugShaderProgram{};
		unsigned int m_textShaderProgram{};

		//Buffers
		unsigned int m_modelMatrixArrayBuffer{};
		unsigned int m_debugMatrixArrayBuffer{};
		unsigned int m_textureOrderBuffer{};
		unsigned int m_debugCollisionCheckBuffer{};
		unsigned int m_frameBufferObject{};
		unsigned int m_textBuffer{};


		int m_windowWidth{};
		int m_windowHeight{};
		float m_aspectRatio{};

		struct Camera
		{
			glm::vec2 m_coordinates;
			float m_angle;
		};

		std::vector<glm::mat3> m_modelToNDCMatrix{};
		std::vector<glm::mat3> m_debugToNDCMatrix{};
		glm::mat3 m_testMatrix{};


		static GLuint m_funcCompileShader(GLuint type, const std::string& shader);

	public:

		enum
		{
			SQUARE,
			CIRCLE,
			SQUARE_LINES,
			CIRCLE_LINES
		};




		~GraphicsPipe();

		static GraphicsPipe* m_funcGetInstance();



		void m_funcInit();
		void m_funcUpdate();

		void m_funcDrawWindow();
		void m_funcDraw();
		void m_funcDrawDebug();

		void m_funcSortDrawOrder();
		static void m_funcSetDrawMode(GLenum mode);
		void m_funcDrawText();

		Mesh m_squareMesh;
		Mesh m_screenMesh;
		Mesh m_squareLinesMesh;
		Mesh m_textMesh;
		std::vector<GraphicsData> m_modelData{};
		std::vector<DebugDrawData> m_debugBoxData{};
		std::vector<TextData> m_textData{};
		std::vector<float> m_debugBoxCollisionChecks{};
		std::vector<int> m_textureOrder{};


		//Array of the texture IDs
		std::vector<unsigned int> m_textureIDs{};
		std::vector<image::Image> m_imageData{};

		unsigned int m_screenTexture{};
		unsigned int m_proxyBackgroundTexture{};
	};
}


#endif GRAPHPIPE_H