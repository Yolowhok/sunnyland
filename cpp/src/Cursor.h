#ifndef CURSOR
#define CURSOR
#include "GameObject.h"
class Cursor : public GameObject{
public:
Cursor() : GameObject() {};
~Cursor() = default;
void mousePos(double mouseX, double mouseY, int winWidth, int winHeight);

};
#endif