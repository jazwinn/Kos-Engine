R"( #version 460 core


out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D screenTexture;

void main()
{
    vec4 col = texture(screenTexture, texCoords).rgba;
    fragColor = col;
} 

)"