#include "assist.h"


extern int hook_id_timer;
extern int hook_id_keyboard;
extern int hook_id_mouse;
extern uint16_t vg_mode;
extern int mouseX;
extern int mouseY;
extern int lastMouseX;
extern int lastMouseY;
extern struct packet mouse_packet;
extern struct Snake snake;
extern struct Apple apple;

int (subscribe_all)(uint8_t aux_timer, uint8_t aux_keyboard, uint8_t aux_mouse){
  if (kbd_subscribe_int(&aux_keyboard) != OK ) return 1;
  if ( timer_subscribe_int(&aux_timer) != OK ) return 1;
  if ( mouse_subscribe_int(&aux_mouse) != OK ) return 1;

  hook_id_keyboard = (int)aux_keyboard;
  hook_id_mouse = (int)aux_mouse;
  hook_id_timer = (int)aux_timer;

  if (mouse_en_data_report()) return 1;
  vg_init(vg_mode);
  vg_draw_rectangle(0, 0, 800, 600, 0x000057FF);
  startPosition();
  return 0;
}


int (unsubscribe_all)(){
  if (  kbd_unsubscribe_int() != OK ) return 1;
  if (timer_unsubscribe_int() != OK ) return 1;
  if (mouse_unsubscribe_int() != OK ) return 1;
  if (mouse_dis_data_report()) return 1;
  vg_exit();
  return 0;
}

void (drawGame)(){
    drawSnake();
    drawApple();
}

void (changeDirection)(uint16_t scancode){
    switch (scancode)
    {
    case RIGHT_ARROW:
        if (snake.direction == 1) break;
        snake.direction = 0;
        break;
    case LEFT_ARROW:
        if (snake.direction == 0) break;
        snake.direction = 1;
        break;
    case UP_ARROW:
        if (snake.direction == 3) break;
        snake.direction = 2;
        break;
    case DOWN_ARROW:
        if (snake.direction == 2) break;
        snake.direction = 3;
        break;
    default:
        break;
    }
}

void (drawMouse)(){
    vg_draw_rectangle(abs(lastMouseX), abs(lastMouseY), 5, 5, 0x000057FF); // erase previous mouse cursor
    vg_draw_rectangle(abs(mouseX), abs(mouseY), 5, 5, 0x0008F300); //draw new mouse cursor
}

void updateMouse(){
    lastMouseX = mouseX;
    lastMouseY = mouseY;
    int increX = (int) mouse_packet.delta_x;
    int increY = (int) mouse_packet.delta_y;
    if (mouse_packet.x_ov || mouse_packet.y_ov) {/*printf("mouse overflow\n");*/} 
    else {
        if ((mouse_packet.delta_x & 0xFF00) == 0xFF) increX = 0 - ((int) mouse_packet.delta_x);
        if ((mouse_packet.delta_y & 0xFF00) == 0xFF) increY = 0 - ((int) mouse_packet.delta_x);
        mouseX += increX;
        mouseY += increY;
        if (mouseX >= 800) mouseX = 795;
        if (mouseY <= -600) mouseY = -595;
        if (mouseX <= 0) mouseX = 0;
        if (mouseY >= 0) mouseY = 0;
    }
    
}

