package sunnyland.engine;

import static org.lwjgl.opengl.GL20.*;

/** Compiles and owns a simple textured-quad shader program. */
public final class ShaderProgram implements AutoCloseable {
    private final int programId;

    public static ShaderProgram texturedQuad() {
        String vertex =
                """
                #version 330 core
                layout(location = 0) in vec2 pos;
                layout(location = 1) in vec2 tposi;
                out vec2 tpos;
                void main() {
                    gl_Position = vec4(pos, 0.0, 1.0);
                    tpos = tposi;
                }
                """;
        String fragment =
                """
                #version 330 core
                out vec4 FragColor;
                in vec2 tpos;
                uniform sampler2D ourTexture;
                void main() {
                    FragColor = texture(ourTexture, tpos);
                }
                """;
        return new ShaderProgram(vertex, fragment);
    }

    public ShaderProgram(String vertexSource, String fragmentSource) {
        int vertex = compile(GL_VERTEX_SHADER, vertexSource);
        int fragment = compile(GL_FRAGMENT_SHADER, fragmentSource);
        programId = glCreateProgram();
        glAttachShader(programId, vertex);
        glAttachShader(programId, fragment);
        glLinkProgram(programId);
        if (glGetProgrami(programId, GL_LINK_STATUS) == 0) {
            throw new IllegalStateException("Shader linking failed:\n" + glGetProgramInfoLog(programId, 512));
        }
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    private static int compile(int type, String source) {
        int shader = glCreateShader(type);
        glShaderSource(shader, source);
        glCompileShader(shader);
        if (glGetShaderi(shader, GL_COMPILE_STATUS) == 0) {
            throw new IllegalStateException("Shader compilation failed:\n" + glGetShaderInfoLog(shader, 512));
        }
        return shader;
    }

    public void bind() {
        glUseProgram(programId);
    }

    public void unbind() {
        glUseProgram(0);
    }

    @Override
    public void close() {
        glDeleteProgram(programId);
    }
}
