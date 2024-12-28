#include <pigpio.h>
#include <stdio.h>

#define PulPerRev 800
#define Lead 5 // SFU1605

#define Y_PUL 18  // GPIO 18 (Physical pin 12 for PUL+)
#define Y_DIR 13  // GPIO 13 (Physical pin 33 for DIR+)

void move(int pin, int direction, int distance, int velocity) { // 0 -> left | 1 -> right
    if (pin == 0) { // 0 -> x | 1 -> y | 2 -> z
        printf("move x");
    } else if (pin == 1) {
        gpioWrite(Y_DIR, direction);
        int PPmm = PulPerRev / Lead;
        int totalPulses = distance * PPmm;
        int PPR = (velocity * PulPerRev) / 60;
        int delay = 1000000 / PulPerRev;

        for (int i = 0; i < totalPulses; i++) {
            gpioWrite(Y_PUL, 1);
            gpioDelay(delay);
            gpioWrite(Y_PUL, 0);
            gpioDelay(delay);
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

    move(1, 1, 5, 10);

    gpioTerminate();
    return 0;
}