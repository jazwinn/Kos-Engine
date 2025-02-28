/******************************************************************/
/*!
\file      debugVertexShader.vert
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      2nd Oct, 2024
\brief     A vertex shader used for debug rendering.

           This shader calculates vertex positions based on the 
           model matrix and passes a collision status to the fragment 
           shader to aid in debugging collision detection.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

R"( #version 460 core


    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;

    out vec2 TexCoords;

    uniform mat3 projection;
    uniform mat3 view;

void main()
{
    mat3 matrix = projection * view;
    // gl_Position = vec4(vec3(matrix * aPos), 1.0);

    gl_Position = vec4(aPos, 1.0);

    TexCoords = aTexCoord;
}

)"