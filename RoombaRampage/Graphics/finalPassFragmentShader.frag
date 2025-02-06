/******************************************************************/
/*!
\file      finalPassFragmentShader.frag
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      17th Jan, 2025
\brief     

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
R"( #version 460 core


out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D screenTexture;
uniform sampler2D lightTexture;
uniform sampler2D unlitScreenTexture;
uniform float globalBrightness;

void main()
{
    vec4 sceneColor = texture(screenTexture, texCoords);
    vec4 lightColor = texture(lightTexture, texCoords);
    vec4 unlitSceneColor = texture(unlitScreenTexture, texCoords);

    vec4 darkenedScene = sceneColor * globalBrightness;  
    vec4 restoredBrightness = vec4 (0,0,0,1.f);
    restoredBrightness.r = mix(darkenedScene.r, sceneColor.r, lightColor.r);
     restoredBrightness.g = mix(darkenedScene.g, sceneColor.g, lightColor.g);
      restoredBrightness.b = mix(darkenedScene.b, sceneColor.b, lightColor.b);
       

    vec4 light = vec4(lightColor.rgb, 1.0);

    vec4 multipliedLighting = restoredBrightness * light;

    float r = max(darkenedScene.r, multipliedLighting.r);
     float g = max(darkenedScene.g, multipliedLighting.g);
      float b = max(darkenedScene.b, multipliedLighting.b);
      // float a = max(darkenedScene.a, multipliedLighting.a);

    vec4 final = vec4(r,g,b,1.0);
    vec4 litScene = clamp(final, 0.0, 1.0);
    fragColor = litScene;

    if(unlitSceneColor.r != 0 || unlitSceneColor.g != 0 || unlitSceneColor.b != 0 || unlitSceneColor.a != 0)
    {
        fragColor = unlitSceneColor;
    }
} 

)"