package sunnyland.game;

import sunnyland.engine.Input;
import sunnyland.engine.Window;

/** Custom cursor entity that follows mouse in NDC. */
public final class GameCursor extends Entity {

    public void followMouse(Input input, Window window) {
        double[] mouse = new double[2];
        input.readMouse(mouse);
        float ndcX = (float) ((2.0 * mouse[0]) / window.framebufferWidth() - 1.0);
        float ndcY = (float) (1.0 - (2.0 * mouse[1]) / window.framebufferHeight());
        setPosition(ndcX, ndcY);
    }
}
