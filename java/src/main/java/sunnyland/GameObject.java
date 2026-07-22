package sunnyland;

import org.lwjgl.system.MemoryStack;

import java.nio.FloatBuffer;

import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL15.*;
import static org.lwjgl.opengl.GL20.*;
import static org.lwjgl.opengl.GL30.*;

public class GameObject {
    public Vec2 pos = new Vec2(0.0f, 0.0f);
    public Vec2 size = new Vec2(1.0f, 1.0f);
    public Vec2 speed = new Vec2(0.0f);
    public int VAO, VBO;
    public int ID;

    public float angle;
    protected int textureID;

    public GameObject() {
        this.pos = new Vec2(0.0f);
        this.size = new Vec2(1.0f);
        this.speed = new Vec2(0.0f);
        this.angle = 0.0f;
        VAO = glGenVertexArrays();
        VBO = glGenBuffers();
        init();
    }

    /** Создаёт объект через Builder (паттерн Builder поверх полей GameObject из C++). */
    public static Builder builder() {
        return new Builder();
    }

    public static class Builder {
        private float posX = 0.0f;
        private float posY = 0.0f;
        private float sizeX = 1.0f;
        private float sizeY = 1.0f;
        private float speedX = 0.0f;
        private float speedY = 0.0f;
        private float angle = 0.0f;
        private int id = 0;
        private int textureID = 0;
        private boolean hasPosition = false;
        private boolean hasSize = false;
        private boolean hasSpeed = false;
        private boolean hasTexture = false;

        public Builder position(float x, float y) {
            this.posX = x;
            this.posY = y;
            this.hasPosition = true;
            return this;
        }

        public Builder size(float x, float y) {
            this.sizeX = x;
            this.sizeY = y;
            this.hasSize = true;
            return this;
        }

        public Builder speed(float x, float y) {
            this.speedX = x;
            this.speedY = y;
            this.hasSpeed = true;
            return this;
        }

        public Builder angle(float angle) {
            this.angle = angle;
            return this;
        }

        public Builder id(int id) {
            this.id = id;
            return this;
        }

        public Builder texture(int texID) {
            this.textureID = texID;
            this.hasTexture = true;
            return this;
        }

        public GameObject build() {
            GameObject obj = new GameObject();
            obj.angle = angle;
            obj.ID = id;
            if (hasSpeed) {
                obj.speed.x = speedX;
                obj.speed.y = speedY;
            }
            if (hasTexture) {
                obj.setTexture(textureID);
            }
            if (hasSize) {
                obj.setSize(sizeX, sizeY);
            }
            if (hasPosition) {
                obj.setPosition(posX, posY);
            }
            return obj;
        }
    }

    public void update(float deltaTime) {
    }

    public void draw() {
        if (textureID == 0) return; // нет текстуры – не рисуем
        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
    }

    public void setTexture(int texID) {
        textureID = texID;
    }

    public void setPosition(float x, float y) {
        pos.x = x;
        pos.y = y;
        updateVertices();
    }

    public void setSize(float x, float y) {
        size.x = x;
        size.y = y;
        updateVertices();
    }

    protected void init() {
        try (MemoryStack stack = MemoryStack.stackPush()) {
            FloatBuffer vertices = stack.mallocFloat(16);
            putVertices(vertices);
            vertices.flip();

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices, GL_DYNAMIC_DRAW);

            // Атрибут 0: позиция (2 float)
            glVertexAttribPointer(0, 2, GL_FLOAT, false, Vertex.SIZEOF, Vertex.OFFSET_POS);
            glEnableVertexAttribArray(0);
            // Атрибут 1: текстурные координаты (2 float)
            glVertexAttribPointer(1, 2, GL_FLOAT, false, Vertex.SIZEOF, Vertex.OFFSET_TEX);
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        updateVertices();
    }

    public void updateVertices() {
        try (MemoryStack stack = MemoryStack.stackPush()) {
            FloatBuffer vertices = stack.mallocFloat(16);
            putVertices(vertices);
            vertices.flip();
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices, GL_DYNAMIC_DRAW);
            // атрибуты настраивать повторно не нужно (они уже заданы)
        }
    }

    private void putVertices(FloatBuffer vertices) {
        // левый верх
        vertices.put(pos.x - size.x / 2).put(pos.y + size.y / 2).put(0.0f).put(1.0f);
        // левый низ
        vertices.put(pos.x - size.x / 2).put(pos.y - size.y / 2).put(0.0f).put(0.0f);
        // правый низ
        vertices.put(pos.x + size.x / 2).put(pos.y - size.y / 2).put(1.0f).put(0.0f);
        // правый верх
        vertices.put(pos.x + size.x / 2).put(pos.y + size.y / 2).put(1.0f).put(1.0f);
    }
}
//В идеале разделить функции отрисовки и хранения данных объекта
