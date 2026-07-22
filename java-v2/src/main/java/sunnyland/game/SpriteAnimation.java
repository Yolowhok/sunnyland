package sunnyland.game;

import sunnyland.engine.Texture;

/** Time-based frame animation over a list of textures. */
public final class SpriteAnimation {
    private final Texture[] frames;
    private final float frameDurationSeconds;
    private float elapsed;
    private int index;

    public SpriteAnimation(Texture[] frames, float framesPerSecond) {
        if (frames == null || frames.length == 0) {
            throw new IllegalArgumentException("frames must not be empty");
        }
        this.frames = frames;
        this.frameDurationSeconds = 1f / framesPerSecond;
    }

    public void update(float deltaSeconds) {
        elapsed += deltaSeconds;
        while (elapsed >= frameDurationSeconds) {
            elapsed -= frameDurationSeconds;
            index = (index + 1) % frames.length;
        }
    }

    public Texture current() {
        return frames[index];
    }

    public void reset() {
        elapsed = 0f;
        index = 0;
    }
}
