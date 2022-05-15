// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <iMouse.h>
#include <stdint.h>
#include <stdio.h>

int(mouse_subscribe_int)(uint8_t *bit_no);

int(mouse_unsubscribe_int)();

int(mouse_get_status)();

int(mouse_check_status)();

void(parse)();

void(mouse_ih)();

int (re_enable_mouse_interrupts)();

int (mouse_reset)();

int (mouse_en_data_report)();

