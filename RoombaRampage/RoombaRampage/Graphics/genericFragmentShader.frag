R"( #version 460 core

	layout (location=0) in vec3 color;
	layout (location=1) in vec2 texCoords;

	//layout (location=2) in int textureID;

	layout (location=0) out vec4 fragColor;

	uniform sampler2D textures;


	void main()
	{
		 fragColor = texture(textures, texCoords) * vec4(color, 1.0);
	}
)"