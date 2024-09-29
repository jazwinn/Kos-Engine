R"( #version 460 core

	layout (location=0) in float collided;
	layout (location=0) out vec4 fragColor;


	void main()
	{
		if (collided < 0.5)
		{
			fragColor = vec4(0,1,0,1.0); // Green
		}
		else
		{
			fragColor = vec4(1,0,0,1.0); // Red
		}

		
	}
)"