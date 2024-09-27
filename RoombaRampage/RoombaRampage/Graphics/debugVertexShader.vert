R"( #version 460 core


layout (location=0) in vec2 vertexPosition;
layout (location=1) in vec3 vertexColor;

layout (location=10) in mat3 modelMatrix;

layout (location=15) bool toDraw;

layout (location=0) out vec3 color;
layout (location=1) out bool drawDebug;

void main()
{

	gl_Position = vec4(vec2(modelMatrix * vec3(vertexPosition, 1.f)),
					   0.0, 1.0);

	color = vertexColor;
	drawDebug = toDraw;
}

)"