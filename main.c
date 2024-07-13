#include "raylib.h"
#include <stdio.h>

#include<string.h>

#define TEXT1_LIGHT (Color){228,229,241,255}
#define TEXT2_LIGHT (Color){210,211,219,255}
#define SELECTED_LIGHT (Color){197,198,210,255}
#define UNSELECTED_LIGHT (Color){147,148,165,255}

int darkMode = 0;

int main() {
    const int screenWidth = 1024;
    const int screenHeight = 600;  

    //CNC Values
    int spindleRPM = 0;

    SetConfigFlags(FLAG_MSAA_4X_HINT);//Anti-Aliasing
    InitWindow(screenWidth, screenHeight, "cncProgram");

    //Loading Fonts
    Font regular = LoadFontEx("JosefinSans-Regular.ttf",30,0,0);
    Font semiBold = LoadFontEx("JosefinSans-SemiBold.ttf",30,0,0);
    Font bold = LoadFontEx("JosefinSans-Bold.ttf",15,0,0);

    //Calculating Font Positions (Unchanging)
    Vector2 spindleTextSize = MeasureTextEx(bold,"SPINDLE RPM",15,0);
    int spindleTextSizeX = (spindleTextSize.x/2);
    
    SetTargetFPS(60);    
    if (IsFontReady(bold)&&IsFontReady(semiBold)&&IsFontReady(regular)) {         
        while (!WindowShouldClose()) {        
            BeginDrawing();
            ClearBackground(RAYWHITE);        
            //Lower Dashboard 
            DrawRectangle(0,350,1024,250,(Color){43,44,64,255});//Spindle Info
            DrawCircleSector((Vector2){125,475},100,140,400,40,(Color){217,217,217,255});
            DrawCircle(125,475,75,(Color){43,44,64,255});
            DrawCircle(125,475,62.5,(Color){35,36,53,255});
            char spindleRPMText[5];
            sprintf(spindleRPMText,"%d",spindleRPM);
            Vector2 spindleRPMTextSize = MeasureTextEx(semiBold,spindleRPMText,30,0);
            DrawTextEx(bold,"SPINDLE RPM",(Vector2){125-spindleTextSizeX,550},15,0,(Color){229,228,250,255});     
            DrawTextEx(semiBold,spindleRPMText,(Vector2){125-(spindleRPMTextSize.x/2),475-(spindleRPMTextSize.y/2)},30,0,(Color){229,228,250,255});        
            
            //Display Area
            DrawRectangle(200,0,824,350,(Color){148,149,177,255});

            //Side Tabs
            DrawRectangle(0,0,200,350,(Color){82,83,109,255});

            EndDrawing();
        }
    }
    CloseWindow();
    return 0;
}

