#include "raylib.h"
#include "stdio.h"
#include <string.h>

#define SELECTED_TEXT (Color){229,228,250,255}
#define UNSELECTED_TEXT (Color){170,171,198,255}
#define TERTIARY_TEXT (Color){64,66,109,255}

int main() {
    const int screenWidth = 1024;
    const int screenHeight = 600;  

    // CNC Values
    const int MAX_SPINDLE_RPM = 20000;
    int spindleRPM = 20000;

    // GUI/Program Values
    const int DELTA_LERP = 5;
    int tabHighlightYPos = 0;
    int targetTabHighlightYPos = 0;    
    char spindleRPMText[6]; // Updated size to fit large numbers and null terminator
    char spindleRPMPercentageText[8]; // 000.00%

    SetConfigFlags(FLAG_MSAA_4X_HINT); // Anti-Aliasing
    InitWindow(screenWidth, screenHeight, "CNC Program");

    // Loading Fonts
    Font regular15 = LoadFontEx("JosefinSans-Regular.ttf", 15, 0, 0);
    Font semiBold = LoadFontEx("JosefinSans-SemiBold.ttf", 30, 0, 0);
    Font bold15 = LoadFontEx("JosefinSans-Bold.ttf", 15, 0, 0);
    Font bold25 = LoadFontEx("JosefinSans-Bold.ttf", 25, 0, 0);

    // Calculating Font Positions (Unchanging; 4Y Offset) 
    int spindleTextSizeX = 125 - (MeasureTextEx(bold15, "SPINDLE RPM", 15, 0).x / 2);
    int fileTextSizeY = 29 - (MeasureTextEx(bold25, "FILE", 25, 0).y / 2);
    int probeTextSizeY = 79 - (MeasureTextEx(bold25, "PROBE", 25, 0).y / 2);
    int toolTextSizeY = 129 - (MeasureTextEx(bold25, "TOOL", 25, 0).y / 2);
    int runTextSizeY = 179 - (MeasureTextEx(bold25, "RUN", 25, 0).y / 2);

    SetTargetFPS(100);    
    if (IsFontReady(bold25) && IsFontReady(bold15) && IsFontReady(semiBold) && IsFontReady(regular15)) {         
        while (!WindowShouldClose()) {        
            BeginDrawing();
            ClearBackground(RAYWHITE);        

            // Lower Dashboard      
            sprintf(spindleRPMText, "%d", spindleRPM); // Display RPM as a regular integer
            Vector2 spindleRPMTextSize = MeasureTextEx(semiBold, spindleRPMText, 30, 0);
            sprintf(spindleRPMPercentageText, "%d%%", (100 * spindleRPM) / MAX_SPINDLE_RPM); // Proper percentage calculation as int
            Vector2 spindleRPMPercentageTextSize = MeasureTextEx(regular15, spindleRPMPercentageText, 15, 0);

            DrawRectangle(0, 350, 1024, 250, (Color){43, 44, 64, 255}); // Background   
            DrawRectangleGradientH(217, 383, 200, 50, (Color){31, 31, 44, 255}, (Color){43, 44, 64, 255}); // Feed Override            
            DrawCircle(125, 475, 130, (Color){43, 44, 64, 255});            
            DrawRing((Vector2){125, 475}, 125, 130, 315, 405, 40, (Color){241, 240, 255, 255}); 
            DrawRing((Vector2){125, 475}, 70, 100, 135, 405, 40, (Color){241, 240, 255, 255}); // Spindle Override
            DrawCircle(125, 475, 62.5, (Color){35, 36, 53, 255});            
            DrawTextEx(bold15, "SPINDLE RPM", (Vector2){spindleTextSizeX, 560}, 15, 0, SELECTED_TEXT);  
            DrawTextEx(regular15, spindleRPMPercentageText, (Vector2){125 - (spindleRPMPercentageTextSize.x / 2), 500}, 15, 0, TERTIARY_TEXT);
            DrawTextEx(semiBold, spindleRPMText, (Vector2){125 - (spindleRPMTextSize.x / 2), 475 - (spindleRPMTextSize.y / 2)}, 30, 0, SELECTED_TEXT);              
            DrawRing((Vector2){125, 475}, 70, 100, 135, (135 + (270 * spindleRPM / MAX_SPINDLE_RPM)), 40, PURPLE);
            
            // Display Area
            DrawRectangle(200, 0, 824, 350, (Color){148, 149, 177, 255});

            // Side Tabs
            DrawRectangle(0, 0, 200, 350, (Color){82, 83, 109, 255});         
            DrawRectangleGradientH(0, tabHighlightYPos, 200, 50, (Color){82, 83, 109, 255}, (Color){148, 149, 177, 255}); // Highlight Icon
            
            // Tab color animation
            const char* tabNames[] = {"FILE", "PROBE", "TOOL", "RUN"};
            int tabPositionsY[] = {fileTextSizeY, probeTextSizeY, toolTextSizeY, runTextSizeY};
            for (int i = 0; i < 4; i++) {
                Color textColor = (tabHighlightYPos == i * 50) ? SELECTED_TEXT : UNSELECTED_TEXT;
                DrawTextEx(bold25, tabNames[i], (Vector2){20, tabPositionsY[i]}, 25, 0, textColor);
            }

            // Draw Tab Separators
            for (int i = 1; i < 4; i++) {
                DrawLineEx((Vector2){0, i * 50}, (Vector2){200, i * 50}, 1, (Color){58, 59, 84, 255});     
            }

            // Mouse Handler       
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePosition = GetMousePosition();
                if (mousePosition.x <= 200 && mousePosition.y < 350) { // Side Tab Area
                    int clickedTab = mousePosition.y / 50; // Determine which tab was clicked
                    if (clickedTab >= 0 && clickedTab < 4) {
                        targetTabHighlightYPos = clickedTab * 50;
                    }
                }
            }

            // Smoothing Animations
            if (tabHighlightYPos < targetTabHighlightYPos) {
                tabHighlightYPos += DELTA_LERP;
            } else if (tabHighlightYPos > targetTabHighlightYPos) {
                tabHighlightYPos -= DELTA_LERP;
            }

            // Testing Items (Mouse Scroll to Change RPM)
            int mouseScroll = (int)GetMouseWheelMove();
            if (mouseScroll > 0) {
                if (spindleRPM < MAX_SPINDLE_RPM) {
                    spindleRPM += mouseScroll * 500;
                }
            } else {
                if (spindleRPM >= 1) {
                    spindleRPM += mouseScroll * 500;
                }
            }    

            EndDrawing();
        }
    }
    
    UnloadFont(regular15);
    UnloadFont(semiBold);
    UnloadFont(bold15);
    UnloadFont(bold25);
    CloseWindow();
    
    return 0;
}
