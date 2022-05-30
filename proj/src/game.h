#include <lcom/lcf.h>
#include <lcom/timer.h>
#include "macros/i8254.h"
#include "macros/i8042.h"
#include "assist.h"
#include "snake.h"


int (game_loop)(uint32_t irq_set_keyboard, uint32_t irq_set_mouse, uint32_t irq_set_timer);

int (start_game)();

void (drawGame)();


