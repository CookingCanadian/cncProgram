#include <pigpio.h>
#include <stdio.h>
#include <stdbool.h>

// Motor configuration struct
typedef struct {
    int pul_pin;          // Pulse pin
    int dir_pin;          // Direction pin
    int switch_pin;       // Limit switch pin
    double length_mm;     // Length of rail (mm)
} MotorConfig;

// Array of motor configurations
MotorConfig motors[] = {
    {18, 12, 4, 800},  // X-axis
    {13, 19, 24, 600},  // Y-axis
    {5, 6, 17, 600}     // Z-axis
};

// Callback function to handle switch events
void limitSwitchCallback(int gpio, int level, uint32_t tick) {
    if (level == 0) { // Trigger only when the switch is pressed (closed)
        printf("Limit switch on GPIO %d triggered. Level: %d\n", gpio, level);
    }
}

int main() {
    if (gpioInitialise() < 0) {
        fprintf(stderr, "pigpio initialization failed\n");
        return 1;
    }

    // Set up motor pins
    for (int i = 0; i < sizeof(motors) / sizeof(motors[0]); i++) {
        if (motors[i].switch_pin != -1) {
            gpioSetMode(motors[i].switch_pin, PI_INPUT);
            gpioSetPullUpDown(motors[i].switch_pin, PI_PUD_UP);
            gpioGlitchFilter(motors[i].switch_pin, 10000); // Debounce filter
            gpioSetAlertFunc(motors[i].switch_pin, limitSwitchCallback);
        }
    }

    printf("Listening for limit switch events. Press Ctrl+C to exit.\n");

    // Keep the program running
    while (1) {
        gpioDelay(1000000); // 1 second delay to prevent CPU overuse
    }

    gpioTerminate();
    return 0;
}
