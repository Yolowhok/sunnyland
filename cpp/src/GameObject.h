#ifndef GAME_OBJECT
#define GAME_OBJECT
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Vertex.h"
class GameObject{
public:
    glm::vec2 pos = glm::vec2 (0.0f, 0.0f);
    glm::vec2 size = glm::vec2 (1.0f, 1.0f);
    glm::vec2 speed;
    GLuint VAO, VBO;
    int ID;
    
    float angle;
    GameObject();
    virtual ~GameObject() = default;
    virtual void update(float deltaTime);
    virtual void draw();
    void setTexture(GLuint texID) { textureID = texID; }
    void setPosition(float x, float y) {pos.x = x; pos.y = y; updateVertices();}
    void setSize(float x, float y) {size.x = x; size.y = y; updateVertices();}
    void updateVertices();

    /** Builder: пошаговая настройка texture/size/position/speed/angle/id */
    class Builder {
    public:
        Builder& position(float x, float y);
        Builder& size(float x, float y);
        Builder& speed(float x, float y);
        Builder& angle(float angle);
        Builder& id(int id);
        Builder& texture(GLuint texID);
        GameObject build();
    private:
        float posX = 0.0f;
        float posY = 0.0f;
        float sizeX = 1.0f;
        float sizeY = 1.0f;
        float speedX = 0.0f;
        float speedY = 0.0f;
        float angle_ = 0.0f;
        int id_ = 0;
        GLuint textureID_ = 0;
        bool hasPosition = false;
        bool hasSize = false;
        bool hasSpeed = false;
        bool hasTexture = false;
    };

    static Builder builder() { return Builder(); }

protected:
    void init();
    GLuint textureID;
};
#endif
