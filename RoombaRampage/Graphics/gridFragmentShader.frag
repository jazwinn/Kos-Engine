R"( #version 460 core


	in vec3 col;
	out vec4 fragColor;

	void main() 
	{
		fragColor = vec4(col.x, col.y, col.z, 1.0); // Gray color for grid lines
	}

)"