#include "cursor.h"

void Cursor::mousePos(double mouseX, double mouseY, int winWidth, int winHeight) {
    pos.x = static_cast<float>((2.0f * mouseX) / winWidth - 1.0f);
    pos.y = static_cast<float>(1.0f - (2.0f * mouseY) / winHeight);
    setPosition(pos.x, pos.y);
}
