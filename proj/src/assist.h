#include <lcom/lcf.h>
#include "macros/i8042.h"
#include "macros/constants.h"
#include "keyboard.h"
#include "mouse.h"
#include "vbe.h"


int (subscribe_all)(uint8_t aux_timer, uint8_t aux_keyboard, uint8_t aux_mouse);

int (unsubscribe_all)();

void (drawGame)();

void (drawMouse)();

void (eraseMouse)(bool mouse);

void (updateMouse)();

void (updateGameMouse)();

int (isInOption)(int x, int y, int width, int height);

bool (checkMouseEnemy)(int enemyX, int enemyY);

bool (checkClickEnemy)(int enemyX, int enemyY);



