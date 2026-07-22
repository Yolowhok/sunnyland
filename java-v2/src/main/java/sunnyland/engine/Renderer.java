package sunnyland.engine;

import static org.lwjgl.opengl.GL11.*;

/** Shared render setup for 2D sprites (blend, clear, viewport already on Window). */
public final class Renderer implements AutoCloseable {
    private final ShaderProgram shader;

    public Renderer() {
        shader = ShaderProgram.texturedQuad();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    }

    public void beginFrame() {
        glClear(GL_COLOR_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        shader.bind();
    }

    public void endFrame() {
        shader.unbind();
    }

    public void draw(Sprite sprite) {
        sprite.draw();
    }

    @Override
    public void close() {
        shader.close();
    }
}
