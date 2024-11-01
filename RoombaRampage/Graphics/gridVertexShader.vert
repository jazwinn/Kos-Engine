R"( #version 460 core

    layout (location = 0) in vec3 aPos;

    uniform mat3 view;
    uniform mat3 projection;

    void main() 
    {
         gl_Position = vec4(vec2(projection * vec3(view * vec3(aPos.xy, 1.f))), -0.99, 1.0);
    }

)"