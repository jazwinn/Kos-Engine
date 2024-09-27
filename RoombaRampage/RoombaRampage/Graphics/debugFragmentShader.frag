R"( #version 460 core

	layout (location=0) in vec3 color;
	layout (location=1) in bool toDraw;
	layout (location=0) out vec4 fragColor;


	void main()
	{
		if(toDraw)
		{
			fragColor = vec4(color,1.0);
		}
		else
		{
			fragColor = vec4(0.f,0.f,0.f,0.f);
		}
		
	}
)"