/******************************************************************/
/*!
\file      debugFragmentShader.frag
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th Nov, 2024
\brief     A fragment shader used for debug visualization. 

           This shader outputs green or red colors depending on the 
           collision status, with green indicating no collision and 
           red indicating a collision. Camera indicates an orange box.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

R"( #version 460 core

    in vec2 TexCoords;
    out vec4 FragColor;

    uniform sampler2D yTexture;
    uniform sampler2D uTexture;
    uniform sampler2D vTexture;


	void main()
	{
        vec2 chromaTexCoords = TexCoords * 0.5; // Scale texture coordinates for chroma components
        float y = texture(yTexture, TexCoords).r;
        float u = texture(uTexture, chromaTexCoords).r - 0.5;
        float v = texture(vTexture, chromaTexCoords).r - 0.5;

        float r = y + 1.402 * v;
        float g = y - 0.344136 * u - 0.714136 * v;
        float b = y + 1.772 * u;

        FragColor = vec4(r, g, b, 1.0);
		
	}
)"