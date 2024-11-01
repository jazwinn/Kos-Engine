/******************************************************************/
/*!
\file      debugFragmentShader.frag
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      2nd Oct, 2024
\brief     A fragment shader used for debug visualization. 

           This shader outputs green or red colors depending on the 
           collision status, with green indicating no collision and 
           red indicating a collision.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

R"( #version 460 core

	layout (location=0) in float collided;
	layout (location=0) out vec4 fragColor;


	void main()
	{
		if (collided < 0.5)
		{
			fragColor = vec4(0,1,0,1.0); // Green
		}
		else if (collided > 0.5 && collided < 1.5)
		{
			fragColor = vec4(1,0,0,1.0); // Red
		}
		else
		{
			fragColor = vec4(1.0,0.8,0.0,1.0); // Camera
		}


		
	}
)"