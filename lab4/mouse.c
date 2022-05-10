#include "mouse.h"

extern struct packet mouse_packet;
uint32_t mouse_status;
int count = 0;
int bit_no_global_mouse;
int hook_id = MOUSE_IRQ;
uint8_t packet_byte;
struct packet mouse_packet;

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

int mouse_get_status()
{
    if (sys_inb(MOUSE_STATUS_REG, &mouse_status) == OK)
    {
        return 0;
    }
    return 1;
}

int mouse_check_status()
{
    if ((mouse_status & MOUSE_OUTB) != 0)  //check if buffer has data to read
    {
        if ((mouse_status & (MOUSE_PARE | MOUSE_TIMEO)) == 0) //check if there are errors
        {
            if ((mouse_status & MOUSE_AUXB) != 0)  //check if the data is for the mouse 
            {
                return 0;
            }
        }
    }
    return 1;
}

void parse()
{
    /* Preencher Struct mouse_packet definida em lab4.c */

    switch (count)
    {
    case 0:
        mouse_packet.bytes[0] = packet_byte;
        mouse_packet.lb = (packet_byte & MOUSE_LB_PRESSED);
        mouse_packet.rb = (packet_byte & MOUSE_RB_PRESSED);
        mouse_packet.mb = (packet_byte & MOUSE_MB_PRESSED);
        mouse_packet.y_ov = (packet_byte & MOUSE_Y_OVF);
        mouse_packet.x_ov = (packet_byte & MOUSE_X_OVF);
        break;
    case 1:
        mouse_packet.bytes[1] = packet_byte;
        if ((mouse_packet.bytes[0] & MOUSE_X_DELTA_SIGN))
        {
            mouse_packet.delta_x = packet_byte;
        }
        else
            mouse_packet.delta_x = 0;
        break;
    case 2:
        mouse_packet.bytes[2] = packet_byte;
        if ((mouse_packet.bytes[0] & MOUSE_Y_DELTA_SIGN))
        {
            mouse_packet.delta_y = packet_byte;
        }
        else
            mouse_packet.delta_y = 0;
        break;
    }
}

void(mouse_ih)()
{
    if (mouse_get_status() == OK)
    {
        if (mouse_check_status() == OK)
        {
            if (util_sys_inb(MOUSE_OUT_BUF, &packet_byte) == OK)
            {
                parse();
            }
        }
    }
}

int(re_enable_mouse_interrupts)()
{
    return 0;
}

int (mouse_reset)(){
    uint8_t ack;
    if (sys_outb(KBC_CMD, MOUSE_RESET) != OK) return 1;
    if (util_sys_inb(OUT_BUF, &ack) == 1) return 1;
    else {
        if (ack == MOUSE_OK) return 0;
        else if (ack == MOUSE_NACK) return mouse_reset();
        else if (ack == MOUSE_ACK_ERROR) return 1;
    }
    printf("Error value: %x\n", ack);
    return 1;
}

int (mouse_en_data_report)(){
    uint8_t ack;
    if (sys_outb(KBC_CMD, MOUSE_ENABLE_CMD) != OK) return 1;
    if (util_sys_inb(OUT_BUF, &ack) == 1) return 1;
    else {
        if (ack == MOUSE_OK) return 0;
        else if (ack == MOUSE_NACK) return mouse_reset();
        else if (ack == MOUSE_ACK_ERROR) return 1;
    }
    printf("Error value: %x\n", ack);
    return 1;
}
