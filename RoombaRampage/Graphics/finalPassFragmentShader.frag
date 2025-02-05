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

    vec4 darkenedScene = sceneColor * globalBrightness;  
    vec4 restoredBrightness = mix(darkenedScene, sceneColor, lightColor.r);

    vec4 light = vec4(lightColor.rgb, 1.0);

    vec4 multipliedLighting = restoredBrightness * light;

    float r = max(darkenedScene.r, multipliedLighting.r);
     float g = max(darkenedScene.g, multipliedLighting.g);
      float b = max(darkenedScene.b, multipliedLighting.b);
      // float a = max(darkenedScene.a, multipliedLighting.a);

    vec4 final = vec4(r,g,b,1.0);
    fragColor = clamp(final, 0.0, 1.0);
} 

)"