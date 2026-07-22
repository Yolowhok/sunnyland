package sunnyland;

import static org.lwjgl.opengl.GL20.*;

public class Shaders {
    public static final String vertexshadertext =
            "#version 330 core\n" +
            "layout(location = 0) in vec2 pos;\n" +
            "layout(location = 1) in vec2 tposi;\n" +
            "out vec2 tpos;\n" +
            "void main()\n" +
            "{\n" +
            "    gl_Position = vec4(pos, 0.0, 1.0);\n" +
            "    tpos = tposi;\n" +
            "}\n";

    public static final String fragmentshadertext =
            "#version 330 core\n" +
            "out vec4 FragColor;\n" +
            "in vec2 tpos;\n" +
            "uniform sampler2D ourTexture;\n" +
            "void main()\n" +
            "{\n" +
            "    FragColor = texture(ourTexture, tpos);\n" +
            "}\n";

    // Проверка компиляции шейдера
    public static void check_shader_compile(int shader) {
        int success = glGetShaderi(shader, GL_COMPILE_STATUS);
        if (success == 0) {
            String infoLog = glGetShaderInfoLog(shader, 512);
            System.err.println("Shader compilation failed:\n" + infoLog);
        }
    }

    public static int compileShader() {
        int vertexshader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexshader, vertexshadertext);
        glCompileShader(vertexshader);
        check_shader_compile(vertexshader);
        int fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentshader, fragmentshadertext);
        glCompileShader(fragmentshader);
        check_shader_compile(fragmentshader);
        int shaderprogram = glCreateProgram();
        glAttachShader(shaderprogram, vertexshader);
        glAttachShader(shaderprogram, fragmentshader);
        glLinkProgram(shaderprogram);
        int success = glGetProgrami(shaderprogram, GL_LINK_STATUS);
        if (success == 0) {
            String infoLog = glGetProgramInfoLog(shaderprogram, 512);
            System.err.println("Shader linking failed:\n" + infoLog);
        }
        //удаляем шейдеры после линковки
        glDeleteShader(vertexshader);
        glDeleteShader(fragmentshader);
        return shaderprogram;
    }
}
