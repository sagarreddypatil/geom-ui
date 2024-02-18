#include "geomui.hpp"
#include <raylib.h>

namespace geomui {

RenderComponent Point::draw() {
  int x = this->x->evalInt(), y = this->y->evalInt();

  return [x, y]() {
    DrawCircle(x, y, 2, WHITE);
  };
}

RenderComponent Line::draw() {
  int x1 = this->p1.x->evalInt(),
      y1 = this->p1.y->evalInt(),
      x2 = this->p2.x->evalInt(),
      y2 = this->p2.y->evalInt();

  return [x1, y1, x2, y2]() { DrawLine(x1, y1, x2, y2, WHITE); };
}

RenderComponent Rectangle::draw() {
  int x = this->tl.x->evalInt(),
      y = this->tl.y->evalInt(),
      w = this->wVar->evalInt(),
      h = this->hVar->evalInt();

  return [x, y, w, h]() { DrawRectangleLines(x, y, w, h, WHITE); };
}

}