#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <lcom/lcf.h>

#include <stdint.h>

#include "i8042.h"

int (kbd_subscribe_int)(uint8_t *bit_no);

int (kbd_unsubscribe_int)();

int (kbd_read_buffer)(uint16_t * data);

int (kbd_read_status)(uint8_t * status);

int (isValidStatus)();

void (kbc_ih)(void);

#endif /* _LCOM_KEYBOARD_H */
