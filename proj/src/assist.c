#include "assist.h"
#include "sprite.h"

extern int hook_id_timer;
extern int hook_id_keyboard;
extern int hook_id_mouse;
extern int mouseX;
extern int mouseY;
extern int lastMouseX;
extern int lastMouseY;
extern struct packet mouse_packet;
extern uint16_t h_res;
extern uint16_t v_res;
uint16_t mode = 0x0000;



int (subscribe_all)(uint8_t aux_timer, uint8_t aux_keyboard, uint8_t aux_mouse){
  if (mouse_en_data_report()) return 1;
  if (timer_set_frequency(0, 60) != OK) return 1;
  if (kbd_subscribe_int(&aux_keyboard) != OK ) return 1;
  if ( timer_subscribe_int(&aux_timer) != OK ) return 1;
  if ( mouse_subscribe_int(&aux_mouse) != OK ) return 1;

  hook_id_keyboard = (int)aux_keyboard;
  hook_id_mouse = (int)aux_mouse;
  hook_id_timer = (int)aux_timer;
  
  printf("Mode: %x\n", mode);
  vg_init(mode);
  return 0;
}


int (unsubscribe_all)(){
  if (mouse_dis_data_report()) return 1;
  if (  kbd_unsubscribe_int() != OK ) return 1;
  if (timer_unsubscribe_int() != OK ) return 1;
  if (mouse_unsubscribe_int() != OK ) return 1;
  vg_exit();
  return 0;
}

void (drawMouse)(){
    uint32_t arena_color = 0x0000, mouse_cursor_color = 0x0000;
    if (mode == 0x115 || mode == 0x14C) {
        arena_color = ARENA_BACKGROUND_COLOR;
        mouse_cursor_color = MOUSE_CURSOR_COLOR;
    }
    else if (mode == 0x110) {
        arena_color = ARENA_BACKGROUND_COLOR_110;
        mouse_cursor_color = MOUSE_CURSOR_COLOR_110;
    }
    else if (mode == 0x105) {
        arena_color = ARENA_BACKGROUND_COLOR_105;
        mouse_cursor_color = MOUSE_CURSOR_COLOR_105;
    }
    else if (mode == 0x11A) {
        arena_color = ARENA_BACKGROUND_COLOR_11A;
        mouse_cursor_color = MOUSE_CURSOR_COLOR_11A;
    }
    vg_draw_rectangle(abs(lastMouseX), abs(lastMouseY), 5, 5, arena_color); // erase previous mouse cursor
    vg_draw_rectangle(abs(mouseX), abs(mouseY), 5, 5, mouse_cursor_color); //draw new mouse cursor
}

int (isInOption)(int x, int y, int width, int height){
    return abs(mouseX) >= x && abs(mouseX) <= x + width && abs(mouseY) >= y && abs(mouseY) <= y + height;
}

bool (checkClickEnemy)(int enemyX, int enemyY){
    return isInOption(enemyX, enemyY, 20, 20) && mouse_packet.lb;
}

void (updateMouse)(){
    lastMouseX = mouseX;
    lastMouseY = mouseY;
    int y_res = (int) v_res - 80, x_res = (int) h_res;
    int increX = (int) mouse_packet.delta_x;
    int increY = (int) mouse_packet.delta_y;
    if (mouse_packet.x_ov || mouse_packet.y_ov) {/*printf("mouse overflow\n");*/} 
    else {
        if ((mouse_packet.delta_x & 0xFF00) == 0xFF) increX = 0 - ((int) mouse_packet.delta_x);
        if ((mouse_packet.delta_y & 0xFF00) == 0xFF) increY = 0 - ((int) mouse_packet.delta_x);
        mouseX += increX;
        mouseY += increY;
        if (mouseX >= x_res-5) mouseX = x_res - 5;
        if (mouseY <= -(y_res-5)) mouseY = -(y_res-5);
        if (mouseX <= 0) mouseX = 0;
        if (mouseY >= 0) mouseY = 0;
    }
}


