#include <iostream>
#include <raylib.h>
#include <geomui.hpp>

using namespace geomui;

RenderComponent root(Viewport& viewport) {
    auto mousePosText = "Mouse Position: (" + std::to_string(viewport.mouseX) + ", " + std::to_string(viewport.mouseY) + ")";
    double strWidth = MeasureText(mousePosText.c_str(), 20);

    geomui::Rectangle vpRect;
    coincedent(vpRect.tl, Point(0, 0));
    coincedent(vpRect.br, Point(viewport.width, viewport.height));

    Point mdPoint;
    midpoint(vpRect.top, mdPoint);

    double textX = mdPoint.x->eval() - strWidth / 2;
    // double textX = vpRect.wVar->eval() / 2;

    return [mousePosText, textX]() {
        ClearBackground(BLACK);
        DrawText(mousePosText.c_str(), textX, 10, 20, WHITE);
    };
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "geomui test");

    Viewport viewport;
    Vector2 mousePos;

    while (!WindowShouldClose())
    {
        mousePos = GetMousePosition();
        viewport.update(mousePos.x, mousePos.y, GetScreenWidth(), GetScreenHeight());

        auto renderRoot = root(viewport);

        BeginDrawing();

        renderRoot();

        EndDrawing();
    }

    CloseWindow();

    return 0;

}