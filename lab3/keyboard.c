/*#include <lcom/lab3.h>
#include "keyboard.h"
#include "utils.c"*/

#include <lcom/lcf.h>

#include <stdint.h>
#include "keyboard.h"

uint8_t status;
uint16_t scancode = 0x0000;
int hook_id = 1;

int (kbd_subscribe_int)(uint8_t *bit_no){
    int aux = (int)*bit_no;
    
    if(sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,&aux))
        return 1;

    *bit_no = (uint8_t)aux;
    return 0;
}

int (kbd_unsubscribe_int)() {
  //Unsubscribing the interruptions
  if(sys_irqrmpolicy(&hook_id))
    return 1;

  return 0;
}

int (kbd_read_buffer)(uint16_t * scancode){
  uint8_t temp = 0x00;
  if(!util_sys_inb(OUT_BUF,&temp)) return 1;

  if (temp == 0xE0){
    *scancode |= 0xE000;
  }
  else{
    *scancode |= temp;
  }
  return 0;
}

int (kbd_read_status)(uint8_t * status){
  uint8_t temp;

  if(!util_sys_inb(STATUS_REG,&temp))
    return 1;
  *status = (uint8_t) temp;
  return 0;
}

int (isValidStatus)(){
    //first reads the status register
  if (kbd_read_status(&status) == 0) {
    return 1;
  }

  //checks if the ouput buffer has data to read
  if (status & KBC_OUTB) {
    //checks if the data is valid 
    if ((status & (KBC_PARE | KBC_TIMEO | KBC_AUXB)) == 0) {
      return 0;
    }
    else {
      return 1; //data has errors
    }
  }
  else
    return 1; //no data to read
}
void (kbc_ih)(void) {
  if(isValidStatus()){
    kbd_read_buffer(&scancode);
    //printf("Scancode: %X\n", scancode);
  }
}
