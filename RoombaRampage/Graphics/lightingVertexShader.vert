R"( #version 460 core


layout (location=0) in vec2 vertexPosition;
layout (location=2) in vec2 vertexTexCoords;

layout (location=4) in vec2 aInnerOuterRadius;
layout (location=11) in vec4 aColor;

layout (location=6) in mat3 modelMatrix;

layout (location=0) out vec4 color; 

void main()
{
	gl_Position = vec4(vec2(modelMatrix * vec3(vertexPosition, 1.f)),
					   -0.999, 1.0);

	color = a_Color;
}

)"