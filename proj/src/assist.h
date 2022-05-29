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

int (update_menu)(int sel);

void (drawGame)();

void (changeDirection)(uint16_t scancode);

int (selectedOpt)(uint16_t scancode,int sel);

void (drawMouse)();

void (updateMouse)();

int (isInOption)(int x, int y, int width, int height);

int (onPress)();


