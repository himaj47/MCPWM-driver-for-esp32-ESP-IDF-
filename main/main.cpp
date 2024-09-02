#include "esp_log.h"
#include <stdio.h>

#include "pwmDriver.hpp"

#define GPIO_PIN  GPIO_NUM_16
#define PULSE_WIDTH_MAX 19990    // max -> 19990
#define PULSE_WIDTH_MIN 0
#define TAG "PWM_TEST"

extern "C" void app_main(void)
{
    pwm_driver *pwmHandle = new pwm_driver();
    pwmHandle->set_gpio_pin(GPIO_PIN);

    pwmHandle->setRange(PULSE_WIDTH_MAX, PULSE_WIDTH_MIN);
    pwmHandle->setupTimer();
    ESP_LOGI(TAG, "timer initialization successfull");

    while (1)
    {
        pwmHandle->update(255);
    }
    
}