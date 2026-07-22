package sunnyland.engine;

/**
 * Fixed-timestep-friendly loop shell: measures delta seconds and drives
 * update → render → present.
 */
public final class GameLoop {
    public interface Listener {
        void update(float deltaSeconds);

        void render();
    }

    private final Window window;
    private final Listener listener;

    public GameLoop(Window window, Listener listener) {
        this.window = window;
        this.listener = listener;
    }

    public void run() {
        double last = org.lwjgl.glfw.GLFW.glfwGetTime();
        while (!window.shouldClose()) {
            double now = org.lwjgl.glfw.GLFW.glfwGetTime();
            float delta = (float) (now - last);
            last = now;
            // clamp huge spikes (debugger pause, etc.)
            if (delta > 0.05f) {
                delta = 0.05f;
            }

            window.pollEvents();
            listener.update(delta);
            listener.render();
            window.swapBuffers();
        }
    }
}
