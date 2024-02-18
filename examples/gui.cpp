#include <geomui.hpp>
#include <iostream>
#include <raylib.h>

using namespace geomui;

// RenderComponent text(std


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

  double textX = textRect.tl.x->evalInt();
  double textY = textRect.tl.y->evalInt();

  auto renderVpRect = vpRect.draw();
  auto renderTextRect = textRect.draw();

  auto renderList = [renderVpRect, renderTextRect] {
    renderVpRect();
    renderTextRect();
  };

  return [mousePosText, textX, textY, renderList]() {
    ClearBackground(BLACK);
    renderList();
    DrawText(mousePosText.c_str(), textX, textY, 20, WHITE);
  };
}

int main() {
  Window window("geomui test", root, Viewport(800, 600));

  while(!window.closed()) {
    window.render();
  }

  return 0;
}