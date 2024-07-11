#include "raylib.h"
#include <stdio.h>

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

    Button buttons[] = {
        {0, 0, 200, 50, "ALIGN", DARK_LIGHT, TEXT1_LIGHT, true},
        {200, 0, 200, 50, "TOOLS", MUTED_LIGHT, TEXT1_LIGHT, false},
        {400, 0, 200, 50, "DASHBOARD", MUTED_LIGHT, TEXT1_LIGHT, false}
    };
    int numButtons = sizeof(buttons) / sizeof(buttons[0]);



    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < numButtons; i++) {
            if (checkButtonPressed(&buttons[i])) {
                if (!buttons[i].isPressed) {
                    buttons[i].isPressed = true;
                    buttons[i].backgroundColor = DARK_LIGHT;                  
                }
                for (int j = 0; j <numButtons; j++) {
                    if (j != i) {
                        buttons[j].isPressed = false;
                        buttons[j].backgroundColor = MUTED_LIGHT;      
                    }
                }               
            } 
            drawButton(&buttons[i]);
        }
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
