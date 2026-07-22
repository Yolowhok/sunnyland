package sunnyland.engine;

import org.lwjgl.system.MemoryStack;

import java.nio.FloatBuffer;

import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL15.*;
import static org.lwjgl.opengl.GL20.*;
import static org.lwjgl.opengl.GL30.*;

/**
 * GPU quad mesh for a sprite. Geometry is updated from NDC position/size.
 * Separated from gameplay entity data.
 */
public final class Sprite implements AutoCloseable {
    private static final int STRIDE = 4 * Float.BYTES;

    private final int vao;
    private final int vbo;
    private Texture texture;
    private final Vec2 position = new Vec2();
    private final Vec2 size = new Vec2(1f, 1f);

    public Sprite() {
        vao = glGenVertexArrays();
        vbo = glGenBuffers();
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, 16L * Float.BYTES, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, false, STRIDE, 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, false, STRIDE, 2L * Float.BYTES);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        uploadVertices();
    }

    public void setTexture(Texture texture) {
        this.texture = texture;
    }

    public Texture texture() {
        return texture;
    }

    public Vec2 position() {
        return position;
    }

    public Vec2 size() {
        return size;
    }

    public void setPosition(float x, float y) {
        position.set(x, y);
        uploadVertices();
    }

    public void setSize(float x, float y) {
        size.set(x, y);
        uploadVertices();
    }

    public void setTransform(float x, float y, float sizeX, float sizeY) {
        position.set(x, y);
        size.set(sizeX, sizeY);
        uploadVertices();
    }

    public void draw() {
        if (texture == null) {
            return;
        }
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, texture.id());
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
    }

    private void uploadVertices() {
        try (MemoryStack stack = MemoryStack.stackPush()) {
            FloatBuffer vertices = stack.mallocFloat(16);
            float hx = size.x / 2f;
            float hy = size.y / 2f;
            // left-top, left-bottom, right-bottom, right-top
            vertices.put(position.x - hx).put(position.y + hy).put(0f).put(1f);
            vertices.put(position.x - hx).put(position.y - hy).put(0f).put(0f);
            vertices.put(position.x + hx).put(position.y - hy).put(1f).put(0f);
            vertices.put(position.x + hx).put(position.y + hy).put(1f).put(1f);
            vertices.flip();
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }

    @Override
    public void close() {
        glDeleteBuffers(vbo);
        glDeleteVertexArrays(vao);
    }
}
