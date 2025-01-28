R"( #version 460 core


layout (location=0) in vec2 vertexPosition;
layout (location=1) in vec2 vertexNDCCoords;
layout (location=2) in vec2 vertexTexCoords;

layout (location=5) in vec3 aLightParams;
layout (location=11) in vec4 aColor;

layout (location=6) in mat3 modelMatrix;

layout (location=0) out vec4 color; 
layout (location=1) out vec2 lightPos;
layout (location=2) out vec3 lightParams;
//layout (location=4) out vec2 fragPos;
layout (location=5) out vec2 lightDirection;

uniform mat3 projection;
uniform mat3 view;

void main()
{
	
	mat3 matrix = projection * view * modelMatrix;

	gl_Position = vec4(vec2(matrix * vec3(vertexPosition, 1.f)),
					   -0.999, 1.0);

	lightDirection = vertexNDCCoords;

	lightPos = vec2(modelMatrix[0][2],modelMatrix[1][2]);

	color = aColor;
	lightParams = aLightParams;
}

)"