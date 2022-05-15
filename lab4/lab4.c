// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "mouse.h"

extern struct packet mouse_packet; // data packet of 3 bytes
extern int hook_id;  
extern int count;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
  //Here we select the bit in the hook_id needed to check if we got the right interruption
  uint32_t irq_set = BIT(hook_id);

  uint8_t aux = (uint8_t)hook_id;

  //Subscription of the interruption
  if(mouse_subscribe_int(&aux))
    return 1;

  /* provided by minix */
  if (mouse_enable_data_reporting())
    return 1;

  hook_id = (int)aux;


  int ipc_status;
  int packet_count = 0;
  message msg;
  //1 is true
  int r;
<<<<<<< HEAD
  mouse_enable_data_reporting();
  while (packet_count < (int) cnt) {
=======

  while (count /* packet_count here, no? */ < (int) cnt) { //The KBC generates a mouse interrupt for each byte of the packet.
>>>>>>> f70765b0ff27c8248ac2e8b688bd268288a800d4
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
            mouse_ih();
            count++; //received another packet
            if (count == 3){ //upon receiving the 3rd byte of a mouse packet, the program should parse it and print it on the console
                packet_count++;
                count = 0;
                mouse_print_packet(&mouse_packet);
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

  //if (mouse_enable_data_reporting()) return 1; // enables mouse data reporting
  if (mouse_reset()) return 1;
  if (mouse_unsubscribe_int()) return 1; // unsubscribes interrupts

  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    /* To be completed */
    printf("%s(%u): under construction\n", __func__, idle_time);
    return 1;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}

