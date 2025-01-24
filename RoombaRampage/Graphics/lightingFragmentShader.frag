R"( #version 460 core

	layout (location=0) in vec4 color;
	layout (location=1) in vec2 texCoords;

	layout (location=2) flat in int textureID;

	layout (location=0) out vec4 fragColor;

	uniform sampler2D textures[192]; // Subscript indicates max texture amount


	void main()
	{
		 vec4 texColor = texture(textures[textureID], texCoords) * vec4(color);

		 if(texColor.a < 0.1)
		 discard;
    
		fragColor = texColor;
	}
)"