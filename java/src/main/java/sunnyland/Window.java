package sunnyland;

import org.lwjgl.glfw.GLFWVidMode;
import org.lwjgl.opengl.GL11;

import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.system.MemoryUtil.NULL;

public class Window {
    private boolean isWindowed;
    private int height;
    private int width;

    private String title;
    private long window;
    private int windowedX, windowedY, windowedWidth, windowedHeight;

    public float pixel_x = 2.0f / 640;
    public float pixel_y = 2.0f / 420;

    public Window(int width, int height, String title) {
        this.width = width;
        this.height = height;
        this.title = title;
        this.isWindowed = true;
        this.window = NULL;
        System.out.println("window object created!");
    }

    public void windowInit() {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // macOS требует forward-compatible core profile
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        this.window = glfwCreateWindow(width, height, title, NULL, NULL);

        if (window == NULL) {
            System.err.println("Failed to create GLFW window");
            return;
        }
        glfwSetFramebufferSizeCallback(window, Window::framebufferSizeCallback);
        // Привязываем указатель на текущий объект Window к GLFW-окну (через registry — аналог user pointer)
        WindowRegistry.put(window, this);
        // Устанавливаем статический колбэк
        glfwSetKeyCallback(window, Window::keyboardCallback);
        int[] xpos = new int[1];
        int[] ypos = new int[1];
        glfwGetWindowPos(window, xpos, ypos);
        windowedX = xpos[0];
        windowedY = ypos[0];
        int[] w = new int[1];
        int[] h = new int[1];
        glfwGetWindowSize(window, w, h);
        windowedWidth = w[0];
        windowedHeight = h[0];
    }

    public void switchWindowMode() {
        if (isWindowed) {
            int[] xpos = new int[1];
            int[] ypos = new int[1];
            glfwGetWindowPos(window, xpos, ypos);
            windowedX = xpos[0];
            windowedY = ypos[0];
            int[] w = new int[1];
            int[] h = new int[1];
            glfwGetWindowSize(window, w, h);
            windowedWidth = w[0];
            windowedHeight = h[0];
            long monitor = glfwGetPrimaryMonitor();
            GLFWVidMode mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(window, monitor, 0, 0, mode.width(), mode.height(), mode.refreshRate());
            isWindowed = false;
            System.out.println("Fullscreen mode toggled!");
        } else {
            glfwSetWindowMonitor(window, NULL, windowedX, windowedY, windowedWidth, windowedHeight, 0);
            isWindowed = true;
            System.out.println("Window mode toggled!");
        }
    }

    public static void keyboardCallback(long glfwWin, int key, int scancode, int action, int mods) {
        Window win = WindowRegistry.get(glfwWin);
        if (win == null) return;

        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(glfwWin, true);
        }
        if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
            win.switchWindowMode();
        }
    }

    public static void framebufferSizeCallback(long glfwWin, int newWidth, int newHeight) {
        Window win = WindowRegistry.get(glfwWin);
        if (win != null) {
            win.width = newWidth;
            win.height = newHeight;
            GL11.glViewport(0, 0, newWidth, newHeight);
            win.pixel_x = 2.0f / newWidth;//Для разрешения пикселей.
            win.pixel_y = 2.0f / newHeight;//Для разрешения пикселей.
        }
    }

    public long getWindow() {
        return window;
    }

    // объявлены в C++ заголовке, тел нет — заглушки как в оригинале
    public void pixelResolution(int width, int height) {
    }

    public boolean getPress() {
        return false;
    }

    public void destroy() {
        if (window != NULL) {
            WindowRegistry.remove(window);
            glfwDestroyWindow(window);
            window = NULL;
            glfwTerminate();
        }
    }
}
