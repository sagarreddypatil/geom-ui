#pragma once

#include <functional>

namespace geomui {

class Viewport {
public:
    double mouseX, mouseY, width, height;
    Viewport() : mouseX(0), mouseY(0), width(0), height(0) {}

    Viewport(double mouseX, double mouseY, double width, double height)
        : mouseX(mouseX), mouseY(mouseY), width(width), height(height) {}

    Viewport(double width, double height)
        : mouseX(0), mouseY(0), width(width), height(height) {}

    void update(double mouseX, double mouseY, double width, double height) {
        this->mouseX = mouseX;
        this->mouseY = mouseY;
        this->width = width;
        this->height = height;
    }
};

typedef std::function<void()> RenderComponent;

} // namespace geomui