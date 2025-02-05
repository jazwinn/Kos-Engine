R"( #version 460 core


out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D screenTexture;
uniform sampler2D lightTexture;
uniform float globalBrightness;

void main()
{
    vec4 sceneColor = texture(screenTexture, texCoords);
    vec4 lightColor = texture(lightTexture, texCoords);

    // Apply global illumination darkening
    vec4 darkenedScene = sceneColor * globalBrightness;

    // Restore brightness where the lightTexture is strong
    vec4 col = mix(darkenedScene, sceneColor, lightColor.r); // Use red channel of light texture

   
   fragColor = col;
} 

)"