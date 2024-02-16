#pragma once

#include <string>
#include <functional>
#include <geomui/component.hpp>

namespace geomui {

typedef std::function<RenderComponent(Viewport &viewport)> RootComponent;

class Window {
private:
  std::string title;
  RootComponent root;
  Viewport viewport;

  double previousTime;
  double currentTime;
  double updateDrawTime;
  double waitTime;
  double deltaTime;
  int targetFPS = 600;

  bool _closed = false;

public:
  Window(std::string title, RootComponent root, Viewport&& viewport);

  bool closed() { return _closed; }

  void render();
};

} // namespace geomui
