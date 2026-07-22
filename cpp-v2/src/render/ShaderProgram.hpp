#pragma once

namespace render {

class ShaderProgram {
public:
    static ShaderProgram texturedQuad();

    ShaderProgram(const char* vertexSource, const char* fragmentSource);
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;
    ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;

    void bind() const;
    void unbind() const;
    unsigned int id() const { return programId_; }

private:
    static unsigned int compile(unsigned int type, const char* source);

    unsigned int programId_ = 0;
};

} // namespace render
