#include "mouse.h"

void (mouse_ih)(void){
    /*manages mouse interrupts*/
}

int (mouse_subscribe_int)(uint8_t *bit_no){
    int aux = (int)*bit_no;
    
    if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,&aux))
        return 1;

    *bit_no = (uint8_t)aux;
    return 0;
}

int (mouse_unsubscribe_int)(){
    //Unsubscribing the interruptions
    if(sys_irqrmpolicy(&hook_id))
    return 1;

    return 0;
}

int (mouse_reset)(){
    uint8_t ack;
    if (sys_outb(0x64, MOUSE_RESET) != OK) return 1;
    if (util_sys_inb(0x60, &ack) == 1) return 1;
    else {
        if (ack == MOUSE_IS_OK) return 0;
        else if (ack == MOUSE_NACK) return mouse_reset();
        else if (ack == MOUSE_ACK_ERROR) return 1;
    }
    printf("Error value: %x\n", ack);
}
