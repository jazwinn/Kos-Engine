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