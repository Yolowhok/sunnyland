#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "src/window.h"
#include "src/GameObject.h"
#include "src/Vertex.h"
#include "src/TextureLoader.h"
#include "src/Cursor.h"
#include "src/shaders.h"

#include <string>

static void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl; //Колбэк для ошибок
}

int main(){
    
    std::cout << "Sunny Land! 0.1" << std::endl; //Вывод названия игры для проверки запуска

    glfwSetErrorCallback(error_callback); //Устанавливаем колбек для ошибок

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }
    
    Window window(640, 420, "Sunny land!"); //Создания объекта окна из класса окна
    // Проверка библиотек
    window.windowInit();//Запуск метода инициализации окна

    if (!window.getWindow()) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;//проверка если окно не запустилось
    }

    glfwMakeContextCurrent(window.getWindow());//создаем контекст окна и делаем его активным


    // Инициализация GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return EXIT_FAILURE;//инициализируем GLAD и проверяем на ошибку 
    }

    glEnable(GL_BLEND);//Включаем GL_BLEND чтобы цвета могли смешиваться по Z
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//разрешаем смешивать цвета по прозрачности объектов



    GLuint shaderprogram = compileShader();



    glfwSwapInterval(1);//Настраиваем интервал смены кадров, чтобы не рисовать кадры которые и так не видно.

    Cursor cursor;
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //заполнение экрана цветом

    glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    int width, height;//создаем переменные для высоты и ширины окна
    // GameObject — Builder: texture/size/pos настраиваются по шагам
    GameObject player = GameObject::builder()
        .texture(loadTexture("../assets/player.png"))
        .build();
    std::cout << "cursor size x: " << cursor.size.x << " y: " << cursor.size.y << std::endl;
    cursor.setTexture(loadTexture("../assets/cursor.png"));
    glfwGetFramebufferSize(window.getWindow(), &width, &height);
    
    float pixelx = 2.0f / width;
    float pixely = 2.0f / height;
    cursor.setSize(64* 8*pixelx, 64 * 8*pixely);
    cursor.updateVertices();


    GameObject ball = GameObject::builder()
        .texture(loadTexture("../assets/ball.png"))
        .size(64.0f *4*window.pixel_x, 64.0f * 4*window.pixel_y)
        .build();
    double mouseX, mouseY;
    std::cout << "cursor size x: " << cursor.size.x << " y: " << cursor.size.y << std::endl;

    float speedx = 0.01f;
    float speedy = 0.005f;
    int frames[41];
    std::string characterpath = "../assets/character/idle/bb";
    for (int i=1; i<41; i++){
        std::string path = characterpath + std::to_string(i) + ".png";
        frames[i] = loadTexture(path.c_str());
    }
    int currentframe = 0;
    
    while (!glfwWindowShouldClose(window.getWindow()))//Главный цикл. Работает и ждет пока окно не будет закрыто
    {
        
        glfwGetFramebufferSize(window.getWindow(), &width, &height);//получаем размер буфера окна
        glViewport(0, 0, width, height);//создаем вьюпорт для игры
        glClear(GL_COLOR_BUFFER_BIT);//когда берем новый буфер то заполняем его цветом.
        glUseProgram(shaderprogram);

        glfwGetCursorPos(window.getWindow(), &mouseX, &mouseY);

        // Рисуем только контуры (линии) вместо заливки
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        player.draw();
        ball.draw();
        
        cursor.setSize(64.0f *4*window.pixel_x, 64.0f * 4*window.pixel_y);
        ball.setSize(64.0f *1*window.pixel_x, 64.0f * 1*window.pixel_y);
        player.setSize(64.0f *4*window.pixel_x, 64.0f * 4*window.pixel_y);
        ball.setPosition(ball.pos.x += speedx, ball.pos.y +=speedy);
        if (abs(ball.pos.x) >= 0.6f){
            std::cout << "hit!" << std::endl;
            speedx = speedx * -1;
        }
        if (abs(ball.pos.y) >= 0.6f){
            std::cout << "hit!" << std::endl;
            speedy = speedy * -1;
        }
        player.setTexture(frames[currentframe++]);
        if (currentframe > 40) currentframe = 1;

        cursor.mousePos(mouseX, mouseY, width, height);
        cursor.draw();


        if (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
            player.setPosition(player.pos.x, player.pos.y += 0.003f);
        }
        if (glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
            player.setPosition(player.pos.x, player.pos.y -= 0.003f);
        }
        if (glfwGetKey(window.getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
            player.setPosition(player.pos.x -= 0.003f, player.pos.y);
        }
        if (glfwGetKey(window.getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
            player.setPosition(player.pos.x += 0.003f, player.pos.y);
        }
        // Можно вернуть обратно на GL_LINE для других объектов, но пока не нужно
        
        glfwSwapBuffers(window.getWindow());//меняем буферы окна (у нас двойная буферизация)
        
        glfwPollEvents();//проверка всех событий по колбеку
    }


    glDeleteProgram(shaderprogram);
    //glfwTerminate(); //Удаляем из памяти glfw
    return EXIT_SUCCESS;//возвращаем успешный выход из игры
}

/*
убрать мусор из консоли
сделать чтобы картинки были рядом с экзе файлом
настроить все
сделать чтобы при загрузке можно было сохранять размер спрайта и его разрешение
При запуске исправить невидимость курсора *исправил. Ошибка была в том, что размер пикселя высчитывался только после изменения окна, при том что окно при спавне не изменяло свой размер, потому значения пикселей были не определены. На будущее стоит все поля определять хотя бы нулем или единицей*
Убрать мусор из файлов
разделить логику отрисовки и хранения данных в геймобъекте
Создать единый массив для VAO и VBO для всех объектов, чтобы в одном месте хранились данные
*/