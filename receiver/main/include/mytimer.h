#ifndef __MY__TIMER__H__
#define __MY__TIMER__H__

#include "driver/gptimer.h"

#define DELAY_TIME 5000000 //5s
#define TRANSMIT_PERIOD 30

extern bool timer_isr_flag;


/**
 * @brief Initialize timer
 * @param gptimer: pointer to the timer handle
 */
void my_timer_init(gptimer_handle_t *gptimer);


/**
 * @brief Timer Alarm ISR function.
 * @return ESP_OK if success, ESP_FAIL if fail
 */
bool timer_on_alarm_cb();


/**
 * @brief Delay for a certain time
 * @param gptimer: pointer to the timer handle
 */
void my_delay(gptimer_handle_t *gptimer);


/**
 * @brief Delay for a certain time
 * @param us: time to delay in us
 */
void udelay(int us);



#endif