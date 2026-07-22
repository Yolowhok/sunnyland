package sunnyland.engine;

/** GPU texture handle + metadata. Owned by AssetManager. */
public final class Texture implements AutoCloseable {
    private final int id;
    private final int width;
    private final int height;

    public Texture(int id, int width, int height) {
        this.id = id;
        this.width = width;
        this.height = height;
    }

    public int id() {
        return id;
    }

    public int width() {
        return width;
    }

    public int height() {
        return height;
    }

    @Override
    public void close() {
        org.lwjgl.opengl.GL11.glDeleteTextures(id);
    }
}
