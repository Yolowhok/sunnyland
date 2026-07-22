package sunnyland.game;

import sunnyland.engine.AssetManager;
import sunnyland.engine.GameLoop;
import sunnyland.engine.Input;
import sunnyland.engine.Renderer;
import sunnyland.engine.Texture;
import sunnyland.engine.Window;

/**
 * Scene / gameplay root: loads assets, owns entities, implements update/render.
 */
public final class SunnyLandGame implements GameLoop.Listener, AutoCloseable {
    /** Original per-frame values assumed ~60 FPS → converted to per-second. */
    private static final float PLAYER_SPEED = 0.003f * 60f;
    private static final float BALL_SPEED_X = 0.01f * 60f;
    private static final float BALL_SPEED_Y = 0.005f * 60f;
    private static final float BOUNCE_LIMIT = 0.6f;
    private static final float ANIMATION_FPS = 60f;

    private final Window window;
    private final Input input;
    private final AssetManager assets;
    private final Renderer renderer;

    private final Player player;
    private final Ball ball;
    private final GameCursor cursor;

    public SunnyLandGame(Window window, Input input, AssetManager assets, Renderer renderer) {
        this.window = window;
        this.input = input;
        this.assets = assets;
        this.renderer = renderer;

        input.setCursorHidden(true);

        player = new Player(PLAYER_SPEED);
        Texture[] idleFrames = new Texture[40];
        for (int i = 0; i < 40; i++) {
            idleFrames[i] = assets.getTexture("assets/character/idle/bb" + (i + 1) + ".png");
        }
        player.setIdleAnimation(new SpriteAnimation(idleFrames, ANIMATION_FPS));
        // fallback / initial size in pixel-NDC units (4 * 64 px), same idea as original
        player.setSize(64f * 4f * window.pixelX(), 64f * 4f * window.pixelY());

        ball = new Ball(BALL_SPEED_X, BALL_SPEED_Y, BOUNCE_LIMIT);
        ball.setTexture(assets.getTexture("assets/ball.png"));
        ball.setSize(64f * 1f * window.pixelX(), 64f * 1f * window.pixelY());

        cursor = new GameCursor();
        cursor.setTexture(assets.getTexture("assets/cursor.png"));
        cursor.setSize(64f * 4f * window.pixelX(), 64f * 4f * window.pixelY());
    }

    @Override
    public void update(float deltaSeconds) {
        // keep sprite sizes tied to current pixel scale (resize-safe)
        float px = window.pixelX();
        float py = window.pixelY();
        player.setSize(64f * 4f * px, 64f * 4f * py);
        ball.setSize(64f * 1f * px, 64f * 1f * py);
        cursor.setSize(64f * 4f * px, 64f * 4f * py);

        player.handleInput(input, deltaSeconds);
        player.update(deltaSeconds);
        ball.update(deltaSeconds);
        cursor.followMouse(input, window);
    }

    @Override
    public void render() {
        renderer.beginFrame();
        renderer.draw(player.sprite());
        renderer.draw(ball.sprite());
        renderer.draw(cursor.sprite());
        renderer.endFrame();
    }

    @Override
    public void close() {
        player.dispose();
        ball.dispose();
        cursor.dispose();
    }
}
