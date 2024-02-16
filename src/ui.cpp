#include "geomui.hpp"
#include <raylib.h>

namespace geomui {

static bool windowCreated = false;

Window::Window(std::string title, RootComponent root, Viewport&& viewport)
    : title(title), root(root), viewport(viewport) {
  if (windowCreated) {
    throw std::runtime_error("Only one window can be created");
  }

  windowCreated = true;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(viewport.width, viewport.height, this->title.c_str());
}

void Window::render() {
  if (WindowShouldClose()) {
    if(!_closed)
      CloseWindow();
    _closed = true;

    return;
  }

  auto mousePos = GetMousePosition();
  viewport.update(mousePos.x, mousePos.y, GetScreenWidth(), GetScreenHeight());

  auto renderRoot = root(viewport);

  BeginDrawing();
  renderRoot();
  EndDrawing();
}

} // namespace geomui