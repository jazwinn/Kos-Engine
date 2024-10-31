#include "../Graphics/GraphicsPipe.h"
namespace graphicpipe
{
	void GraphicsPipe::m_funcSetupSquareLinesVao()
	{
		std::vector<glm::vec2> lvPosVtx;
		std::vector<GLushort>idx_vtx;

		lvPosVtx = { glm::vec2(0.5f, -0.5f),
					glm::vec2(0.5f, 0.5f),
					glm::vec2(-0.5f, 0.5f),
					glm::vec2(-0.5f, -0.5f) };



		GLsizei position_data_offset = 0;
		GLsizei position_attribute_size = sizeof(glm::vec2);
		GLsizei position_data_size = position_attribute_size * static_cast<GLsizei>(lvPosVtx.size());


		unsigned int lvVboId{};

		glCreateBuffers(1, &lvVboId);


		glNamedBufferStorage(lvVboId, position_data_size, nullptr, GL_DYNAMIC_STORAGE_BIT);
		glNamedBufferSubData(lvVboId, position_data_offset, position_data_size, lvPosVtx.data());

		glCreateVertexArrays(1, &m_squareLinesMesh.m_vaoId);
		glEnableVertexArrayAttrib(m_squareLinesMesh.m_vaoId, 0);
		glVertexArrayVertexBuffer(m_squareLinesMesh.m_vaoId, 0, lvVboId,
			position_data_offset, position_attribute_size);
		glVertexArrayAttribFormat(m_squareLinesMesh.m_vaoId, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(m_squareLinesMesh.m_vaoId, 0, 0);

		m_squareLinesMesh.m_primitiveType = GL_LINE_LOOP;
		idx_vtx = { 0, 1, 2, 3 };


		m_squareLinesMesh.m_indexElementCount = static_cast<unsigned short>(idx_vtx.size());
		unsigned int ebo_hdl;
		glCreateBuffers(1, &ebo_hdl);
		glNamedBufferStorage(ebo_hdl, sizeof(unsigned short) * m_squareLinesMesh.m_indexElementCount,
			reinterpret_cast<GLvoid*>(idx_vtx.data()),
			GL_DYNAMIC_STORAGE_BIT);
		glVertexArrayElementBuffer(m_squareLinesMesh.m_vaoId, ebo_hdl);
		glBindVertexArray(0);
	}

	void GraphicsPipe::m_funcSetupCircleLinesVao()
	{
		std::vector<glm::vec2> lvPosVtx;
		std::vector<GLushort> idx_vtx;

		int numVertices = 36; // For example, 36 vertices
		float radius = 0.5f;  // Circle radius

		// Generate circle vertices
		for (int i = 0; i < numVertices; ++i) {
			float angle = 2.0f * 3.1415f * i / numVertices; // Calculate angle for each vertex
			float x = radius * cos(angle);
			float y = radius * sin(angle);
			lvPosVtx.push_back(glm::vec2(x, y));
			idx_vtx.push_back(static_cast<GLushort>(i));
		}

		GLsizei position_data_offset = 0;
		GLsizei position_attribute_size = sizeof(glm::vec2);
		GLsizei position_data_size = position_attribute_size * static_cast<GLsizei>(lvPosVtx.size());

		unsigned int lvVboId{};
		glCreateBuffers(1, &lvVboId);

		// Allocate storage for vertex buffer and fill with circular vertices
		glNamedBufferStorage(lvVboId, position_data_size, nullptr, GL_DYNAMIC_STORAGE_BIT);
		glNamedBufferSubData(lvVboId, position_data_offset, position_data_size, lvPosVtx.data());

		glCreateVertexArrays(1, &m_circleLinesMesh.m_vaoId);
		glEnableVertexArrayAttrib(m_circleLinesMesh.m_vaoId, 0);
		glVertexArrayVertexBuffer(m_circleLinesMesh.m_vaoId, 0, lvVboId, position_data_offset, position_attribute_size);
		glVertexArrayAttribFormat(m_circleLinesMesh.m_vaoId, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(m_circleLinesMesh.m_vaoId, 0, 0);

		m_circleLinesMesh.m_primitiveType = GL_LINE_LOOP;
		m_circleLinesMesh.m_indexElementCount = static_cast<unsigned short>(idx_vtx.size());

		unsigned int ebo_hdl;
		glCreateBuffers(1, &ebo_hdl);
		glNamedBufferStorage(ebo_hdl, sizeof(unsigned short) * m_circleLinesMesh.m_indexElementCount,
			idx_vtx.data(), GL_DYNAMIC_STORAGE_BIT);
		glVertexArrayElementBuffer(m_circleLinesMesh.m_vaoId, ebo_hdl);

		glBindVertexArray(0);
	}

	void GraphicsPipe::m_funcSetupFboVao()
	{
		float quadVertices[] =
		{
			// Positions        // Texture Coords
			-1.0f,  1.0f,       0.0f, 1.0f,  // Top-left
			-1.0f, -1.0f,       0.0f, 0.0f,  // Bottom-left
			 1.0f, -1.0f,       1.0f, 0.0f,  // Bottom-right
			 1.0f, -1.0f,       1.0f, 0.0f,  // Bottom-right
			 1.0f,  1.0f,       1.0f, 1.0f,  // Top-right
			-1.0f,  1.0f,       0.0f, 1.0f   // Top-left
		};

		unsigned int vboID;
		glGenVertexArrays(1, &m_screenMesh.m_vaoId);
		glGenBuffers(1, &vboID);

		glBindVertexArray(m_screenMesh.m_vaoId);

		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	void GraphicsPipe::m_funcSetupTextVao()
	{

		glGenVertexArrays(1, &m_textMesh.m_vaoId);
		glGenBuffers(1, &m_textBuffer);

		glBindVertexArray(m_textMesh.m_vaoId);

		glBindBuffer(GL_ARRAY_BUFFER, m_textBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	void GraphicsPipe::m_funcSetupVao(Mesh& shape)
	{
		std::vector<glm::vec2> lvPosVtx;
		std::vector<glm::vec3> lvClrVtx;
		std::vector<glm::vec2> lvTexCoords;
		std::vector<GLushort>idx_vtx;


		lvPosVtx = { glm::vec2(0.5f, -0.5f),
						glm::vec2(0.5f, 0.5f),
						glm::vec2(-0.5f, 0.5f),
						glm::vec2(-0.5f, -0.5f) };
		lvClrVtx = { glm::vec3(1.f, 1.f, 1.f),
						glm::vec3(1.f, 1.f, 1.f),
						glm::vec3(1.f, 1.f, 1.f),
						glm::vec3(1.f, 1.f, 1.f) };
		lvTexCoords = { glm::vec2(1.f, 0.f),
						glm::vec2(1.f, 1.f),
						glm::vec2(0.f, 1.f),
						glm::vec2(0.f, 0.f) };



		GLsizei position_data_offset = 0;
		GLsizei position_attribute_size = sizeof(glm::vec2);
		GLsizei position_data_size = position_attribute_size * static_cast<GLsizei>(lvPosVtx.size());
		//GLsizei color_data_offset = position_data_size;
		//GLsizei color_attribute_size = sizeof(glm::vec3);
		//GLsizei color_data_size = color_attribute_size * static_cast<GLsizei>(lvClrVtx.size());
		GLsizei texcoord_data_offset = position_data_size;
		GLsizei texcoord_attribute_size = sizeof(glm::vec2);
		GLsizei texcoord_data_size = texcoord_attribute_size * static_cast<GLsizei>(lvTexCoords.size());


		unsigned int lvVboId{};

		glCreateBuffers(1, &lvVboId);

		glNamedBufferStorage(lvVboId, position_data_size + texcoord_data_size, nullptr, GL_DYNAMIC_STORAGE_BIT);

		glNamedBufferSubData(lvVboId, position_data_offset, position_data_size, lvPosVtx.data());
		//glNamedBufferSubData(lvVboId, color_data_offset, color_data_size, lvClrVtx.data());
		glNamedBufferSubData(lvVboId, texcoord_data_offset, texcoord_data_size, lvTexCoords.data());

		glCreateVertexArrays(1, &shape.m_vaoId);
		glEnableVertexArrayAttrib(shape.m_vaoId, 0);
		glVertexArrayVertexBuffer(shape.m_vaoId, 0, lvVboId,
			position_data_offset, position_attribute_size);
		glVertexArrayAttribFormat(shape.m_vaoId, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(shape.m_vaoId, 0, 0);

		/*glEnableVertexArrayAttrib(shape.m_vaoId, 1);
		glVertexArrayVertexBuffer(shape.m_vaoId, 1, lvVboId,
			color_data_offset, color_attribute_size);
		glVertexArrayAttribFormat(shape.m_vaoId, 1, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(shape.m_vaoId, 1, 1);*/


		glEnableVertexArrayAttrib(shape.m_vaoId, 2);
		glVertexArrayVertexBuffer(shape.m_vaoId, 2, lvVboId, texcoord_data_offset,
			texcoord_attribute_size);
		glVertexArrayAttribFormat(shape.m_vaoId, 2, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(shape.m_vaoId, 2, 2);



		shape.m_primitiveType = GL_TRIANGLES;
		idx_vtx = { 0, 1, 2, 2, 3, 0 };


		shape.m_indexElementCount = static_cast<unsigned short>(idx_vtx.size());
		unsigned int ebo_hdl;
		glCreateBuffers(1, &ebo_hdl);
		glNamedBufferStorage(ebo_hdl, sizeof(unsigned short) * shape.m_indexElementCount,
			reinterpret_cast<GLvoid*>(idx_vtx.data()),
			GL_DYNAMIC_STORAGE_BIT);
		glVertexArrayElementBuffer(shape.m_vaoId, ebo_hdl);
		glBindVertexArray(0);
	}
}