package sunnyland.game;

import sunnyland.engine.Sprite;
import sunnyland.engine.Texture;
import sunnyland.engine.Vec2;

/**
 * Gameplay entity: transform + optional sprite.
 * No OpenGL calls here — rendering goes through Sprite.
 */
public class Entity {
    protected final Vec2 position = new Vec2();
    protected final Vec2 size = new Vec2(1f, 1f);
    protected final Sprite sprite = new Sprite();

    public Vec2 position() {
        return position;
    }

    public Vec2 size() {
        return size;
    }

    public Sprite sprite() {
        return sprite;
    }

    public void setTexture(Texture texture) {
        sprite.setTexture(texture);
    }

    public void setPosition(float x, float y) {
        position.set(x, y);
        syncSprite();
    }

    public void setSize(float width, float height) {
        size.set(width, height);
        syncSprite();
    }

    /** Push entity transform into GPU sprite mesh. */
    public void syncSprite() {
        sprite.setTransform(position.x, position.y, size.x, size.y);
    }

    public void update(float deltaSeconds) {
        // override in subclasses
    }

    public void dispose() {
        sprite.close();
    }
}
