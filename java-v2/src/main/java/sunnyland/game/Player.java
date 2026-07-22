package sunnyland.game;

import sunnyland.engine.Input;

import static org.lwjgl.glfw.GLFW.*;

public final class Player extends Entity {
    private final float moveSpeedNdcPerSecond;
    private SpriteAnimation idleAnimation;

    public Player(float moveSpeedNdcPerSecond) {
        this.moveSpeedNdcPerSecond = moveSpeedNdcPerSecond;
    }

    public void setIdleAnimation(SpriteAnimation animation) {
        this.idleAnimation = animation;
        if (animation != null) {
            setTexture(animation.current());
        }
    }

    public void handleInput(Input input, float deltaSeconds) {
        float step = moveSpeedNdcPerSecond * deltaSeconds;
        if (input.isKeyDown(GLFW_KEY_W)) {
            position.y += step;
        }
        if (input.isKeyDown(GLFW_KEY_S)) {
            position.y -= step;
        }
        if (input.isKeyDown(GLFW_KEY_A)) {
            position.x -= step;
        }
        if (input.isKeyDown(GLFW_KEY_D)) {
            position.x += step;
        }
    }

    @Override
    public void update(float deltaSeconds) {
        if (idleAnimation != null) {
            idleAnimation.update(deltaSeconds);
            setTexture(idleAnimation.current());
        }
        syncSprite();
    }
}
