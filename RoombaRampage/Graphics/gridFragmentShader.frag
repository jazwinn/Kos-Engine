R"( #version 460 core


	in vec4 col;
	out vec4 fragColor;

	void main() 
	{
		fragColor = vec4(col.x, col.y, col.z, col.w); // Gray color for grid lines
	}

)"