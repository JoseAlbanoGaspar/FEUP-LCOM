#include "assist.h"


extern int hook_id_timer;
extern int hook_id_keyboard;
extern int hook_id_mouse;
extern uint16_t vg_mode;
extern int direction;

int (subscribe_all)(uint8_t aux_timer, uint8_t aux_keyboard, uint8_t aux_mouse){
  if (kbd_subscribe_int(&aux_keyboard) != OK ) return 1;
  if ( timer_subscribe_int(&aux_timer) != OK ) return 1;
  if ( mouse_subscribe_int(&aux_mouse) != OK ) return 1;

  hook_id_keyboard = (int)aux_keyboard;
  hook_id_mouse = (int)aux_mouse;
  hook_id_timer = (int)aux_timer;

  vg_init(vg_mode);
  vg_draw_rectangle(0, 0, 800, 600, 0x000057FF);
  startPosition();
  return 0;
}


int (unsubscribe_all)(){
  if (kbd_unsubscribe_int() != OK ) return 1;
  if ( timer_unsubscribe_int() != OK ) return 1;
  if ( mouse_unsubscribe_int() != OK ) return 1;

  vg_exit();
  return 0;
}

void (drawGame)(){
    drawSnake();
}

void (changeDirection)(uint16_t scancode){
    switch (scancode)
    {
    case RIGHT_ARROW:
        if (direction == 1) break;
        direction = 0;
        break;
    case LEFT_ARROW:
        if (direction == 0) break;
        direction = 1;
        break;
    case UP_ARROW:
        if (direction == 3) break;
        direction = 2;
        break;
    case DOWN_ARROW:
        if (direction == 2) break;
        direction = 3;
        break;
    default:
        break;
    }
}

