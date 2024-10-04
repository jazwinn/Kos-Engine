/******************************************************************/
/*!
\file      textVertexShader.vert
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      2nd Oct, 2024
\brief     A vertex shader for rendering text.

           This shader processes vertex positions and texture 
           coordinates for text rendering. It applies a projection 
           matrix to position the text in 2D space and adjusts 
           the z-value to ensure text is rendered in front.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

R"( #version 460 core

layout (location = 0) in vec4 vertex;
out vec2 texCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.9, 1.0); // z value to draw text in front for now
    texCoords = vertex.zw;
}  

)"