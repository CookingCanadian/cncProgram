#include "raylib.h"
#include "stdio.h"

int main() {
    const int screenWidth = 1024;
    const int screenHeight = 600;  

    //CNC Values
    int spindleRPM = 0;

    //GUI/Program Values
    const int deltaLerp = 5;
    int tabHighlightYPos = 0;
    int targetTabHighlightYPos = 0;    

    SetConfigFlags(FLAG_MSAA_4X_HINT);//Anti-Aliasing
    InitWindow(screenWidth, screenHeight, "cncProgram");

    //Loading Fonts
    Font regular = LoadFontEx("JosefinSans-Regular.ttf",30,0,0);
    Font semiBold = LoadFontEx("JosefinSans-SemiBold.ttf",30,0,0);
    Font bold15 = LoadFontEx("JosefinSans-Bold.ttf",15,0,0);
    Font bold25 = LoadFontEx("JosefinSans-Bold.ttf",25,0,0);

    //Calculating Font Positions (Unchanging; 4Y Offset) 
    int spindleTextSizeX = 125-((MeasureTextEx(bold15,"SPINDLE RPM",15,0).x)/2);
    int setupTextSizeY = 29-((MeasureTextEx(bold25,"SETUP",25,0).y)/2);
    int toolsTextSizeY = 79-((MeasureTextEx(bold25,"TOOLS",25,0).y)/2);
    
    SetTargetFPS(200);    
    if (IsFontReady(bold25) && IsFontReady(bold15) && IsFontReady(semiBold) && IsFontReady(regular)) {         
        while (!WindowShouldClose()) {        
            BeginDrawing();
            ClearBackground(RAYWHITE);        
            //Lower Dashboard 
            DrawRectangle(0,350,1024,250,(Color){43,44,64,255});//Spindle Info
            DrawCircleSector((Vector2){125,475},100,140,400,40,(Color){217,217,217,255});
            DrawCircle(125,475,75,(Color){43,44,64,255});
            DrawCircle(125,475,62.5,(Color){35,36,53,255});
            char spindleRPMText[5];
            sprintf(spindleRPMText,"%'d",spindleRPM);
            Vector2 spindleRPMTextSize = MeasureTextEx(semiBold,spindleRPMText,30,0);
            DrawTextEx(bold15,"SPINDLE RPM",(Vector2){spindleTextSizeX,550},15,0,(Color){229,228,250,255});     
            DrawTextEx(semiBold,spindleRPMText,(Vector2){125-(spindleRPMTextSize.x/2),475-(spindleRPMTextSize.y/2)},30,0,(Color){229,228,250,255});        
            
            //Display Area
            DrawRectangle(200,0,824,350,(Color){148,149,177,255});

            //Side Tabs
            DrawRectangle(0,0,200,350,(Color){82,83,109,255});
            DrawRectangle(0,tabHighlightYPos,200,50,(Color){148,149,177,255}); //Highlight Icon

            DrawTextEx(bold25,"SETUP",(Vector2){50,setupTextSizeY},25,0,(Color){229,228,250,255}); 
            DrawTextEx(bold25,"TOOLS",(Vector2){50,toolsTextSizeY},25,0,(Color){229,228,250,255});

            //Mouse Handler
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePosition = GetMousePosition();
                if (mousePosition.x <= 200 && mousePosition.y < 350) { //Side Tab Area
                    if (mousePosition.y >=0 && mousePosition.y < 50) { //Button 1
                        targetTabHighlightYPos = 0;
                    } else if (mousePosition.y >= 50 && mousePosition.y < 100) { //Button 2
                        targetTabHighlightYPos = 50;
                    } else if (mousePosition.y >=100 && mousePosition.y < 150) { //Button 3
                        targetTabHighlightYPos = 100;
                    }
                }
            }

            //GUI Animation
            if (tabHighlightYPos != targetTabHighlightYPos) {
                if (tabHighlightYPos > targetTabHighlightYPos) {
                    tabHighlightYPos -= deltaLerp;
                } else if (tabHighlightYPos < targetTabHighlightYPos) {
                    tabHighlightYPos += deltaLerp;
                }
            }

            //Testing Items
            DrawFPS(512,300);
            EndDrawing();
        }
    }
    CloseWindow();
    return 0;
}

