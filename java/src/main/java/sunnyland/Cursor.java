package sunnyland;

public class Cursor extends GameObject {
    public Cursor() {
        super();
    }

    public void mousePos(double mouseX, double mouseY, int winWidth, int winHeight) {
        pos.x = (float) ((2.0f * mouseX) / winWidth - 1.0f);
        pos.y = (float) (1.0f - (2.0f * mouseY) / winHeight);
        setPosition(pos.x, pos.y);
    }
}
