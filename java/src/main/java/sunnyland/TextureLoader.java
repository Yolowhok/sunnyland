package sunnyland;

import org.lwjgl.BufferUtils;
import org.lwjgl.system.MemoryStack;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.nio.channels.Channels;
import java.nio.channels.ReadableByteChannel;

import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL30.glGenerateMipmap;
import static org.lwjgl.stb.STBImage.*;

/** Загружает текстуру из файла/ресурса и возвращает OpenGL ID */
public class TextureLoader {

    public static int loadTexture(String filepath) {
        int textureID = glGenTextures();

        // пути вида ../assets/... из C++ → resources/assets/...
        String resourcePath = filepath;
        if (resourcePath.startsWith("../")) {
            resourcePath = resourcePath.substring(3);
        }

        ByteBuffer imageBuffer;
        try {
            imageBuffer = ioResourceToByteBuffer(resourcePath);
        } catch (IOException e) {
            System.err.println("Failed to load texture: " + filepath + " - " + e.getMessage());
            glDeleteTextures(textureID);
            return 0;
        }

        try (MemoryStack stack = MemoryStack.stackPush()) {
            IntBuffer width = stack.mallocInt(1);
            IntBuffer height = stack.mallocInt(1);
            IntBuffer nrChannels = stack.mallocInt(1);

            stbi_set_flip_vertically_on_load(true);
            ByteBuffer data = stbi_load_from_memory(imageBuffer, width, height, nrChannels, 0);
            if (data != null) {
                int format;
                int channels = nrChannels.get(0);
                if (channels == 1)
                    format = GL_RED;
                else if (channels == 3)
                    format = GL_RGB;
                else if (channels == 4)
                    format = GL_RGBA;
                else {
                    System.err.println("Unsupported channel count: " + channels);
                    stbi_image_free(data);
                    glDeleteTextures(textureID);
                    return 0;
                }

                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width.get(0), height.get(0), 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                // Настройки фильтрации и оборачивания
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                System.out.println("Texture loaded: " + filepath
                        + " (" + width.get(0) + "x" + height.get(0) + ", " + channels + " channels)");

                stbi_image_free(data);
            } else {
                System.err.println("Failed to load texture: " + filepath
                        + " - " + stbi_failure_reason());
                glDeleteTextures(textureID);
                return 0;
            }
        }
        return textureID;
    }

    private static ByteBuffer ioResourceToByteBuffer(String resource) throws IOException {
        InputStream source = TextureLoader.class.getClassLoader().getResourceAsStream(resource);
        if (source == null) {
            throw new IOException("Resource not found: " + resource);
        }
        try (InputStream is = source; ReadableByteChannel rbc = Channels.newChannel(is)) {
            ByteBuffer buffer = BufferUtils.createByteBuffer(8 * 1024);
            while (true) {
                int bytes = rbc.read(buffer);
                if (bytes == -1) {
                    break;
                }
                if (buffer.remaining() == 0) {
                    buffer = resizeBuffer(buffer, buffer.capacity() * 2);
                }
            }
            buffer.flip();
            return buffer;
        }
    }

    private static ByteBuffer resizeBuffer(ByteBuffer buffer, int newCapacity) {
        ByteBuffer newBuffer = BufferUtils.createByteBuffer(newCapacity);
        buffer.flip();
        newBuffer.put(buffer);
        return newBuffer;
    }
}
