#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  uint16_t initial_value = TIMER_FREQ / freq;
  if(timer > 2 || freq == 0 )
    return 1;
  uint8_t control;
  uint8_t timer_port = TIMER_0 + timer;
  if(timer_get_conf(timer,&control) == 0){
    control = control & 15; // 15 => F => 1111 --> mantem os 4 primeiros bits

    switch(timer){
      case 0:
        control = TIMER_SEL0 | TIMER_LSB_MSB | control;
        break;
      case 1:
        control = TIMER_SEL1 | TIMER_LSB_MSB | control;
        break;
      case 2:
        control = TIMER_SEL2 | TIMER_LSB_MSB | control;
        break;
    }
    sys_outb(TIMER_CTRL,control); //sending the control word to the TIMER_CTRL
  }
  //LOAD initial_value into timer (lsb followed by msb)
  uint8_t lsb;
  if(util_get_LSB(initial_value,&lsb) == 0)
    sys_outb(timer_port,lsb);
  uint8_t msb;
  if(util_get_LSB(initial_value,&msb) == 0)
    sys_outb(timer_port,msb);
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  /* To be implemented by the students */
  if (timer > 2)
    return 1;
  
  uint8_t readBackCommand = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  uint8_t tmr = TIMER_0 + timer;
  if (sys_outb(TIMER_CTRL, readBackCommand) == OK)
  {
    
    if (util_sys_inb(tmr, st) == OK)
    {
      return 0;
    }
  }
  return 1;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  /* To be implemented by the students */

  
if (timer > 2)
    return 1;
  union timer_status_field_val config;
  if (field == tsf_all)
  {
    config.byte = st;
  }
  if (field == tsf_initial)
  {
    config.in_mode = (st & (BIT(4) | BIT(5))) >> 4;
  }
  if (field == tsf_mode)
  {
    config.count_mode = (st & (BIT(1) | BIT(2) | (BIT(3)))) >> 1;
    
    switch ((st & (BIT(1) | BIT(2) | (BIT(3)))) >> 1)
    {
    case 6:
      config.count_mode = 2;
      break;
    case 7:
      config.count_mode = 3;
      break;
    }
  }
  if (field == tsf_base)
  {
    config.bcd = (st & BIT(0)) >> 0;
  }
  if (timer_print_config(timer, field, config))
    return 0;
  return 1;
}
