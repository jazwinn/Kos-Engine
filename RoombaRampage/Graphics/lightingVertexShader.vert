R"( #version 460 core


layout (location=0) in vec2 vertexPosition;
layout (location=2) in vec2 vertexTexCoords;

layout (location=5) in vec3 aLightParams;
layout (location=11) in vec4 aColor;

layout (location=6) in mat3 modelMatrix;

layout (location=0) out vec4 color; 
//layout (location=1) out vec2 lightPos;
layout (location=2) out vec3 lightParams;
layout (location=3) out float distance;

uniform mat3 projection;
uniform mat3 view;

void main()
{
	
	mat3 matrix = projection * view * modelMatrix;

	gl_Position = vec4(vec2(matrix * vec3(vertexPosition, 1.f)),
					   -0.999, 1.0);
	vec4 center = vec4(vec2(matrix * vec3(0.f,0.f,1.f)), -0.999,1.0);

	distance = length(vec2(gl_Position.xy) - vec2(center.xy));

	color = aColor;
	lightParams = aLightParams;
}

)"