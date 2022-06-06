#include <lcom/lcf.h>
#include "macros/i8042.h"
#include "macros/constants.h"
#include "keyboard.h"
#include "mouse.h"
#include "vbe.h"

/**
 * subscribes all the devices
 * @param aux_timer
 * @param aux_keyboard
 * @param aux_mouse
 * @return
 */
int (subscribe_all)(uint8_t aux_timer, uint8_t aux_keyboard, uint8_t aux_mouse);

/**
 * unsubscribes all the devices
 * @return
 */
int (unsubscribe_all)();

void (drawGame)(); //nao está implementada

/**
 * fills the variables according to the specified mode
 */
void (drawMouse)();

/**
 * clears the variables form that were initialized lately
 * @param mouse
 */
void (eraseMouse)(bool mouse);

/**
 * updates mouse state
 */
void (updateMouse)();

/**
 * updates game mouse state
 */
void (updateGameMouse)();

/**
 * auxiliar function to see if the mouse is over an enemy
 * @param x x_pos of the enemy
 * @param y y_pos of the enemy
 * @param width
 * @param height
 * @return
 */
int (isInOption)(int x, int y, int width, int height);

/**
 * checks if mouse is over an enemy
 * @param enemyX x position of the enemy
 * @param enemyY y position of the enemy
 * @return true if it's over him
 */
bool (checkMouseEnemy)(int enemyX, int enemyY);

/**
 * checks if had clicked in an enemy
 * @param enemyX x position of the enemy
 * @param enemyY y position of the enemy
 * @return true if it had been clicked
 */
bool (checkClickEnemy)(int enemyX, int enemyY);



