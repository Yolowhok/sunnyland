package sunnyland.engine;

import static org.lwjgl.glfw.GLFW.*;

/** Thin keyboard/mouse facade over GLFW for the active window. */
public final class Input {
    private final long window;

    public Input(Window window) {
        this.window = window.handle();
    }

    public boolean isKeyDown(int key) {
        return glfwGetKey(window, key) == GLFW_PRESS;
    }

    public void setCursorHidden(boolean hidden) {
        glfwSetInputMode(window, GLFW_CURSOR, hidden ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
    }

    /** Writes cursor position into xy[0]=x, xy[1]=y (window coordinates). */
    public void readMouse(double[] xy) {
        double[] x = new double[1];
        double[] y = new double[1];
        glfwGetCursorPos(window, x, y);
        xy[0] = x[0];
        xy[1] = y[0];
    }
}
