#include "./include/encoder.h"
#include "./driver/gpio.h"
#include "./include/motor.h"
#include "esp_timer.h"

extern int motor_speed;
int data[200];

int encoder_edge_cnt = 0;
int encoder_edge_cnt2 = 0;
esp_timer_handle_t timer1;

int adjust_wheel_speed()
{
    static float pwm_new = 0, bias = 0 , last_bias = 0;
    bias = motor_speed - encoder_edge_cnt;
    encoder_edge_cnt = 0;
    pwm_new += KI*bias + KP*(bias - last_bias);
    if(pwm_new  >= (1 << DUTY_RESOLUTION) -1)
    {
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0,(1 << DUTY_RESOLUTION) -1);
    }
    else
    {
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, pwm_new);
    }
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
    return  pwm_new;
}

void go_straight()
{   //右轮随便跑，调整左轮和右轮一致
    static float pwm_other = 0, bias2 = 0 , last_bias2 = 0;
    bias2 = encoder_edge_cnt2 - encoder_edge_cnt; 
    encoder_edge_cnt2 = encoder_edge_cnt = 0;
    pwm_other += KI*bias2 + KP*(bias2 - last_bias2);
    if(pwm_other  >= (1 << DUTY_RESOLUTION) -1)
    {
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1,(1 << DUTY_RESOLUTION) -1);
    }
    else
    {
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, pwm_other);
    }
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1);
}

void read_encoder()
{
    printf("%d\n", encoder_edge_cnt);
    encoder_edge_cnt = 0;
}

void encoder_timer_config()
{
    esp_timer_init();
    esp_timer_create_args_t timer_args={
        .arg= NULL, 
        .callback = go_straight,
        .dispatch_method =  ESP_TIMER_TASK,
        .name = "Encoder_Timer1",
        .skip_unhandled_events = false
    };
    esp_timer_create(&timer_args, &timer1);
    esp_timer_start_periodic(timer1, UNIT_TIME);
    
} 

static void IRAM_ATTR encoder_cnt_isr_handler (void *arg){
    encoder_edge_cnt ++;
}
static void IRAM_ATTR encoder_cnt_isr_handler_2 (void *arg){
    encoder_edge_cnt2 ++;
}


void encoder_config()
{
    gpio_config_t encoder_con =
    {
        .intr_type = GPIO_INTR_POSEDGE,
        .pin_bit_mask = (1ULL << GPIO_ENCODER_OUTPUT_1),
        .mode = GPIO_MODE_INPUT
    };
    gpio_config(&encoder_con);
    encoder_con.pin_bit_mask = ( 1ULL << GPIO_INPUT_ENCODER_1);
    gpio_config(&encoder_con);
    gpio_install_isr_service(0);
    
    gpio_isr_handler_add(GPIO_ENCODER_OUTPUT_1, encoder_cnt_isr_handler, NULL);   
    gpio_isr_handler_add(GPIO_INPUT_ENCODER_1, encoder_cnt_isr_handler_2, NULL);   
}