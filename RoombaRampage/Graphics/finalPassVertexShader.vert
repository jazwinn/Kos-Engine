R"( #version 460 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 inTexCoords;

out vec2 texCoords;

void main()
{
    gl_Position = vec4(vertex.x, vertex.y, 0.0, 1.0); 
    texCoords = inTexCoords;
}  


)"