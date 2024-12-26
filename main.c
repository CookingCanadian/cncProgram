#include <raylib.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include "NHG_medium.h"
#include "NHG_light.h"

#define PRUSSIAN_BLUE (Color){33, 45, 64, 255}
#define CHARCOAL (Color){54, 65, 86, 255}
#define GHOST_WHITE (Color){232, 233, 243, 255}
#define CADET_GREY (Color){147, 163, 177, 255}
#define IMPERIAL_RED (Color){247, 23, 53, 255}

// Motor thread function
void* motorControlThread(void* arg) {
    struct sched_param param;
    param.sched_priority = sched_get_priority_max(SCHED_FIFO);
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);

    while (1) {
        // Control motors here
        printf("Motor running...\n");

        // Simulate motor control timing (e.g., motor step interval)
        usleep(5000); // 5ms for example, adjust as needed
    }

    return NULL;
}

// UI thread function
typedef struct textButton {
    Rectangle rect;
    Color color;
    bool isPressed;
    const char *label;
    Vector2 textPosition;  
} textButton;

bool checkButtonInteraction(textButton *button) {
    Vector2 mousePos = GetMousePosition();
    bool isHover = CheckCollisionPointRec(mousePos, button->rect);

    if (isHover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        return true; 
    }

    return false; 
}

Font hNHG_MEDIUM;
Font bNHG_MEDIUM;
Font NHG_LIGHT;
void loadFonts() {
    hNHG_MEDIUM = LoadFontFromMemory(".ttf", font_data_medium, sizeof(font_data_medium), 24, NULL, 0);
    bNHG_MEDIUM = LoadFontFromMemory(".ttf", font_data_medium, sizeof(font_data_medium), 20, NULL, 0);
    NHG_LIGHT = LoadFontFromMemory(".ttf", font_data_light, sizeof(font_data_light), 18, NULL, 0);
}

void precomputeButtonTextPositions(textButton *buttons, int buttonCount, Font font) {
    for (int i = 0; i < buttonCount; i++) {
        Vector2 textSize = MeasureTextEx(font, buttons[i].label, font.baseSize, 1.0f);
        buttons[i].textPosition.x = buttons[i].rect.x + (buttons[i].rect.width - textSize.x) / 2;
        buttons[i].textPosition.y = buttons[i].rect.y + (buttons[i].rect.height - textSize.y) / 2;
    }
}

void DrawCenteredText(Font font, const char *text, Vector2 position, Color color) {
    DrawTextEx(font, text, position, font.baseSize, 1.0f, color);
}

void* uiThread(void* arg) {
    InitWindow(1024, 600, "cncProgram");
    loadFonts();
    SetTargetFPS(24);

    textButton topButtons[4] = { 
        {{0, 0, 256, 50}, CHARCOAL, true, "FILE"},   
        {{256, 0, 256, 50}, PRUSSIAN_BLUE, false, "ALIGN"},
        {{512, 0, 256, 50}, PRUSSIAN_BLUE, false, "ROUTINE"},
        {{768, 0, 256, 50}, PRUSSIAN_BLUE, false, "HEALTH"}
    };

    int selectedButton = 0;
    precomputeButtonTextPositions(topButtons, 4, hNHG_MEDIUM);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(CHARCOAL);

        for (int i = 0; i < 4; i++) {
            if (checkButtonInteraction(&topButtons[i])) {
                topButtons[selectedButton].isPressed = false;
                topButtons[selectedButton].color = PRUSSIAN_BLUE;
                topButtons[i].isPressed = true;
                topButtons[i].color = CHARCOAL;
                selectedButton = i;  
            }
            DrawRectangleRec(topButtons[i].rect, topButtons[i].color);
            DrawCenteredText(hNHG_MEDIUM, topButtons[i].label, topButtons[i].textPosition, (topButtons[i].isPressed) ? WHITE : CADET_GREY);
        }
        EndDrawing();
    }
    switch (selectedButton) {
        case 0: //File
            DrawRectangle()
            break;
        case 1: //Align
            break;
        case 2: //Routine
            break;
        case 3: //Health
            break;
        default:
            break;
    }

    UnloadFont(hNHG_MEDIUM);
    UnloadFont(bNHG_MEDIUM);
    UnloadFont(NHG_LIGHT);
    CloseWindow();

    return NULL;
}

int main() {
    pthread_t motorThread, uiThreadHandle;

    // Create the motor control thread
    if (pthread_create(&motorThread, NULL, motorControlThread, NULL) != 0) {
        perror("Failed to create motor control thread");
        return 1;
    }

    // Create the UI thread
    if (pthread_create(&uiThreadHandle, NULL, uiThread, NULL) != 0) {
        perror("Failed to create UI thread");
        return 1;
    }

    // Set the motor thread to real-time priority
    struct sched_param param;
    param.sched_priority = sched_get_priority_max(SCHED_FIFO);
    pthread_setschedparam(motorThread, SCHED_FIFO, &param);

    // Wait for threads to complete (UI thread will terminate on window close)
    pthread_join(uiThreadHandle, NULL);

    // Note: Motor thread is likely infinite, terminate manually if needed
    pthread_cancel(motorThread);
    pthread_join(motorThread, NULL);

    return 0;
}
