#include "./include/motor.h"
#include "./include/servo.h"

void motor_config()
{
    ledc_timer_config_t ledc_timer = { 
	.duty_resolution = DUTY_RESOLUTION,	// PWM占空比分辨率
	.freq_hz = 5000,						// PWM信号频率
	.speed_mode = LEDC_HIGH_SPEED_MODE,				// 定时器模式
	.timer_num = LEDC_TIMER_0				// 定时器序号;使用哪个定时器0-3;
    };
    ledc_timer_config(&ledc_timer);
    //configure PWMA
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_sel = LEDC_TIMER_0,
        .gpio_num = GPIO_OUTPUT_PWMA,
        .channel = LEDC_CHANNEL_0,
        .duty = 3095,
        .intr_type = LEDC_INTR_DISABLE,
        .hpoint = 0
    };
    ledc_channel_config(&ledc_channel);
    //configure PWMB
    ledc_channel.gpio_num = GPIO_OUTPUT_PWMB;
    ledc_channel_config(&ledc_channel);
    //configure AIN1,AIN2,BIN1,BIN2
    gpio_config_t io_config;
    io_config.mode = GPIO_MODE_OUTPUT;
    io_config.pin_bit_mask = ( (1 << GPIO_OUTPUT_AIN1) | (1 << GPIO_OUTPUT_AIN2) | (1 << GPIO_OUTPUT_BIN1) 
                                | (1 << GPIO_OUTPUT_BIN2));
    gpio_config(&io_config);
}

int perDuty2Duty(int perDuty)
{
    int duty = 0;
    int total_duty = (1 << DUTY_RESOLUTION) - 1;
    duty = perDuty * total_duty * 0.01;
    return duty; 
}

void forward(int per)
{
    turn_forward();
    stop();
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, perDuty2Duty(per));
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);

    gpio_set_level(GPIO_OUTPUT_AIN1, 0);
    gpio_set_level(GPIO_OUTPUT_AIN2, 1);
    gpio_set_level(GPIO_OUTPUT_BIN1, 1);
    gpio_set_level(GPIO_OUTPUT_BIN2, 0);
}

void backward(int per)
{
    turn_forward();
    stop();
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, perDuty2Duty(per));
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);

    gpio_set_level(GPIO_OUTPUT_AIN1, 1);
    gpio_set_level(GPIO_OUTPUT_AIN2, 0);
    gpio_set_level(GPIO_OUTPUT_BIN1, 0);
    gpio_set_level(GPIO_OUTPUT_BIN2, 1);

}

void stop()
{
    gpio_set_level(GPIO_OUTPUT_AIN1, 0);
    gpio_set_level(GPIO_OUTPUT_AIN2, 0);
    gpio_set_level(GPIO_OUTPUT_BIN1, 0);
    gpio_set_level(GPIO_OUTPUT_BIN2, 0);
}

void left_forward(int per)
{

    //turn_left45();
    turn_angle(SERVO_MID_DUTY-10);
    stop();
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, perDuty2Duty(per));
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);

    gpio_set_level(GPIO_OUTPUT_AIN1, 0);
    gpio_set_level(GPIO_OUTPUT_AIN2, 1);
    gpio_set_level(GPIO_OUTPUT_BIN1, 1);
    gpio_set_level(GPIO_OUTPUT_BIN2, 0);
    
}

void right_forward(int per)
{
    turn_angle(SERVO_MID_DUTY+10);
    stop();
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, perDuty2Duty(per));
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);

    gpio_set_level(GPIO_OUTPUT_AIN1, 0);
    gpio_set_level(GPIO_OUTPUT_AIN2, 1);
    gpio_set_level(GPIO_OUTPUT_BIN1, 1);
    gpio_set_level(GPIO_OUTPUT_BIN2, 0);
}
void car_control(const char* cmd)
{
    if(strcmp(cmd,STOP)==0)
    {
        printf("stop\n");
        stop();
    }
    else if(strcmp(cmd,FORWARD)==0)
    {
        printf("forward\n");
        forward(20);
    }
    else if(strcmp(cmd,BACKWARD)==0)
    {
        printf("backward\n");
        backward(20);
    }
    else if(strcmp(cmd,LEFT)==0)
    {
        printf("left\n");
        left_forward(20);
    }
    else if(strcmp(cmd,RIGHT)==0)
    {
        printf("right\n");
        right_forward(20);
    }
}