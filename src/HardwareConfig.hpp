#ifndef HARDWARECONFIG_HPP
#define HARDWARECONFIG_HPP

#include "driver/gpio.h"

constexpr gpio_num_t ledPin = GPIO_NUM_15;
constexpr gpio_num_t buttonPin = GPIO_NUM_17;
constexpr uint32_t debounceDelay = 200;
constexpr uint64_t usToMsDivider = 1000;
constexpr uint32_t blinkInterval = 500;
constexpr uint32_t superloopYieldDelayMs = 1;

#endif