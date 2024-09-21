R"( #version 460 core

	layout (location=0) in vec3 color;
	layout (location=1) in vec2 texCoords;

	layout (location=2) flat in int textureID;

	layout (location=0) out vec4 fragColor;

	uniform sampler2D textures[16];


	void main()
	{
		 vec4 texColor = texture(textures[textureID], texCoords) * vec4(color, 1.0);

		 if(texColor.a < 0.01)
		 discard;
    
		fragColor = texColor;
	}
)"