#include <lcom/lcf.h>
#include <lcom/timer.h>
#include "macros/i8254.h"
#include "macros/i8042.h"
#include "keyboard.h"
#include "mouse.h"
#include "vbe.h"
#include "assist.h"

int (menu_loop)(uint32_t irq_set_keyboard, uint32_t irq_set_mouse, uint32_t irq_set_timer);

int (init_menu)();

int (update_menu)(int sel);

int (selectedOpt)(uint16_t scancode,int sel);

int (isInOption)(int x, int y, int width, int height);

int (onPress)();

