/*#include <lcom/lab3.h>

#include "keyboard.c"

#include <stdbool.h>*/

#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
#include "keyboard.h"

extern int hook_id;
extern uint16_t scancode;
extern int global_counter;


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  //Here we select the bit in the hook_id needed to check if we got the right interruption
  uint32_t irq_set = BIT(hook_id);

  uint8_t aux = (uint8_t)hook_id;

  //Subscription of the interruption
  if(kbd_subscribe_int(&aux))
    return 1;

  hook_id = (int)aux;
  
  int ipc_status;
  message msg;
  //1 is true
  int r;
  int dont_screw_upCounter = 0;
  while (scancode != 0x81) {
    if (dont_screw_upCounter == 100) {
      break;
    }
    dont_screw_upCounter++;
    // Get a request message
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:

          // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set) { // subscribed interrupt
            kbc_ih();
            if (scancode != 0xE000){
              uint8_t size = 0x01;
              bool make = true;
              uint8_t temp1 = (uint8_t) scancode;
              uint16_t tempcode = scancode;
              tempcode = tempcode >> 8; //get the most significant byte;
              uint8_t temp2 = (uint8_t) tempcode; 
              if (temp2 != 0x00){
                uint8_t bytes[2];
                bytes[0] = temp2;
                bytes[1] = temp1;
                size = 0x02;
                if ((bytes[1] & 0x80) == 0x80) make = false;
                kbd_print_scancode(make, size, bytes);
              }
              else{
                if ((temp1 & 0x80) == 0x80) make = false;
                kbd_print_scancode(make, size, &temp1);
              }
              if (scancode != 0x81)
                scancode = 0x0000;
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
  kbd_unsubscribe_int();
  kbd_print_no_sysinb(global_counter);
  return 1;

  /* 
  if receives te break code of ESC (0x81), then:
  -ubsubscribe KBC interrupt
  -print the number of sys_inb kernel calls (function that does this is provided(int kbd_print_no_sysinb(unint32_t cnt);))
  -to count the number of sys_inb() calls we must use the approach based on a wrapper function as described in the lecture notes
  -return 1
  */
}

int(kbd_test_poll)() {
  /* To be completed by the students */
  /*
  -Must not use interrupts
  -should call kbd_print_scancode()
  -if receives te break code of ESC (0x81), then:
    -ubsubscribe KBC interrupt
    -print the number of sys_inb kernel calls (function that does this is provided(int kbd_print_no_sysinb(unint32_t cnt);))
    -to count the number of sys_inb() calls we must use   the approach based on a wrapper function as described in the lecture notes
    -return 1
  -Must enble interrupts before returning, by writing an appropriate KBC command type
  */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);
  /*
    Similarities to kbd_test_scan():
      -like kbd_test_scan(), subscribe to keyboard interrupts and print the scancodes received by invoking kbd_print_scancode()
    Differences:
      -not only exit when user releases ESC key, but also if it does not receive a scan code for a number o seconds equal to the argument n:
        ->measure time:
          -must use the interrups of the PC's Timer 0, no configuration changes needed, only to subscribe to its interrrupts as done in timer_test_int() of Lab2.
  */

  return 1;
}
