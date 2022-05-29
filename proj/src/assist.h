#include <lcom/lcf.h>
#include "macros/i8042.h"
#include "keyboard.h"
#include "mouse.h"
#include "vbe.h"
#include "snake.h"


int (subscribe_all)(uint8_t aux_timer, uint8_t aux_keyboard, uint8_t aux_mouse);

int (unsubscribe_all)();

int (init_game)();

int (init_menu)();

void (drawGame)();

void (changeDirection)(uint16_t scancode);

void (drawMouse)();

void (updateMouse)();

int (click_play)(int x, int y, int width, int height);


