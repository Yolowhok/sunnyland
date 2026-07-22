package sunnyland.engine;

import org.lwjgl.BufferUtils;
import org.lwjgl.system.MemoryStack;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.nio.channels.Channels;
import java.nio.channels.ReadableByteChannel;
import java.util.HashMap;
import java.util.Map;

import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL30.glGenerateMipmap;
import static org.lwjgl.stb.STBImage.*;

/**
 * Loads and caches textures from classpath resources.
 * Callers request by path; GPU lifetime is managed here.
 */
public final class AssetManager implements AutoCloseable {
    private final Map<String, Texture> cache = new HashMap<>();

    public Texture getTexture(String resourcePath) {
        return cache.computeIfAbsent(resourcePath, this::loadTexture);
    }

    private Texture loadTexture(String resourcePath) {
        ByteBuffer imageBuffer;
        try {
            imageBuffer = readResource(resourcePath);
        } catch (IOException e) {
            throw new IllegalStateException("Failed to read texture resource: " + resourcePath, e);
        }

        try (MemoryStack stack = MemoryStack.stackPush()) {
            IntBuffer width = stack.mallocInt(1);
            IntBuffer height = stack.mallocInt(1);
            IntBuffer channels = stack.mallocInt(1);

            stbi_set_flip_vertically_on_load(true);
            ByteBuffer data = stbi_load_from_memory(imageBuffer, width, height, channels, 0);
            if (data == null) {
                throw new IllegalStateException("Failed to decode texture: " + resourcePath + " - " + stbi_failure_reason());
            }

            int format = switch (channels.get(0)) {
                case 1 -> GL_RED;
                case 3 -> GL_RGB;
                case 4 -> GL_RGBA;
                default -> {
                    stbi_image_free(data);
                    throw new IllegalStateException("Unsupported channel count: " + channels.get(0));
                }
            };

            int textureId = glGenTextures();
            glBindTexture(GL_TEXTURE_2D, textureId);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width.get(0), height.get(0), 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            stbi_image_free(data);

            System.out.println("Texture loaded: " + resourcePath
                    + " (" + width.get(0) + "x" + height.get(0) + ", " + channels.get(0) + " channels)");
            return new Texture(textureId, width.get(0), height.get(0));
        }
    }

    private static ByteBuffer readResource(String resource) throws IOException {
        InputStream source = AssetManager.class.getClassLoader().getResourceAsStream(resource);
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
                    ByteBuffer bigger = BufferUtils.createByteBuffer(buffer.capacity() * 2);
                    buffer.flip();
                    bigger.put(buffer);
                    buffer = bigger;
                }
            }
            buffer.flip();
            return buffer;
        }
    }

    @Override
    public void close() {
        for (Texture texture : cache.values()) {
            texture.close();
        }
        cache.clear();
    }
}
