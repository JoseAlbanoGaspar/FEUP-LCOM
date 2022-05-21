// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <lcom/timer.h>
#include "macros/i8254.h"
#include "macros/i8042.h"
#include "keyboard.h"
#include "mouse.h"
#include "vbe.h"
#include "assist.h"


extern int count;
extern int snakeCount;

extern int snakeAdd;

extern uint16_t scancode;
extern int hook_id_timer;
extern int hook_id_keyboard;
extern int hook_id_mouse;
uint16_t vg_mode;
extern struct packet mouse_packet; // data packet of 3 bytes
extern int mouseCount; //used to say in which byte of the mouse package we are in
uint32_t backgroundColor = 0x000057FF;
extern struct Snake snake;


// Any header files included below this line should have been created by you

int main(int argc, char *argv[])
{
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/src/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (proj_main_loop)(int argc, char* argv[])
{ 
  vg_mode = 0x0115;
  /* Wait for ESC key */
  //Here we select the bit in the hook_id needed to check if we got the right interruption
  
  //keyboard
  uint32_t irq_set_keyboard = BIT(hook_id_keyboard);
  uint8_t aux_keyboard = (uint8_t)hook_id_keyboard;
  //mouse
  uint32_t irq_set_mouse = BIT(hook_id_mouse);
  uint8_t aux_mouse = (uint8_t)hook_id_mouse;
  //timer
  uint32_t irq_set_timer = BIT(hook_id_timer);
  uint8_t aux_timer = (uint8_t)hook_id_timer;


  if (subscribe_all(aux_timer, aux_keyboard, aux_mouse) != OK) return 1;

//--------------------------------
  int ipc_status;
  message msg;
  //1 is true
  int r;

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
  if (unsubscribe_all() != OK) return 1;

  return 0;
}



