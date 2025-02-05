R"( #version 460 core


out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D screenTexture;
uniform sampler2D lightTexture;
uniform float globalIllumination;

void main()
{
    vec4 sceneColor = texture(screenTexture, texCoords);
    vec3 lightColor = texture(lightTexture, texCoords);

    vec3 col = sceneColor.rgb * (1.0 + lightColor * )
  

   
   fragColor = col;
} 

)"