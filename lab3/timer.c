#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

//Timer to be incremented by the timer interrupts
int count = 0;
//Hook id to be used to set the interrupt policy
int hook_id_timer = 0;

///timer.c only needed by the last function of this lab

int (timer_subscribe_int)(uint8_t *bit_no) {
    //This function should be used to subscribe a notification on every interrupt in the input irq_line
    //It returns, via its arguments, the bit number, that will be set in msg.m_notify.interrupts upon a TIMER 0 interrupt

    //Auxiliaty variable used to pass and get the return of sys_irqsetpolicy, porque o parametro hook_id aqui é como int
    int aux = (int)*bit_no;

    //used to subscribe the interruption:
    //note policy use IRQ_REENABLE to inform the GIH that it can give the EOI command
    if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &aux))
        return 1; //error

    //Used for the return
    *bit_no = (uint8_t)aux;

    return 0; //success

}

int (timer_unsubscribe_int)() {
    //Unsubscribing the interruptions
    if(sys_irqrmpolicy(&hook_id_timer))
        return 1; //error

    return 0; //success
}

//implement for timer_test_int()
void (timer_int_handler)() {
    count++;
}

