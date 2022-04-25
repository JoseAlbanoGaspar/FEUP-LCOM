// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include "int86.h"
//#include "register.c"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR	0xF0000000
#define H_RES             1024
#define V_RES		  768
#define BITS_PER_PIXEL	  8

/* Private global variables */

//static char *video_mem;		/* Process address to which VRAM is mapped */

//static unsigned h_res = H_RES;		/* Horizontal screen resolution in pixels */
//static unsigned v_res = V_RES;		/* Vertical screen resolution in pixels */
//static unsigned bits_per_pixel = BITS_PER_PIXEL; /* Number of VRAM bits per pixel */


//Timer to be incremented by the timer interrupts
extern int count;
//Hook id to be used to set the interrupt policy
extern int hook_id_timer;
int hook_id = 1;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  struct reg86u r86;

  r86.u.b.intno = INT_NO;
  r86.u.b.ah = VBE_CALL;   
  r86.u.b.al = SET_VBE_MODE;    
  r86.u.w.bx = mode;

  
  if( sys_int86(&r86) != OK ) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return 1;
  }
  
  uint32_t irq_set = BIT(hook_id);
  uint32_t irq_set_timer = BIT(hook_id_timer);
  uint8_t aux = (uint8_t)hook_id;
  hook_id = (int)aux;
  aux = (uint8_t)hook_id_timer;
  if(timer_subscribe_int(&aux))
    return 1;
  hook_id_timer = (int)aux;
  int ipc_status;
  message msg;
  //1 is true
  int r;
  while (count < (int) delay * 60) {

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
            count = 0;
          }
          if(msg.m_notify.interrupts & irq_set_timer){
            timer_int_handler();
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
  timer_unsubscribe_int();
  vg_exit();
  return 1;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  /* To be completed */
  printf("%s(0x%03X, %u, %u, %u, %u, 0x%08x): under construction\n",
         __func__, mode, x, y, width, height, color);

  return 1;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  /* To be completed */
  printf("%s(0x%03x, %u, 0x%08x, %d): under construction\n", __func__,
         mode, no_rectangles, first, step);

  return 1;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* To be completed */
  printf("%s(%8p, %u, %u): under construction\n", __func__, xpm, x, y);

  return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}

