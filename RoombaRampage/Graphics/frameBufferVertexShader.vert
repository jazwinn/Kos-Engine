/******************************************************************/
/*!
\file      frameBufferVertexShader.vert
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      2nd Oct, 2024
\brief     A vertex shader for framebuffer rendering.

           This shader processes vertex positions and 
           texture coordinates, outputting the final 
           position for rendering.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

R"( #version 460 core

layout (location = 0) in vec4 vertex;

out vec2 texCoords;

void main()
{
    texCoords = vertex.zw;
    gl_Position = vec4(vertex.xy, 0.0, 1.0); 
}  


)"