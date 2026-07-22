package sunnyland.app;

import org.lwjgl.glfw.GLFWErrorCallback;
import sunnyland.engine.AssetManager;
import sunnyland.engine.GameLoop;
import sunnyland.engine.Input;
import sunnyland.engine.Renderer;
import sunnyland.engine.Window;
import sunnyland.game.SunnyLandGame;

import static org.lwjgl.glfw.GLFW.glfwInit;
import static org.lwjgl.glfw.GLFW.glfwTerminate;

/**
 * Composition root: wires engine + game, owns lifecycle.
 * No gameplay logic here.
 */
public final class Main {
    public static void main(String[] args) {
        System.out.println("Sunny Land! 0.2 (architecture)");

        GLFWErrorCallback.createPrint(System.err).set();
        if (!glfwInit()) {
            throw new IllegalStateException("Failed to initialize GLFW");
        }

        try (Window window = new Window(640, 420, "Sunny land!");
             AssetManager assets = new AssetManager();
             Renderer renderer = new Renderer();
             SunnyLandGame game = new SunnyLandGame(window, new Input(window), assets, renderer)) {

            new GameLoop(window, game).run();
        } finally {
            glfwTerminate();
        }
    }
}
