#ifndef INC_STM32F103XX_INIT_H
#define INC_STM32F103XX_INIT_H

#include<stdint.h>


void board_init();
void led_init();
void led_off();
void led_on();
void led_toggle();
void led_flash(uint8_t freq, uint16_t interval_ms);
void clock_init();
void tim2_init();
void tim_delay_us(uint16_t delay);
void tim_delay_ms(uint16_t delay);

void display_cpu_info();
void display_clk_info();


#endif