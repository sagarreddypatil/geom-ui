#include <iostream>
#include <raylib.h>
#include <geomui.hpp>

geomui::RenderComponent root(geomui::Viewport& viewport) {
    auto mousePosText = "Mouse Position: (" + std::to_string(viewport.mouseX) + ", " + std::to_string(viewport.mouseY) + ")";
    double strWidth = MeasureText(mousePosText.c_str(), 20);

    geomui::Var offset;
    offset + (strWidth / 2) |= viewport.width / 2;

    double offsetValue = offset->eval();

    return [mousePosText, offsetValue]() {
        ClearBackground(BLACK);
        DrawText(mousePosText.c_str(), offsetValue, 10, 20, WHITE);
    };
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "geomui test");

    geomui::Viewport viewport;
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