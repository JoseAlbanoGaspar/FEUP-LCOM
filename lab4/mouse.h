// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include <iMouse.h>

int hook_id = MOUSE_IRQ;

void (mouse_ih)(void);

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

int (mouse_reset)();

