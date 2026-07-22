#include <glad/glad.h>
#include "GameObject.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Vertex.h"


GameObject::GameObject()
    : pos(0.0f), size(1.0f), speed(0.0f), angle(0.0f) 
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        init();
    }

void GameObject::update(float deltaTime) {
}

void GameObject::draw() {
    if (textureID == 0) return; // нет текстуры – не рисуем
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}

void GameObject::init()
{
    Vertex vertices[] = {
        {{pos.x - size.x / 2, pos.y + size.y / 2}, {0.0f, 1.0f}},  // левый верх
        {{pos.x - size.x / 2, pos.y - size.y / 2}, {0.0f, 0.0f}},  // левый низ
        {{pos.x + size.x / 2, pos.y - size.y / 2}, {1.0f, 0.0f}},  // правый низ
        {{pos.x + size.x / 2, pos.y + size.y / 2}, {1.0f, 1.0f}}   // правый верх
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    // Атрибут 0: позиция (2 float)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);
    // Атрибут 1: текстурные координаты (2 float)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    updateVertices();
}

void GameObject::updateVertices() {
    Vertex vertices[] = {
        {{pos.x - size.x/2, pos.y + size.y/2}, {0.0f, 1.0f}},
        {{pos.x - size.x/2, pos.y - size.y/2}, {0.0f, 0.0f}},
        {{pos.x + size.x/2, pos.y - size.y/2}, {1.0f, 0.0f}},
        {{pos.x + size.x/2, pos.y + size.y/2}, {1.0f, 1.0f}}
    };
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    // атрибуты настраивать повторно не нужно (они уже заданы)
    
}

GameObject::Builder& GameObject::Builder::position(float x, float y) {
    posX = x;
    posY = y;
    hasPosition = true;
    return *this;
}

GameObject::Builder& GameObject::Builder::size(float x, float y) {
    sizeX = x;
    sizeY = y;
    hasSize = true;
    return *this;
}

GameObject::Builder& GameObject::Builder::speed(float x, float y) {
    speedX = x;
    speedY = y;
    hasSpeed = true;
    return *this;
}

GameObject::Builder& GameObject::Builder::angle(float angle) {
    angle_ = angle;
    return *this;
}

GameObject::Builder& GameObject::Builder::id(int id) {
    id_ = id;
    return *this;
}

GameObject::Builder& GameObject::Builder::texture(GLuint texID) {
    textureID_ = texID;
    hasTexture = true;
    return *this;
}

GameObject GameObject::Builder::build() {
    GameObject obj;
    obj.angle = angle_;
    obj.ID = id_;
    if (hasSpeed) {
        obj.speed.x = speedX;
        obj.speed.y = speedY;
    }
    if (hasTexture) {
        obj.setTexture(textureID_);
    }
    if (hasSize) {
        obj.setSize(sizeX, sizeY);
    }
    if (hasPosition) {
        obj.setPosition(posX, posY);
    }
    return obj;
}
//В идеале разделить функции отрисовки и хранения данных объекта