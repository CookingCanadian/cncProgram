#include "raylib.h"
#include "stdio.h"
#include <string.h>

#define SELECTED_TEXT (Color){229,228,250,255}
#define UNSELECTED_TEXT (Color){170,171,198,255}
#define TERTIARY_TEXT (Color){64,66,109,255}

int main() {
    const int screenWidth = 1024;
    const int screenHeight = 600;  

    //CNC Values
    float spindleRPM = 0;
    float MAXspindleRPM = 20000;

    //int feedRateGlobal = 0;

    //GUI/Program Values
    const int deltaLerp = 5;
    int tabHighlightYPos = 0;
    int targetTabHighlightYPos = 0;    
    char spindleRPMText[5]; //00000
    char spindleRPMPercentageText[7]; //000%

    SetConfigFlags(FLAG_MSAA_4X_HINT);//Anti-Aliasing
    InitWindow(screenWidth, screenHeight, "cncProgram");

    //Loading Fonts
    Font regular15 = LoadFontEx("JosefinSans-Regular.ttf",15,0,0);
    Font semiBold = LoadFontEx("JosefinSans-SemiBold.ttf",30,0,0);
    Font bold15 = LoadFontEx("JosefinSans-Bold.ttf",15,0,0);
    Font bold25 = LoadFontEx("JosefinSans-Bold.ttf",25,0,0);

    //Calculating Font Positions (Unchanging; 4Y Offset) 
    int spindleTextSizeX = 125-((MeasureTextEx(bold15,"SPINDLE RPM",15,0).x)/2);
    int fileTextSizeY = 29-((MeasureTextEx(bold25,"FILE",25,0).y)/2);
    int probeTextSizeY = 79-((MeasureTextEx(bold25,"PROBE",25,0).y)/2);
    int toolTextSizeY = 129-((MeasureTextEx(bold25,"TOOL",25,0).y)/2);
    int runTextSizeY = 179-((MeasureTextEx(bold25,"RUN",25,0).y)/2);

    SetTargetFPS(200);    
    if (IsFontReady(bold25) && IsFontReady(bold15) && IsFontReady(semiBold) && IsFontReady(regular15)) {         
        while (!WindowShouldClose()) {        
            BeginDrawing();
            ClearBackground(RAYWHITE);        
            //Lower Dashboard      
            //float spindlePercentage = (float)spindleRPM/MAXspindleRPM;
            //sprintf(spindleRPMText,"%d", (int)spindleRPM);

            sprintf(spindleRPMText,"%.0f", spindleRPM);
            Vector2 spindleRPMTextSize = MeasureTextEx(semiBold,spindleRPMText,30,0);

            sprintf(spindleRPMPercentageText, "%.2f", 100*spindleRPM/MAXspindleRPM);           
            Vector2 spindleRPMPercentageTextSize = MeasureTextEx(regular15,spindleRPMPercentageText,15,0);

            
            
            DrawRectangle(0,350,1024,250,(Color){43,44,64,255}); //Background   

            DrawRectangleGradientH(217,383,200,50,(Color){31,31,44,255},(Color){43,44,64,255}); //Feed Override            
            DrawCircle(125,475,130,(Color){43,44,64,255});            
            DrawRing((Vector2){125,475},125,130,315,405,40,(Color){241,240,255,255}); 

            DrawRing((Vector2){125,475},70,100,135,405,40,(Color){241,240,255,255}); //Spindle Override
            DrawCircle(125,475,62.5,(Color){35,36,53,255});            
            DrawTextEx(bold15,"SPINDLE RPM",(Vector2){spindleTextSizeX,560},15,0,SELECTED_TEXT);  

            DrawTextEx(regular15,spindleRPMPercentageText,(Vector2){125-(spindleRPMPercentageTextSize.x/2),500},15,0,TERTIARY_TEXT);

            DrawTextEx(semiBold,spindleRPMText,(Vector2){125-(spindleRPMTextSize.x/2),475-(spindleRPMTextSize.y/2)},30,0,SELECTED_TEXT);   
           
            DrawRing((Vector2){125,475},70,100,135,(135+(270*spindleRPM/MAXspindleRPM)),40,PURPLE);
            
            //Display Area
            DrawRectangle(200,0,824,350,(Color){148,149,177,255});

            //Side Tabs
            DrawRectangle(0,0,200,350,(Color){82,83,109,255});         
            DrawRectangleGradientH(0,tabHighlightYPos,200,50,(Color){82,83,109,255},(Color){148,149,177,255}); //Highlight Icon
            switch(tabHighlightYPos) { //Color Animation
                case 0:
                    DrawTextEx(bold25,"FILE",(Vector2){20,fileTextSizeY},25,0,SELECTED_TEXT); 
                    DrawTextEx(bold25,"PROBE",(Vector2){20,probeTextSizeY},25,0,UNSELECTED_TEXT);
                    DrawTextEx(bold25,"TOOL",(Vector2){20,toolTextSizeY},25,0,UNSELECTED_TEXT);
                    DrawTextEx(bold25,"RUN",(Vector2){20,runTextSizeY},25,0,UNSELECTED_TEXT);
                    break;
                case 50:
                    DrawTextEx(bold25,"FILE",(Vector2){20,fileTextSizeY},25,0,UNSELECTED_TEXT); 
                    DrawTextEx(bold25,"PROBE",(Vector2){20,probeTextSizeY},25,0,SELECTED_TEXT);
                    DrawTextEx(bold25,"TOOL",(Vector2){20,toolTextSizeY},25,0,UNSELECTED_TEXT);
                    DrawTextEx(bold25,"RUN",(Vector2){20,runTextSizeY},25,0,UNSELECTED_TEXT);
                    break;
                case 100:
                    DrawTextEx(bold25,"FILE",(Vector2){20,fileTextSizeY},25,0,UNSELECTED_TEXT); 
                    DrawTextEx(bold25,"PROBE",(Vector2){20,probeTextSizeY},25,0,UNSELECTED_TEXT);
                    DrawTextEx(bold25,"TOOL",(Vector2){20,toolTextSizeY},25,0,SELECTED_TEXT);
                    DrawTextEx(bold25,"RUN",(Vector2){20,runTextSizeY},25,0,UNSELECTED_TEXT);
                    break;
                case 150:
                    DrawTextEx(bold25,"FILE",(Vector2){20,fileTextSizeY},25,0,UNSELECTED_TEXT); 
                    DrawTextEx(bold25,"PROBE",(Vector2){20,probeTextSizeY},25,0,UNSELECTED_TEXT);
                    DrawTextEx(bold25,"TOOL",(Vector2){20,toolTextSizeY},25,0,UNSELECTED_TEXT);
                    DrawTextEx(bold25,"RUN",(Vector2){20,runTextSizeY},25,0,SELECTED_TEXT);
                    break;
                default:
                    DrawTextEx(bold25,"FILE",(Vector2){20,fileTextSizeY},25,0,UNSELECTED_TEXT); 
                    DrawTextEx(bold25,"PROBE",(Vector2){20,probeTextSizeY},25,0,UNSELECTED_TEXT);
                    DrawTextEx(bold25,"TOOL",(Vector2){20,toolTextSizeY},25,0,UNSELECTED_TEXT);
                    DrawTextEx(bold25,"RUN",(Vector2){20,runTextSizeY},25,0,UNSELECTED_TEXT);
            }
            DrawLineEx((Vector2){0,50},(Vector2){200,50},1,(Color){58,59,84,255});     
            DrawLineEx((Vector2){0,100},(Vector2){200,100},1,(Color){58,59,84,255});          
            DrawLineEx((Vector2){0,150},(Vector2){200,150},1,(Color){58,59,84,255}); 

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
                    } else if (mousePosition.y >=150 && mousePosition.y < 200) { //Button 4
                        targetTabHighlightYPos = 150;
                    }
                }
            }

            //Touch Handler           
            

            //GUI Animation
            if (tabHighlightYPos != targetTabHighlightYPos) {
                if (tabHighlightYPos > targetTabHighlightYPos) {
                    tabHighlightYPos -= deltaLerp;
                } else if (tabHighlightYPos < targetTabHighlightYPos) {
                    tabHighlightYPos += deltaLerp;
                }
            }

            //Testing Items
            int mouseScroll = (int)GetMouseWheelMove();
            if (mouseScroll > 0) {
                if (spindleRPM < MAXspindleRPM) {
                    spindleRPM += mouseScroll*500;
                }
            } else {
                if (spindleRPM >= 1) {
                    spindleRPM += mouseScroll*500;
                }
            }              

            if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) {              
                int display = GetCurrentMonitor();    
                
                if (IsWindowFullscreen())
                {                   
                    SetWindowSize(screenWidth, screenHeight);
                }
                else
                {                  
                    SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
                }   
                ToggleFullscreen();
            }

            DrawFPS(512,300);
            EndDrawing();
        }
    }
    CloseWindow();
    return 0;
}

