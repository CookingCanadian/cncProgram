#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <ncurses.h> // For keyboard input

// Define GPIO pins (WiringPi numbering)
#define LIMIT_SWITCH_PIN 0 // GPIO 17, physical pin 11
#define PUL_PIN 1          // GPIO 18, physical pin 12
#define DIR_PIN 2          // GPIO 27, physical pin 13

// Define motor parameters
#define STEP_DELAY 1000 // Base delay between steps in microseconds (adjustable with RPM)

// Function to perform a single step
void stepMotor(int steps, int direction, int rpm) {
    // Set direction
    digitalWrite(DIR_PIN, direction);

    // Calculate step delay based on RPM
    int stepDelay = 60000000 / (steps * rpm); // Microseconds per step

    for (int i = 0; i < steps; i++) {
        digitalWrite(PUL_PIN, HIGH);
        delayMicroseconds(stepDelay / 2);
        digitalWrite(PUL_PIN, LOW);
        delayMicroseconds(stepDelay / 2);
    }
}

int main() {
    // Initialize WiringPi and configure GPIO pins
    if (wiringPiSetup() == -1) {
        fprintf(stderr, "Failed to initialize WiringPi\n");
        return 1;
    }

    pinMode(LIMIT_SWITCH_PIN, INPUT);          // Set GPIO as input
    pullUpDnControl(LIMIT_SWITCH_PIN, PUD_UP); // Enable internal pull-up resistor
    pinMode(PUL_PIN, OUTPUT);                  // Set PUL_PIN as output
    pinMode(DIR_PIN, OUTPUT);                  // Set DIR_PIN as output

    // Initialize ncurses for keyboard input
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); // Non-blocking input

    // Motor control parameters
    int rpm = 120;  // Default RPM
    int stepsPerRevolution = 200; // Adjust based on your motor's specs

    printf("Monitoring limit switch and controlling motor...\n");
    printf("Press UP to move forward, DOWN to move backward, Q to quit.\n");

    int ch;
    while (1) {
        // Check limit switch state
        if (digitalRead(LIMIT_SWITCH_PIN) == LOW) { // Switch pressed
            printf("Limit switch pressed! Stopping motor.\n");
            continue;
        }

        // Check keyboard input
        ch = getch();
        if (ch == 'q' || ch == 'Q') { // Quit program
            break;
        } else if (ch == KEY_UP) { // Move forward
            stepMotor(stepsPerRevolution, HIGH, rpm);
            printf("Motor moving forward at %d RPM.\n", rpm);
        } else if (ch == KEY_DOWN) { // Move backward
            stepMotor(stepsPerRevolution, LOW, rpm);
            printf("Motor moving backward at %d RPM.\n", rpm);
        }

        delay(100); // Polling delay
    }

    // Cleanup ncurses
    endwin();

    return 0;
}
