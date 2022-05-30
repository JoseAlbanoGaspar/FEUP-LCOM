#include "game.h"

extern int count;
extern int snakeCount;

extern int snakeAdd;

extern uint16_t scancode;
uint16_t vg_mode;
extern struct packet mouse_packet; // data packet of 3 bytes
extern int mouseCount; //used to say in which byte of the mouse package we are in
uint32_t backgroundColor = 0x000057FF;
extern struct Snake snake;



int (game_loop)(uint32_t irq_set_keyboard, uint32_t irq_set_mouse, uint32_t irq_set_timer)
{ 
  vg_mode = 0x0115;
  /* Wait for ESC key */
  //Here we select the bit in the hook_id needed to check if we got the right interruption
  
//--------------------------------
  int ipc_status;
  message msg;
  //1 is true
  int r;

  start_game();
  //Main loop variable
  //bool running = true;
  while (scancode != ESC_KEY) {
    // Get a request message
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:

          // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set_timer) { // subscribed timer interrupt
            timer_int_handler();
            if (count == 5) {
              count = 0;
              drawGame();
            }
            if (snakeCount == 20){
              snakeCount = 0;
              if (canMove(snake.direction)) moveSnake(snake.direction);
            }
          }

          // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set_keyboard) { // subscribed keyboard interrupt
            kbc_ih();
            changeDirection(scancode);
            kbc_reset_scancode();
          }

          // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set_mouse) { // subscribed mouse interrupt
            mouse_ih();
            mouseCount++; //received another packet
            if (mouseCount == 3){ //upon receiving the 3rd byte of a mouse packet, the program should parse it and print it on the console
                mouseCount = 0;
            
                updateMouse();
                drawMouse();
            }
          }
          break;
        default:
          break; // no other notifications expected: do nothing
      }
    }
    else { //received a standard message, not a notification
      // no standard messages expected: do nothing
    }
  }
  scancode = 0x0000;
  return 0;
}


int (start_game)(){
    vg_draw_rectangle(0, 0, 800, 600, 0x000057FF);
    startPosition();
    return 0;
}

void (drawGame)(){
    drawSnake();
    drawApple();
}


