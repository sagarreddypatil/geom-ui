#include <geomui.hpp>
#include <iostream>
#include <cmath>
#include <raylib.h>

using namespace geomui;

RenderComponent drawPoint(const geomui::Point &point) {
  int x = point.x->evalInt(), y = point.y->evalInt();

  return [x, y]() {
    DrawCircle(x, y, 2, WHITE);
  };
}

RenderComponent drawLine(const geomui::Line &line) {
  int x1 = line.p1.x->evalInt(),
      y1 = line.p1.y->evalInt(),
      x2 = line.p2.x->evalInt(),
      y2 = line.p2.y->evalInt();

  return [x1, y1, x2, y2]() { DrawLine(x1, y1, x2, y2, WHITE); };
}

RenderComponent drawRect(const geomui::Rectangle &rect) {
  int x = rect.tl.x->evalInt(),
      y = rect.tl.y->evalInt(),
      w = rect.wVar->evalInt(),
      h = rect.hVar->evalInt();

  return [x, y, w, h]() { DrawRectangleLines(x, y, w, h, WHITE); };
}

RenderComponent root(Viewport &viewport) {
  auto mousePosText = "Mouse Position: (" + std::to_string(viewport.mouseX) +
                      ", " + std::to_string(viewport.mouseY) + ")";
  double strWidth = MeasureText(mousePosText.c_str(), 20);

  geomui::Rectangle vpRect;
  coincedent(vpRect.tl, Point(0, 0));
  coincedent(vpRect.br, Point(viewport.width, viewport.height));

  geomui::Rectangle textRect;
  width(textRect.top, strWidth);
  height(textRect.left, 20);

  coincedent(vpRect.center, textRect.center);

  auto renderTextBox = drawRect(textRect);
  auto renderTextDiag = drawLine(textRect.diagonal);
  auto renderTextCenter = drawPoint(textRect.center);

  auto renderVpDiag = drawLine(vpRect.diagonal);
  auto renderVpCenter = drawPoint(vpRect.center);

  auto renderStuff = [renderTextBox, renderTextDiag, renderTextCenter, renderVpDiag, renderVpCenter]() {
    // renderTextBox();
    // renderTextDiag();
    // renderTextCenter();

    // renderVpDiag();
    // renderVpCenter();
  };

  double textX = textRect.tl.x->evalInt();
  double textY = textRect.tl.y->evalInt();

  std::cout << "textX: " << textRect.tl.x->eval() << "; int: " << textX << std::endl;

  return [mousePosText, textX, textY, renderStuff]() {
    ClearBackground(BLACK);

    renderStuff();

    DrawText(mousePosText.c_str(), textX, textY, 20, WHITE);
  };
}

int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(800, 600, "geomui test");

  Viewport viewport;
  Vector2 mousePos;

  while (!WindowShouldClose()) {
    mousePos = GetMousePosition();
    viewport.update(mousePos.x, mousePos.y, GetScreenWidth(),
                    GetScreenHeight());

    auto renderRoot = root(viewport);

    BeginDrawing();

    renderRoot();

    EndDrawing();
  }

  CloseWindow();

  return 0;
}