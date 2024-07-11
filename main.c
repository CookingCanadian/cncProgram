#include "raylib.h"
#include <stdio.h>

#define TEXT1_LIGHT (Color){228,229,241,255}
#define TEXT2_LIGHT (Color){210,211,219,255}
#define SELECTED_LIGHT (Color){197,198,210,255}
#define UNSELECTED_LIGHT (Color){147,148,165,255}

int darkMode = 0;
const int menuTabs = 3;

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

//Menu tabs for topbar
Button buttons[] = {
    {0,0,200,50, "ALIGN", SELECTED_LIGHT, TEXT1_LIGHT, true},
    {200,0,200,50, "TOOLS", UNSELECTED_LIGHT, TEXT1_LIGHT, false},
    {400,0,200,50, "DASHBOARD", UNSELECTED_LIGHT, TEXT1_LIGHT, false}
};


// Functions
void drawButton(Button *button);
void resetExcept(int skip);
bool checkButtonPressed(Button *button);

// Main
int main() {
    const int screenWidth = 1024;
    const int screenHeight = 600;    
    
    InitWindow(screenWidth, screenHeight, "cncProgram");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        //Topbar
        DrawRectangle(600,0,424,50,(Color){89,90,108,255});     
        DrawRectangle(0,50,1024,550,SELECTED_LIGHT);
       //Switch tabs
        for (int i = 0; i < menuTabs; i++) {
            if (checkButtonPressed(&buttons[i])) {
                if (!buttons[i].isPressed) {
                    buttons[i].isPressed = true;
                    buttons[i].backgroundColor = SELECTED_LIGHT;                  
                }
                switch(i) {
                    case 0: //Setup
                        resetExcept(i);                        
                        break;
                    case 1: //Tools
                        resetExcept(i);
                        break;
                    case 2: //Dashboard
                        resetExcept(i);                      
                        break;
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

void resetExcept(int skip) {
    for (int i = 0; i < menuTabs; i++) {
        if (i==skip){continue;}
        buttons[i].isPressed = false;
        buttons[i].backgroundColor = UNSELECTED_LIGHT;
    }
}

bool checkButtonPressed(Button *button) {
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){button->xPos, button->yPos, button->xSize, button->ySize})) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            return true;
        }
    }
    return false;
}
