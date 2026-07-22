package sunnyland.engine;

/** 2D vector (engine math, not tied to OpenGL). */
public final class Vec2 {
    public float x;
    public float y;

    public Vec2() {
        this(0f, 0f);
    }

    public Vec2(float x, float y) {
        this.x = x;
        this.y = y;
    }

    public Vec2 set(float x, float y) {
        this.x = x;
        this.y = y;
        return this;
    }

    public Vec2 set(Vec2 other) {
        return set(other.x, other.y);
    }

    public Vec2 add(float dx, float dy) {
        x += dx;
        y += dy;
        return this;
    }

    public Vec2 copy() {
        return new Vec2(x, y);
    }
}
