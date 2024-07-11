#include "raylib.h"

#define TEXT1_LIGHT (Color){228,229,241, 255}
#define TEXT2_LIGHT (Color){210,211,219, 255}
#define MUTED_LIGHT (Color){147,148,165, 255}
#define DARK_LIGHT (Color){72,75,106, 255}

typedef struct Button {
    int xPos;
    int yPos;
    int xSize;
    int ySize;
    const char *Text;
    Color backgroundColor;
    Color textColor;
    bool isPressed;
} Button;

int darkMode = 0;
int selectedTab = 0;

// Functions
void drawButton(Button *button);
bool checkButtonPressed(Button *button);

// Main
int main() {
    const int screenWidth = 1024;
    const int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "cncProgram");
    SetTargetFPS(60);

    Button homeButton = {0, 0, 100, 50, "Test", MUTED_LIGHT, TEXT1_LIGHT, false};

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (checkButtonPressed(&homeButton)) {
            homeButton.isPressed = !homeButton.isPressed;  // Toggle button state
            homeButton.backgroundColor = homeButton.isPressed ? DARK_LIGHT : MUTED_LIGHT;  // Change color based on state
        }

        drawButton(&homeButton);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void drawButton(Button *button) {
    DrawRectangle(button->xPos, button->yPos, button->xSize, button->ySize, button->backgroundColor);

    int textWidth = MeasureText(button->Text, 20); // 20 is the font size
    int textHeight = 20; // 20 is the font size    
    int textXPos = button->xPos + (button->xSize - textWidth) / 2;
    int textYPos = button->yPos + (button->ySize - textHeight) / 2;
    
    DrawText(button->Text, textXPos, textYPos, 20, button->textColor);
}

bool checkButtonPressed(Button *button) {
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){button->xPos, button->yPos, button->xSize, button->ySize})) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            return true;
        }
    }
    return false;
}
