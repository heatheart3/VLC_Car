#include "./include/servo.h"




void servo_config()
{
    ledc_timer_config_t timer_con={
        .timer_num = LEDC_TIMER_2,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .freq_hz = SERVO_FREQ,
        .speed_mode = LEDC_HIGH_SPEED_MODE
    };
    ledc_timer_config(&timer_con);
    ledc_channel_config_t  channel_con={
        .channel = LEDC_CHANNEL_3,
        .gpio_num = GPIO_OUTPUT_PWM_SERVO,
        .duty = 78,
        .intr_type = 0,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_sel = LEDC_TIMER_2
    };
    ledc_channel_config(&channel_con);
}

void turn_left45()
{
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, 52);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3);
}

void turn_right45()
{
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, 100);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3);
}
void turn_forward()
{
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, 76);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3);
}


void turn_angle(const uint8_t angle)
{
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, angle);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3);
}