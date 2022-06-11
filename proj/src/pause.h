#include <lcom/lcf.h>
#include "macros/i8254.h"
#include "macros/i8042.h"
#include "keyboard.h"

int pause_loop(uint32_t irq_set_keyboard,uint32_t irq_set_mouse);
