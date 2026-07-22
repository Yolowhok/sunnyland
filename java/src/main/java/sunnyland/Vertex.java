package sunnyland;

/** Аналог struct Vertex из Vertex.h */
public class Vertex {
    public Vec2 pos; //Координаты позиции
    public Vec2 tex; //Координаты текстуры

    public Vertex() {
        this.pos = new Vec2();
        this.tex = new Vec2();
    }

    public Vertex(Vec2 pos, Vec2 tex) {
        this.pos = pos;
        this.tex = tex;
    }

    /** Размер структуры в байтах: 4 float = 16 */
    public static final int SIZEOF = 4 * Float.BYTES;
    public static final int OFFSET_POS = 0;
    public static final int OFFSET_TEX = 2 * Float.BYTES;
}
