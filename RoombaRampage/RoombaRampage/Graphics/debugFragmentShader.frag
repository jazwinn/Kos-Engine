R"( #version 460 core

	layout (location=0) in vec3 color;
	//layout (location=1) in float toDraw;
	layout (location=0) out vec4 fragColor;


	void main()
	{
		fragColor = vec4(0,1.0,0,1.0);
	}
)"