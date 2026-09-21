#include "HardwareConfig.hpp"
#include "driver/gpio.h"
#include "esp_attr.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static volatile bool s_isButtonPressed = false;
static volatile uint32_t lastButtonPressTimeMs = 0;

bool isButtonPressed() {
    return s_isButtonPressed;
};

void setButtonPressed(bool value) {
    s_isButtonPressed = value;
};

void IRAM_ATTR gpioIsrOnButtonPress(void *arg)
{
    const uint32_t currentTime = pdTICKS_TO_MS(xTaskGetTickCountFromISR());

    if ((currentTime - lastButtonPressTimeMs) < debounceDelay)
        return;

    s_isButtonPressed = true;
    lastButtonPressTimeMs = currentTime;
}

void gpioInit()
{
    const gpio_config_t ledConfig = {
        .pin_bit_mask = 1ULL << ledPin,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE};

    gpio_config(&ledConfig);

    const gpio_config_t buttonConfig = {
        .pin_bit_mask = 1ULL << buttonPin,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE,
    };

    gpio_config(&buttonConfig);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(buttonPin, gpioIsrOnButtonPress, nullptr);
}