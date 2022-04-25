#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int count = 0;
int hook_id = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t status;
  uint16_t div = TIMER_FREQ / freq; 
  timer_get_conf(timer, &status);
  uint8_t control = timer << 6;
  control = control | (status & 0x0F) | TIMER_LSB_MSB;
  uint8_t msb, lsb;
  util_get_LSB(div, &lsb);
  util_get_MSB(div, &msb);
  if (sys_outb(TIMER_CTRL, control) == OK)
    if (sys_outb(TIMER_0, lsb) == OK)
      if (sys_outb(TIMER_0, msb) == OK) return 0;
  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  int aux = (int) *bit_no;
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &aux) != OK) return 1;

  *bit_no = (uint8_t) aux;
  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id) != OK) return 1;
  return 0;
}

void (timer_int_handler)() {
  count++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if ((int) timer > 2) return 1;
  uint8_t timer_port = TIMER_0 + timer;
  uint8_t readBackCommand = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if (sys_outb(TIMER_CTRL, readBackCommand) == OK)
  {
    if (util_sys_inb(timer_port, st) == OK)
    {
      return 0;
    }
  }
  return 1;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  if (timer > 2) return 1;

  union timer_status_field_val val;
  if (field == tsf_all){
    val.byte = st;
  }
 
  if (field == tsf_initial){
    val.in_mode = (st & (BIT(4) | BIT(5))) >> 4;
  }
  if (field == tsf_mode){
    val.count_mode = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1;

    switch ((st & (BIT(1) | BIT(2) | (BIT(3)))) >> 1)
    {
    case 6:
      val.count_mode = 2;
      break;
    case 7:
      val.count_mode = 3;
      break;
    }
  }
  if (field == tsf_base){
    val.bcd = (st & BIT(0));
  }

  if (timer_print_config(timer,field, val) != OK) return 1;

  return 0;
}
