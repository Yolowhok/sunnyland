#ifndef SHADERS
#define SHADERS

#include <glad/glad.h>
#include <iostream>

static const char* vertexshadertext = 
R"(#version 330 core
layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 tposi;
out vec2 tpos;
void main()
{
    gl_Position = vec4(pos, 0.0, 1.0);
    tpos = tposi;
}
)";

static const char* fragmentshadertext =
R"(#version 330 core
out vec4 FragColor;
in vec2 tpos;
uniform sampler2D ourTexture;
void main()
{
    FragColor = texture(ourTexture, tpos);
}
)";
void check_shader_compile(GLuint shader);
GLuint compileShader();
#endif