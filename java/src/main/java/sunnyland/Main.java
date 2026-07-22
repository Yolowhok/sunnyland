package sunnyland;

import org.lwjgl.opengl.GL;
import org.lwjgl.system.MemoryStack;

import java.nio.DoubleBuffer;
import java.nio.IntBuffer;

import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL20.glDeleteProgram;
import static org.lwjgl.opengl.GL20.glUseProgram;
import static org.lwjgl.system.MemoryUtil.NULL;

public class Main {

    private static void error_callback(int error, long description) {
        System.err.println("Error: " + org.lwjgl.system.MemoryUtil.memUTF8(description)); //Колбэк для ошибок
    }

    public static void main(String[] args) {

        System.out.println("Sunny Land! 0.1"); //Вывод названия игры для проверки запуска

        glfwSetErrorCallback(Main::error_callback); //Устанавливаем колбек для ошибок

        if (!glfwInit()) {
            System.err.println("Failed to initialize GLFW");
            System.exit(1);
        }

        Window window = new Window(640, 420, "Sunny land!"); //Создания объекта окна из класса окна
        // Проверка библиотек
        window.windowInit();//Запуск метода инициализации окна

        if (window.getWindow() == NULL) {
            System.err.println("Failed to create GLFW window");
            glfwTerminate();
            System.exit(1);//проверка если окно не запустилось
        }

        glfwMakeContextCurrent(window.getWindow());//создаем контекст окна и делаем его активным

        // Инициализация GLAD → в LWJGL: GL.createCapabilities()
        try {
            GL.createCapabilities();
        } catch (IllegalStateException e) {
            System.err.println("Failed to initialize GLAD");
            System.exit(1);//инициализируем GLAD и проверяем на ошибку
        }

        glEnable(GL_BLEND);//Включаем GL_BLEND чтобы цвета могли смешиваться по Z
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//разрешаем смешивать цвета по прозрачности объектов

        int shaderprogram = Shaders.compileShader();

        glfwSwapInterval(1);//Настраиваем интервал смены кадров, чтобы не рисовать кадры которые и так не видно.

        Cursor cursor = new Cursor();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //заполнение экрана цветом

        glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        int width, height;//создаем переменные для высоты и ширины окна
        // GameObject — кандидат на Builder: texture/size/pos настраиваются по шагам
        GameObject player = GameObject.builder()
                .texture(TextureLoader.loadTexture("../assets/player.png"))
                .build();
        System.out.println("cursor size x: " + cursor.size.x + " y: " + cursor.size.y);
        cursor.setTexture(TextureLoader.loadTexture("../assets/cursor.png"));

        try (MemoryStack stack = MemoryStack.stackPush()) {
            IntBuffer w = stack.mallocInt(1);
            IntBuffer h = stack.mallocInt(1);
            glfwGetFramebufferSize(window.getWindow(), w, h);
            width = w.get(0);
            height = h.get(0);
        }

        float pixelx = 2.0f / width;
        float pixely = 2.0f / height;
        cursor.setSize(64 * 8 * pixelx, 64 * 8 * pixely);
        cursor.updateVertices();

        GameObject ball = GameObject.builder()
                .texture(TextureLoader.loadTexture("../assets/ball.png"))
                .size(64.0f * 4 * window.pixel_x, 64.0f * 4 * window.pixel_y)
                .build();
        double mouseX = 0, mouseY = 0;
        System.out.println("cursor size x: " + cursor.size.x + " y: " + cursor.size.y);

        float speedx = 0.01f;
        float speedy = 0.005f;
        int[] frames = new int[41];
        String characterpath = "../assets/character/idle/bb";
        for (int i = 1; i < 41; i++) {
            String path = characterpath + i + ".png";
            frames[i] = TextureLoader.loadTexture(path);
        }
        int currentframe = 0;

        while (!glfwWindowShouldClose(window.getWindow()))//Главный цикл. Работает и ждет пока окно не будет закрыто
        {
            try (MemoryStack stack = MemoryStack.stackPush()) {
                IntBuffer w = stack.mallocInt(1);
                IntBuffer h = stack.mallocInt(1);
                glfwGetFramebufferSize(window.getWindow(), w, h);//получаем размер буфера окна
                width = w.get(0);
                height = h.get(0);
            }
            glViewport(0, 0, width, height);//создаем вьюпорт для игры
            glClear(GL_COLOR_BUFFER_BIT);//когда берем новый буфер то заполняем его цветом.
            glUseProgram(shaderprogram);

            try (MemoryStack stack = MemoryStack.stackPush()) {
                DoubleBuffer xpos = stack.mallocDouble(1);
                DoubleBuffer ypos = stack.mallocDouble(1);
                glfwGetCursorPos(window.getWindow(), xpos, ypos);
                mouseX = xpos.get(0);
                mouseY = ypos.get(0);
            }

            // Рисуем только контуры (линии) вместо заливки
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            player.draw();
            ball.draw();

            cursor.setSize(64.0f * 4 * window.pixel_x, 64.0f * 4 * window.pixel_y);
            ball.setSize(64.0f * 1 * window.pixel_x, 64.0f * 1 * window.pixel_y);
            player.setSize(64.0f * 4 * window.pixel_x, 64.0f * 4 * window.pixel_y);
            ball.setPosition(ball.pos.x += speedx, ball.pos.y += speedy);
            if (Math.abs(ball.pos.x) >= 0.6f) {
                System.out.println("hit!");
                speedx = speedx * -1;
            }
            if (Math.abs(ball.pos.y) >= 0.6f) {
                System.out.println("hit!");
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
        // в Java деструктор Window не вызывается автоматически — завершаем без terminate, как в C++ (закомментировано)
        System.exit(0);//возвращаем успешный выход из игры
    }
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
