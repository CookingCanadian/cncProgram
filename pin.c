#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>

#define BCM2837_PERI_BASE  0x3F000000  
#define GPIO_BASE          (BCM2837_PERI_BASE + 0x200000)

// GPIO setup macros
#define BLOCK_SIZE (4 * 1024)
#define INP_GPIO(g) *(gpio + ((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio + ((g)/10)) |=  (1<<(((g)%10)*3))
#define GPIO_SET *(gpio + 7)  
#define GPIO_CLR *(gpio + 10)

volatile unsigned *gpio;

int main() {
    int mem_fd;
    void *gpio_map;

    if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
        printf("couldn't open /dev/mem :( \n");
        return -1;
    }

    // Map GPIO memory
    gpio_map = mmap(
        NULL,               
        BLOCK_SIZE,         
        PROT_READ | PROT_WRITE, 
        MAP_SHARED,          
        mem_fd,               
        GPIO_BASE            
    );

    close(mem_fd); 

    if (gpio_map == MAP_FAILED) {
        printf("mmap no work \n");
        return -1;
    }

    gpio = (volatile unsigned *)gpio_map;

    int gpio_pin = 18; // physical pin 12
    INP_GPIO(gpio_pin); 
    OUT_GPIO(gpio_pin);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int test_cycles = 1000000;
    for (int i = 0; i < test_cycles; i++) {
        GPIO_SET = 1 << gpio_pin; 
        GPIO_CLR = 1 << gpio_pin; 
    }

    clock_gettime(CLOCK_MONOTONIC, &end);


    double elapsedTime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    double frequency = test_cycles / elapsedTime;

    printf("gpio frequency: %.2f Hz\n", frequency);

    return 0;
}
