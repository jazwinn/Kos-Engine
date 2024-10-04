/******************************************************************/
/*!
\file      genericFragmentShader.frag
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      2nd Oct, 2024
\brief     A basic fragment shader for rendering textures with color output.

           This shader calculates the final color of each fragment 
           based on the sampled texture data.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

R"( #version 460 core

	layout (location=0) in vec3 color;
	layout (location=1) in vec2 texCoords;

	layout (location=2) flat in int textureID;

	layout (location=0) out vec4 fragColor;

	uniform sampler2D textures[32]; // Subscript indicates max texture amount


	void main()
	{
		 vec4 texColor = texture(textures[textureID], texCoords) * vec4(color, 1.0);

		 if(texColor.a < 0.01)
		 discard;
    
		fragColor = texColor;
	}
)"