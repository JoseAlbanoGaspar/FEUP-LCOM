#include <lcom/lcf.h>
#include <lcom/keyboard.h>

#include <stdint.h>

#include "i8042.h"

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

int (kbd_read_buffer)(uint8_t * data){

}
int (kbd_read_status) (uint8_t * status){

}
int (isValidStatus)(){
    
}
 void (kbc_ih)(void) {
     if(isValidStatus()){
         kbd_read_buffer(&scancode);
     }
 }
