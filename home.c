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
    {18, 12, 4, 800},  // X-axis
    {13, 19, 24, 600}, // Y-axis
    {5, 6, 17, 600}    // Z-axis
};

// Function prototypes
unsigned int calculatePulseDelay(double speed_mm_per_sec);
void moveMotor(MotorConfig *motor, int direction, double speed_mm_per_sec, double distance_mm, volatile bool *limit_switch_pressed);
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

    // Home the Z-axis motor
    printf("Homing Z-axis motor...\n");
    homeMotor(&motors[2]);

    gpioTerminate();
    return 0;
}

// Calculate pulse delay based on speed (mm/s)
unsigned int calculatePulseDelay(double speed_mm_per_sec) {
    double distance_per_pulse = LEAD / PULSE_PER_REVOLUTION;
    double pulse_frequency = speed_mm_per_sec / distance_per_pulse;
    return (unsigned int)(1e6 / pulse_frequency);
}

// Move the motor
void moveMotor(MotorConfig *motor, int direction, double speed_mm_per_sec, double distance_mm, volatile bool *limit_switch_pressed) {
    unsigned int pulseDelay = calculatePulseDelay(speed_mm_per_sec);
    gpioWrite(motor->dir_pin, direction);

    double pulses = distance_mm / (LEAD / PULSE_PER_REVOLUTION);
    for (int i = 0; i < pulses; i++) {
        if (*limit_switch_pressed) {
            printf("Limit switch pressed. Stopping motor.\n");
            return;
        }

        gpioWrite(motor->pul_pin, 1);
        gpioDelay(pulseDelay / 2);
        gpioWrite(motor->pul_pin, 0);
        gpioDelay(pulseDelay / 2);
    }
}

// Homing function with fast/slow homing
void homeMotor(MotorConfig *motor) {
    double homing_speed_fast = 10.0;  // Fast homing speed (mm/s)
    double homing_speed_slow = 5.0;   // Slow homing speed (mm/s)
    double reverse_distance = 10.0;   // Distance to reverse after hitting the limit switch (mm)
    volatile bool motor_limit_switch_pressed = false; // Local state for the motor
    uint32_t motor_last_trigger_time = 0;            // Local debounce tracking

    // Local callback for limit switch events
    void motorLimitSwitchCallback(int gpio, int level, uint32_t tick) {
        if (gpio == motor->switch_pin && level == 0) { // Trigger on switch press
            if (tick - motor_last_trigger_time > DEBOUNCE_TIME_US) {
                motor_limit_switch_pressed = true;
                motor_last_trigger_time = tick;
            }
        }
    }

    // Enable limit switch listening for this motor
    if (gpioSetAlertFunc(motor->switch_pin, motorLimitSwitchCallback) != 0) {
        fprintf(stderr, "Failed to set alert function for pin %d\n", motor->switch_pin);
        return;
    }

    // Perform fast homing
    printf("Performing fast homing...\n");
    moveMotor(motor, 1, homing_speed_fast, motor->length_mm, &motor_limit_switch_pressed);

    if (motor_limit_switch_pressed) {
        printf("Limit switch pressed during fast homing. Reversing...\n");
        motor_limit_switch_pressed = false; // Reset for the slow homing pass

        // Perform slow homing
        printf("Performing slow homing...\n");
        moveMotor(motor, 0, homing_speed_slow, reverse_distance, &motor_limit_switch_pressed);
        moveMotor(motor, 1, homing_speed_slow, reverse_distance, &motor_limit_switch_pressed);

        if (motor_limit_switch_pressed) {
            printf("Homing complete for motor on pin %d!\n", motor->pul_pin);
        } else {
            printf("Homing failed during slow homing.\n");
        }
    } else {
        printf("Homing failed during fast homing.\n");
    }

    // Disable limit switch listening for this motor
    if (gpioSetAlertFunc(motor->switch_pin, NULL) != 0) {
        fprintf(stderr, "Failed to stop alert function for pin %d\n", motor->switch_pin);
    }
}