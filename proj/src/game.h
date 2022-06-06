#include <lcom/lcf.h>
#include <lcom/timer.h>
#include "macros/i8254.h"
#include "macros/i8042.h"
#include "macros/constants.h"
#include "assist.h"
#include "snake.h"
#include "pause.h"

int (game_loop)(uint32_t irq_set_keyboard, uint32_t irq_set_mouse, uint32_t irq_set_timer, uint16_t vg_mode);

int (start_game)();

void (drawGame)();


