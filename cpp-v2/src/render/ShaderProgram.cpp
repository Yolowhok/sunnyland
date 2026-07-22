#include "render/ShaderProgram.hpp"

#include <glad/glad.h>

#include <iostream>
#include <stdexcept>
#include <string>

namespace render {

ShaderProgram ShaderProgram::texturedQuad() {
    static const char* vertex =
        R"(#version 330 core
layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 tposi;
out vec2 tpos;
void main() {
    gl_Position = vec4(pos, 0.0, 1.0);
    tpos = tposi;
}
)";
    static const char* fragment =
        R"(#version 330 core
out vec4 FragColor;
in vec2 tpos;
uniform sampler2D ourTexture;
void main() {
    FragColor = texture(ourTexture, tpos);
}
)";
    return ShaderProgram(vertex, fragment);
}

unsigned int ShaderProgram::compile(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        throw std::runtime_error(std::string("Shader compilation failed:\n") + infoLog);
    }
    return shader;
}

ShaderProgram::ShaderProgram(const char* vertexSource, const char* fragmentSource) {
    unsigned int vertex = compile(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragment = compile(GL_FRAGMENT_SHADER, fragmentSource);
    programId_ = glCreateProgram();
    glAttachShader(programId_, vertex);
    glAttachShader(programId_, fragment);
    glLinkProgram(programId_);
    int success = 0;
    glGetProgramiv(programId_, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(programId_, 512, nullptr, infoLog);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        throw std::runtime_error(std::string("Shader linking failed:\n") + infoLog);
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

ShaderProgram::~ShaderProgram() {
    if (programId_ != 0) {
        glDeleteProgram(programId_);
        programId_ = 0;
    }
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept : programId_(other.programId_) {
    other.programId_ = 0;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {
    if (this != &other) {
        if (programId_ != 0) {
            glDeleteProgram(programId_);
        }
        programId_ = other.programId_;
        other.programId_ = 0;
    }
    return *this;
}

void ShaderProgram::bind() const {
    glUseProgram(programId_);
}

void ShaderProgram::unbind() const {
    glUseProgram(0);
}

} // namespace render
