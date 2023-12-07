#include "utility.h"

#include <chrono>
#include <thread>

unsigned int covert_gpio(unsigned int bank, unsigned int gpio_number) {
    return (gpio_number - (bank*32));
}

void delay_ms(unsigned int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}