#ifndef INC_STM32F103XX_INIT_H
#define INC_STM32F103XX_INIT_H

#include<stdint.h>


void board_init();
void tim_delay_ms(uint16_t delay);
void tim_delay_us(uint16_t delay);

void display_cpu_info();
void display_clk_src();
void display_clk_freqs();


#endif