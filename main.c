#include "raylib.h"

int darkMode = 0;

//Functions
void drawButton(int xSize, int ySize, int xPos, int yPos, char Name[], int RGB[]);

//Main
int main() {
    const int screenWidth = 1024;
    const int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "cncProgram");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {      
        BeginDrawing();

        ClearBackground(RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void drawButton(int xSize, int ySize, int xPos, int yPos, char Name[], int RGB[]) {
    if (darkMode==0) {
        //lightMode
    } else {
        //dark
    }
}