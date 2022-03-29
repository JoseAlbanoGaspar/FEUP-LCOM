#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include "scan.c"

#include <stdbool.h>
#include <stdint.h>

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
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  /* 
  if receives te break code of ESC (0x81), then:
  -ubsubscribe KBC interrupt
  -print the number of sys_inb kernel calls (function that does this is provided(int kbd_print_no_sysinb(unint32_t cnt);))
  -to count the number of sys_inb() calls we must use the approach based on a wrapper function as described in the lecture notes
  -return 1
  */
  return 1;
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