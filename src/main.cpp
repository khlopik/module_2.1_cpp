#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Gpio.hpp"
#include "Led.hpp"
#include "HardwareConfig.hpp"

uint64_t startTime;

static LedBlinkMode currentLedBlinkMode = LedBlinkMode::Blink;
Led blueLed;

void timerInit()
{
    startTime = esp_timer_get_time() / usToMsDivider;
}

void updateState()
{
    const uint64_t currentTime = esp_timer_get_time() / usToMsDivider;

    if (isButtonPressed())
    {
        switch (currentLedBlinkMode)
        {
        case LedBlinkMode::Blink:
        {
            currentLedBlinkMode = LedBlinkMode::On;
        }
        break;
        case LedBlinkMode::On:
        {
            currentLedBlinkMode = LedBlinkMode::Off;
        }
        break;
        case LedBlinkMode::Off:
        {
            currentLedBlinkMode = LedBlinkMode::Blink;
            startTime = currentTime;
        }
        break;
        }

        setButtonPressed(false);
    }

    switch (currentLedBlinkMode)
    {
    case LedBlinkMode::Blink:
    {
        const uint64_t timeDiff = currentTime - startTime;
        if (timeDiff < blinkInterval)
            break;

        if (blueLed.get() == LedState::Off)
        {
            blueLed.set(LedState::On);
        }
        else
        {
            blueLed.set(LedState::Off);
        }
        startTime = currentTime;
    }
    break;
    case LedBlinkMode::On:
    {
        blueLed.set(LedState::On);
    }
    break;
    case LedBlinkMode::Off:
    {
        blueLed.set(LedState::Off);
    }
    break;
    }
}

extern "C" void app_main()
{
    gpioInit();
    timerInit();
    blueLed.init(ledPin);

    while (true)
    {
        updateState();
        vTaskDelay(pdMS_TO_TICKS(superloopYieldDelayMs));
    }
}