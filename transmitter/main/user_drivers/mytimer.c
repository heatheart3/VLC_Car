#include "./include/mytimer.h"

bool  timer_on_alarm_cb()
{
    timer_isr_flag = true;
    return true;
}



void my_timer_init(gptimer_handle_t *out_gptimer)
{
    gptimer_handle_t gptimer = NULL;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));

    gptimer_alarm_config_t alarm_config = {
        .alarm_count = DELAY_TIME, // 1s @resolution is 1MHz
    };

    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));

    gptimer_event_callbacks_t cbs = {
        .on_alarm = timer_on_alarm_cb,
    };

    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));

    *out_gptimer = gptimer;

    ESP_ERROR_CHECK(gptimer_enable(*out_gptimer));
}

void my_delay(gptimer_handle_t *gptimer)
{
    gptimer_set_raw_count(*gptimer,0);
    ESP_ERROR_CHECK(gptimer_start(*gptimer));

    while(!timer_isr_flag)
    {};
    ESP_ERROR_CHECK(gptimer_stop(*gptimer));
    timer_isr_flag=false;
}

static __inline void delay_clock(int ts)
{
uint32_t start, curr;

__asm__ __volatile__("rsr %0, ccount" : "=r"(start));
do
__asm__ __volatile__("rsr %0, ccount" : "=r"(curr));
while (curr - start <= ts);
}

void udelay(int us)
{
    while(us--)
    {
        delay_clock(160);
    }
}