#include <geomui.hpp>
#include <iostream>
#include <raylib.h>

using namespace geomui;

// RenderComponent text(std


RenderComponent root(Viewport &viewport) {
  geomui::Rectangle vpRect;
  coincedent(vpRect.tl, Point(0, 0));
  coincedent(vpRect.br, Point(viewport.width, viewport.height));

  geomui::Rectangle todo;
  geomui::Rectangle doing;
  geomui::Rectangle done;

  coincedent(todo.tl, vpRect.tl);
  coincedent(doing.tl, todo.tr);
  coincedent(done.tl, doing.tr);
  coincedent(done.br, vpRect.br);

  (1.0 / 3.0) * viewport.width |= doing.tl.x;
  (2.0 / 3.0) * viewport.width |= done.tl.x;

  // todo.wVar |= doing.wVar;
  // doing.wVar |= done.wVar;

  todo.hVar |= done.hVar;
  doing.hVar |= done.hVar;

  auto renderTodo = todo.draw();
  auto renderDoing = doing.draw();
  auto renderDone = done.draw();

  return [=]() {
    ClearBackground(BLACK);

    renderTodo();
    renderDoing();
    renderDone();
  };
}

int main() {
  Window window("geomui test", root, Viewport(800, 600));

  while(!window.closed()) {
    window.render();
  }

  return 0;
}