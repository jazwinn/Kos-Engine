R"( #version 460 core


layout (location=0) in vec2 vertexPosition;
layout (location=1) in vec3 vertexColor;
layout (location=2) in vec2 vertexTexCoords;

layout (location=3) in int aFrameNumber;
layout (location=4) in int aStripCount;
layout (location=5) in int atextureID;

layout (location=6) in mat3 modelMatrix;



layout (location=0) out vec3 color; 
layout (location=1) out vec2 texCoords;
layout (location=2) flat out int textureID;

void main()
{

	float frameWidth = 1.0 / aStripCount;

	float frameOffset = frameWidth * aFrameNumber;

	gl_Position = vec4(vec2(modelMatrix * vec3(vertexPosition, 1.f)),
					   -0.1 * atextureID, 1.0);

	texCoords = vec2(vertexTexCoords.x * frameWidth + frameOffset, vertexTexCoords.y);
	color = vertexColor;
	textureID = atextureID;
}

)"