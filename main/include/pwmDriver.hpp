#ifndef PWM_DRIVER
#define PWM_DRIVER

#include <driver/mcpwm_prelude.h>
#include "driver/gpio.h"

#define TIMER_RESOLUTION_HZ     1 * 1000 * 1000   // 1 MHz (tick resolution)
#define TIME_PERIOD             20000             // in clock ticks
#define MAX_PULSE_WIDTH         2500              // 2500 us
#define MIN_PULSE_WIDTH         500               // 500 us

class pwm_driver {

public:
    pwm_driver();
    void set_gpio_pin(gpio_num_t gpio_num);
    void setupTimer();
    void setRange(float max_pwm, float min_pwm);
    void update(float val);

private:
    gpio_config_t opcfg;
    gpio_num_t pin_number;

    struct unitHandler
    {
        mcpwm_timer_handle_t timerHandle;
        mcpwm_oper_handle_t operHandle;
        mcpwm_cmpr_handle_t cmpHandle;
        mcpwm_gen_handle_t genHandle;
    } unit0;

    float MAX_PWM = MAX_PULSE_WIDTH;
    float MIN_PWM = MIN_PULSE_WIDTH;

    static int map(float val);
    
};

#endif