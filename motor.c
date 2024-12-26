#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include <stdlib.h>

// Pin definitions
#define PUL_PIN 12   // Pulse pin
#define DIR_PIN 33   // Direction pin

// Parameters for motor control
#define STEPS_PER_REV 800    // Motor pulses per revolution
#define RPM 200              // Desired RPM
#define STEP_DELAY 375       // Delay between pulses for 200 RPM (in microseconds)

// Function to kill any existing pigpiod processes
void kill_pigpiod() {
    printf("Killing any existing pigpiod processes...\n");
    system("sudo killall pigpiod");  // Kill all running pigpiod processes
}

// Function to initialize GPIO pins
int init_motor() {
    if (gpioInitialise() < 0) {
        fprintf(stderr, "pigpio initialization failed\n");
        return -1;  // Return -1 if initialization fails
    }
    
    // Set the direction pin as output
    gpioSetMode(DIR_PIN, PI_OUTPUT);
    // Set the pulse pin as output
    gpioSetMode(PUL_PIN, PI_OUTPUT);
    
    return 0;  // Return 0 on successful initialization
}

// Function to set the motor direction
void set_direction(int direction) {
    gpioWrite(DIR_PIN, direction); // 0 for one direction, 1 for the other
}

// Function to send pulses to the motor for continuous rotation
void pulse_motor_continuously() {
    while (1) {
        // Send a pulse to the motor: High for the pulse duration, Low for the rest
        gpioWrite(PUL_PIN, 1);  // Generate a high pulse
        usleep(STEP_DELAY);     // Wait for the pulse duration
        gpioWrite(PUL_PIN, 0);  // Generate a low pulse
        usleep(STEP_DELAY);     // Wait for the rest of the step
    }
}

int main() {
    // Kill any existing pigpiod processes to free up the port
    kill_pigpiod();
    
    // Initialize the motor control
    if (init_motor() < 0) {
        return 1;  // Exit if initialization fails
    }

    // Set the motor direction
    set_direction(0);  // 0 for one direction, 1 for the other

    // Start the motor spinning continuously at 200 RPM
    printf("Starting motor at 200 RPM...\n");
    pulse_motor_continuously();  // Continuous spinning
    
    // Clean up and terminate (this will never be reached as the motor is spinning continuously)
    gpioTerminate();
    return 0;
}
