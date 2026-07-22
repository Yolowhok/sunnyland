package sunnyland.game;

/** Bouncing ball with simple axis-aligned NDC bounds. */
public final class Ball extends Entity {
    private float velocityX;
    private float velocityY;
    private final float bounceLimit;

    public Ball(float velocityX, float velocityY, float bounceLimit) {
        this.velocityX = velocityX;
        this.velocityY = velocityY;
        this.bounceLimit = bounceLimit;
    }

    @Override
    public void update(float deltaSeconds) {
        position.x += velocityX * deltaSeconds;
        position.y += velocityY * deltaSeconds;

        if (Math.abs(position.x) >= bounceLimit) {
            System.out.println("hit!");
            velocityX = -velocityX;
            position.x = Math.copySign(bounceLimit, position.x);
        }
        if (Math.abs(position.y) >= bounceLimit) {
            System.out.println("hit!");
            velocityY = -velocityY;
            position.y = Math.copySign(bounceLimit, position.y);
        }
        syncSprite();
    }
}
