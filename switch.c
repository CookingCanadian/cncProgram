#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

// Define the GPIO pin (Broadcom GPIO 17, physical pin 11)
#define LIMIT_SWITCH_PIN 0  // WiringPi GPIO numbering for GPIO 17

int main() {
    // Initialize WiringPi and configure the GPIO pin
    if (wiringPiSetup() == -1) {
        fprintf(stderr, "Failed to initialize WiringPi\n");
        return 1;
    }

    pinMode(LIMIT_SWITCH_PIN, INPUT);          // Set GPIO as input
    pullUpDnControl(LIMIT_SWITCH_PIN, PUD_UP); // Enable internal pull-up resistor

    printf("Monitoring limit switch on GPIO 17 (pin 11)...\n");

    while (1) {
        // Check the state of the switch
        if (digitalRead(LIMIT_SWITCH_PIN) == LOW) {  // Switch pressed
            printf("Limit switch pressed!\n");
            fflush(stdout);  // Ensure the output is sent immediately
        }
        delay(100); // Polling delay (100ms)
    }

    return 0;
}
