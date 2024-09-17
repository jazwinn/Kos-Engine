R"( #version 450 core


//Input Variables
layout (location=0) in vec2 vertexPosition;
layout (location=1) in vec3 vertexColor;
//layout (location=2) in vec2 vertexTexCoords;
layout (location=3) in mat3 modelMatrix;

//Output Variables
layout (location=0) out vec3 color;
//layout (location=1) out texCoords;

//Uniform
//unifrom mat3 modelToNDCMatrix;

void main()
{


	gl_Position = vec4(vec2(modelMatrix * vec3(vertexPosition, 1.f)),
					   0.0, 1.0);

	color = vertexColor;
}

)"