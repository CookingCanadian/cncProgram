#include <pigpio.h>
#include <stdio.h>
#include <stdbool.h>

// Constants
#define PULSE_PER_REVOLUTION 800.0  // Pulses per revolution 
#define LEAD 5.0                    // Lead of the screw (mm per revolution)
#define MAX_SPEED 15.0              // Maximum speed (mm/s)
#define DEBOUNCE_TIME_US 10000      // Debounce time in microseconds (10 ms)

// Motor configuration struct
typedef struct {
    int pul_pin;          // Pulse pin
    int dir_pin;          // Direction pin
    int switch_pin;       // Limit switch pin 
    double length_mm;     // Length of rail (mm)
} MotorConfig;

// Array of motor configurations
MotorConfig motors[] = {
    {18, 12, 25, 800},  // X-axis
    {13, 19, 24, 600},  // Y-axis
    {5, 6, 17, 600}     // Z-axis
};

// Global variables
volatile bool limit_switch_pressed = false;
volatile uint32_t last_trigger_time = 0;  // For debouncing

// Function prototypes
unsigned int calculatePulseDelay(double speed_mm_per_sec);
void moveMotor(MotorConfig *motor, int direction, double speed_mm_per_sec, double distance_mm);
void limitSwitchCallback(int gpio, int level, uint32_t tick);
void homeMotor(MotorConfig *motor);

int main() {
    if (gpioInitialise() < 0) {
        fprintf(stderr, "pigpio initialization failed\n");
        return 1;
    }

    // Set up motor pins
    for (int i = 0; i < sizeof(motors) / sizeof(motors[0]); i++) {
        gpioSetMode(motors[i].pul_pin, PI_OUTPUT);
        gpioSetMode(motors[i].dir_pin, PI_OUTPUT);
        if (motors[i].switch_pin != -1) {
            gpioSetMode(motors[i].switch_pin, PI_INPUT);
            gpioSetPullUpDown(motors[i].switch_pin, PI_PUD_UP);
        }
    }

    // Set up limit switch callback for Y-axis motor
    if (motors[1].switch_pin != -1) {
        gpioSetAlertFunc(motors[1].switch_pin, limitSwitchCallback);
    }

    // Home the Y-axis motor
    printf("Homing Y-axis motor...\n");
    homeMotor(&motors[1]);
    printf("Homing X-axis motor...\n");
    homeMotor(&motors[0]);

    gpioTerminate();
    return 0;
}

// Calculate pulse delay based on speed (mm/s)
unsigned int calculatePulseDelay(double speed_mm_per_sec) {
    double distance_per_pulse = LEAD / PULSE_PER_REVOLUTION;  // Distance per pulse (mm)
    double pulse_frequency = speed_mm_per_sec / distance_per_pulse;  // Pulses per second
    return (unsigned int)(1e6 / pulse_frequency);  // Delay in microseconds
}

// Move the motor
void moveMotor(MotorConfig *motor, int direction, double speed_mm_per_sec, double distance_mm) {
    unsigned int pulseDelay = calculatePulseDelay(speed_mm_per_sec);
    gpioWrite(motor->dir_pin, direction);  // Set direction

    double pulses = distance_mm / (LEAD / PULSE_PER_REVOLUTION);  // Total pulses needed
    for (int i = 0; i < pulses; i++) {
        if (limit_switch_pressed) {
            printf("Limit switch pressed. Stopping motor.\n");
            gpioDelay(100000);  // Small delay to ensure the flag is detected
            return;  // Exit the function immediately
        }

        gpioWrite(motor->pul_pin, 1);  // Send a pulse

        // Busy-wait for half the pulse delay
        uint32_t start_tick = gpioTick();
        while (gpioTick() - start_tick < pulseDelay / 2) {}

        gpioWrite(motor->pul_pin, 0);

        // Busy-wait for the other half of the pulse delay
        start_tick = gpioTick();
        while (gpioTick() - start_tick < pulseDelay / 2) {}
    }
}

// Limit switch callback function with debouncing
void limitSwitchCallback(int gpio, int level, uint32_t tick) {
    if (level == 0) {  // Limit switch pressed (assuming active-low)
        if (tick - last_trigger_time > DEBOUNCE_TIME_US) {
            limit_switch_pressed = true;
            last_trigger_time = tick;
        }
    } else if (level == 1) {  // Limit switch released
        last_trigger_time = tick;
    }
}

// Homing function for the Y-axis motor
void homeMotor(MotorConfig *motor) {
    double homing_speed_fast = 10.0;  // Fast homing speed (mm/s)
    double homing_speed_slow = 5.0;  // Slow homing speed (mm/s)
    double reverse_distance = 10.0;  // Distance to move back after hitting the limit switch (mm)

    printf("Moving towards limit switch at %.1f mm/s...\n", homing_speed_fast);
    moveMotor(motor, 1, homing_speed_fast, motor->length_mm);

    if (limit_switch_pressed) {
        printf("Limit switch pressed. Moving back %.1f mm...\n", reverse_distance);
        limit_switch_pressed = false;  // Reset the flag
        moveMotor(motor, 0, homing_speed_slow, reverse_distance);  // Move back

        printf("Moving towards limit switch at %.1f mm/s for precise homing...\n", homing_speed_slow);
        moveMotor(motor, 1, homing_speed_slow, reverse_distance);

        if (limit_switch_pressed) {
            printf("Homing complete!\n");
        } else {
            printf("Homing failed: Limit switch not pressed during slow homing.\n");
        }
    } else {
        printf("Homing failed: Limit switch not pressed during fast homing.\n");
    }
}