#include "pwmDriver.hpp"

pwm_driver* pwm_drv = 0;

mcpwm_timer_config_t timer_cfg = {
    .group_id = 0,
    .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
    .resolution_hz = TIMER_RESOLUTION_HZ,
    .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    .period_ticks = TIME_PERIOD,
};

mcpwm_operator_config_t operator_config = {
    .group_id = 0,
    .flags = {}
};

mcpwm_comparator_config_t comparator_config = {
    .flags = {1,0,0}
};

pwm_driver::pwm_driver() {
    pwm_drv = this;

    // gpio configurations
    opcfg.pin_bit_mask = (uint64_t)(0);
    opcfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    opcfg.pull_up_en = GPIO_PULLUP_DISABLE;
    opcfg.mode = GPIO_MODE_OUTPUT;
    opcfg.intr_type = GPIO_INTR_DISABLE;
}

void pwm_driver::set_gpio_pin(gpio_num_t gpio_num) {
    opcfg.pin_bit_mask |= (uint64_t)(1 << gpio_num);
    gpio_config(&opcfg);
    pin_number = gpio_num;
}

void pwm_driver::setupTimer() {
    mcpwm_new_timer(&timer_cfg, &unit0.timerHandle);
    mcpwm_new_operator(&operator_config, &unit0.operHandle);
    mcpwm_operator_connect_timer(unit0.operHandle, unit0.timerHandle);

    mcpwm_new_comparator(unit0.operHandle, &comparator_config, &unit0.cmpHandle);

    mcpwm_generator_config_t generator_config = {
        .gen_gpio_num = pin_number,
        .flags = {}
    };

    mcpwm_new_generator(unit0.operHandle, &generator_config, &unit0.genHandle);

    mcpwm_gen_timer_event_action_t tim_ev_ac;
    tim_ev_ac.direction = MCPWM_TIMER_DIRECTION_UP;
    tim_ev_ac.event = MCPWM_TIMER_EVENT_EMPTY;
    tim_ev_ac.action = MCPWM_GEN_ACTION_HIGH;

    mcpwm_generator_set_actions_on_timer_event(unit0.genHandle, tim_ev_ac);

    mcpwm_gen_compare_event_action_t ev_act;
    ev_act.direction = MCPWM_TIMER_DIRECTION_UP;
    ev_act.action = MCPWM_GEN_ACTION_LOW;
    ev_act.comparator = unit0.cmpHandle;

    mcpwm_generator_set_action_on_compare_event(unit0.genHandle, ev_act);

    mcpwm_timer_enable(unit0.timerHandle);
    mcpwm_timer_start_stop(unit0.timerHandle, MCPWM_TIMER_START_NO_STOP);
}


void pwm_driver::setRange(float max_pwm, float min_pwm) {
    MAX_PWM = max_pwm;
    MIN_PWM = min_pwm;
}

// analogWrite (0-255)
void pwm_driver::update(float val) {
    mcpwm_comparator_set_compare_value(unit0.cmpHandle, map(val));
}

// map(0, 255, max_pulse_width, min_pulse_width) : arduino map function
int pwm_driver::map(float val) {
    return ((val*(pwm_drv->MAX_PWM - pwm_drv->MIN_PWM))/255.0f) + pwm_drv->MIN_PWM;
}