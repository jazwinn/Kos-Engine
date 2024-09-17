R"( #version 450 core

	layout (location=0) in vec3 color;
	//layout (location=1) in vec3 texCoords;

	layout (location=0) out vec4 fragColor;

	//uniform sampler2D texture;

	void main()
	{
		fragColor = vec4(color, 1.0);
	}
)"