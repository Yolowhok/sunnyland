package sunnyland;

/** Простой аналог glm::vec2 */
public class Vec2 {
    public float x;
    public float y;

    public Vec2() {
        this(0.0f, 0.0f);
    }

    public Vec2(float x, float y) {
        this.x = x;
        this.y = y;
    }

    public Vec2(float v) {
        this(v, v);
    }
}
