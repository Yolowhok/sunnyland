#include "shaders.h"

// Проверка компиляции шейдера
void check_shader_compile(GLuint shader) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;
    }
};
GLuint compileShader() {
    GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexshader, 1, &vertexshadertext, NULL);
    glCompileShader(vertexshader);
    check_shader_compile(vertexshader);
    GLuint fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentshader, 1, &fragmentshadertext, NULL);
    glCompileShader(fragmentshader);
    check_shader_compile(fragmentshader);
    GLuint shaderprogram = glCreateProgram();
    glAttachShader(shaderprogram, vertexshader);
    glAttachShader(shaderprogram, fragmentshader);
    glLinkProgram(shaderprogram);
    GLint success;
    glGetProgramiv(shaderprogram, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(shaderprogram, 512, NULL, infoLog);
            std::cerr << "Shader linking failed:\n" << infoLog << std::endl;
            }
    //удаляем шейдеры после линковки
    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);
    return shaderprogram;
}
