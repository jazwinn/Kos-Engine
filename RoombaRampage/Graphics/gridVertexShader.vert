R"( #version 460 core

    layout (location = 0) in vec3 aPos;

    uniform mat3 view;
    uniform mat3 projection;

    layout (location = 0) out vec3 col;

    void main() 
    {
         gl_Position = vec4(vec2(projection * vec3(view * vec3(aPos.xy, 1.f))), -0.99, 1.0);
         if ((aPos.x < 0.1 && aPos.x > -0.1) || (aPos.y < 0.1 && aPos.y > -0.1))
         {
            col = vec3(0,0,0);
         }
         else
         {
             col = vec3(0.6,0.6,0.6);
         }
    }

)"