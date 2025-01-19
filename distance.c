#include <pigpio.h>
#include <stdio.h>

#define PulPerRev 800
#define Lead 5 // SFU1605

#define Y_PUL 5  // GPIO 18 (Physical pin 12 for PUL+)
#define Y_DIR 6  // GPIO 13 (Physical pin 33 for DIR+)

void move(int pin, int direction, int distance, int velocity) { // 0 -> left | 1 -> right
    if (pin == 0) { // 0 -> x | 1 -> y | 2 -> z
        printf("move x");
    } else if (pin == 1) {
        gpioWrite(Y_DIR, direction);
        int PPmm = PulPerRev / Lead;
        int totalPulses = distance * PPmm;
        int delay = (60 * 1000000) / (PulPerRev * velocity); // Calculate delay based on velocity

        for (int i = 0; i < totalPulses; i++) {
            gpioWrite(Y_PUL, 1);
            gpioDelay(delay / 2); // Half the delay for the high pulse
            gpioWrite(Y_PUL, 0);
            gpioDelay(delay / 2); // Half the delay for the low pulse
        }
    }
}

int main() {
    if (gpioInitialise() < 0) {
        printf("didn't init pigpio\n");
        return 1;
    }
    gpioSetMode(Y_PUL, PI_OUTPUT);
    gpioSetMode(Y_DIR, PI_OUTPUT);

    move(1, 0, 200, 200); // Move 200mm at 5 mm/s

    gpioTerminate();
    return 0;
}