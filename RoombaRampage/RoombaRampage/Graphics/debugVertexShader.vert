R"( #version 460 core


layout (location=0) in vec2 vertexPosition;

layout (location=4) in float isCollided;

layout (location=7) in mat3 modelMatrix;

layout (location=0) out float collided;

void main()
{

	gl_Position = vec4(vec2(modelMatrix * vec3(vertexPosition, 1.f)),
					   0.0, 1.0);
	collided = isCollided;
}

)"