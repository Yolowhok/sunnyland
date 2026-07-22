package sunnyland.engine;

import org.lwjgl.glfw.GLFWVidMode;
import org.lwjgl.opengl.GL;
import org.lwjgl.system.MemoryStack;

import java.nio.IntBuffer;
import java.util.Objects;

import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.glViewport;
import static org.lwjgl.system.MemoryUtil.NULL;

/**
 * Owns the GLFW window and OpenGL context lifecycle.
 * Game code should not call GLFW directly for window creation.
 */
public final class Window implements AutoCloseable {
    private final long handle;
    private final String title;
    private int windowWidth;
    private int windowHeight;
    private int framebufferWidth;
    private int framebufferHeight;
    private boolean windowed = true;
    private int windowedX, windowedY, windowedWidth, windowedHeight;
    private float pixelX;
    private float pixelY;

    public Window(int width, int height, String title) {
        this.windowWidth = width;
        this.windowHeight = height;
        this.title = Objects.requireNonNull(title);

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

        handle = glfwCreateWindow(width, height, title, NULL, NULL);
        if (handle == NULL) {
            throw new IllegalStateException("Failed to create GLFW window");
        }

        glfwSetKeyCallback(handle, this::onKey);
        glfwSetFramebufferSizeCallback(handle, this::onFramebufferResize);

        try (MemoryStack stack = MemoryStack.stackPush()) {
            IntBuffer x = stack.mallocInt(1);
            IntBuffer y = stack.mallocInt(1);
            glfwGetWindowPos(handle, x, y);
            windowedX = x.get(0);
            windowedY = y.get(0);
            IntBuffer w = stack.mallocInt(1);
            IntBuffer h = stack.mallocInt(1);
            glfwGetWindowSize(handle, w, h);
            windowedWidth = w.get(0);
            windowedHeight = h.get(0);
        }

        glfwMakeContextCurrent(handle);
        glfwSwapInterval(1);
        GL.createCapabilities();
        refreshFramebufferSize();
    }

    private void onKey(long win, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(win, true);
        }
        if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
            toggleFullscreen();
        }
    }

    private void onFramebufferResize(long win, int width, int height) {
        framebufferWidth = Math.max(width, 1);
        framebufferHeight = Math.max(height, 1);
        pixelX = 2.0f / framebufferWidth;
        pixelY = 2.0f / framebufferHeight;
        glViewport(0, 0, framebufferWidth, framebufferHeight);
    }

    private void refreshFramebufferSize() {
        try (MemoryStack stack = MemoryStack.stackPush()) {
            IntBuffer w = stack.mallocInt(1);
            IntBuffer h = stack.mallocInt(1);
            glfwGetFramebufferSize(handle, w, h);
            onFramebufferResize(handle, w.get(0), h.get(0));
        }
    }

    public void toggleFullscreen() {
        if (windowed) {
            try (MemoryStack stack = MemoryStack.stackPush()) {
                IntBuffer x = stack.mallocInt(1);
                IntBuffer y = stack.mallocInt(1);
                glfwGetWindowPos(handle, x, y);
                windowedX = x.get(0);
                windowedY = y.get(0);
                IntBuffer w = stack.mallocInt(1);
                IntBuffer h = stack.mallocInt(1);
                glfwGetWindowSize(handle, w, h);
                windowedWidth = w.get(0);
                windowedHeight = h.get(0);
            }
            long monitor = glfwGetPrimaryMonitor();
            GLFWVidMode mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(handle, monitor, 0, 0, mode.width(), mode.height(), mode.refreshRate());
            windowed = false;
        } else {
            glfwSetWindowMonitor(handle, NULL, windowedX, windowedY, windowedWidth, windowedHeight, 0);
            windowed = true;
        }
    }

    public void pollEvents() {
        glfwPollEvents();
    }

    public void swapBuffers() {
        glfwSwapBuffers(handle);
    }

    public boolean shouldClose() {
        return glfwWindowShouldClose(handle);
    }

    public long handle() {
        return handle;
    }

    public int framebufferWidth() {
        return framebufferWidth;
    }

    public int framebufferHeight() {
        return framebufferHeight;
    }

    /** NDC size of one framebuffer pixel (matches original pixel_x / pixel_y). */
    public float pixelX() {
        return pixelX;
    }

    public float pixelY() {
        return pixelY;
    }

    public String title() {
        return title;
    }

    @Override
    public void close() {
        if (handle != NULL) {
            glfwDestroyWindow(handle);
        }
    }
}
