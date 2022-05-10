#include "mouse.h"

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
    if ((mouse_status & BIT(0)) != 0)  //check if buffer has data to read
    {
        if ((mouse_status & (BIT(7) | BIT(6))) == 0) //check if there are errors
        {
            if ((mouse_status & BIT(5)) != 0)  //check if the data is for the mouse 
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

    uint32_t packet;

    switch (bytes_count)
    {
    case 0:
        mouse_packet.bytes[0] = packet_byte;
        mouse_packet.lb = (packet_byte & BIT(0));
        mouse_packet.rb = (packet_byte & BIT(1));
        mouse_packet.mb = (packet_byte & BIT(2));
        mouse_packet.y_ov = (packet_byte & BIT(7));
        mouse_packet.x_ov = (packet_byte & BIT(6));
        break;
    case 1:
        mouse_packet.bytes[1] = packet_byte;
        if ((mouse_packet.bytes[0] & BIT(4)))
        {
            packet = (packet_byte | 65280);
            mouse_packet.delta_x = packet;
        }
        else
            mouse_packet.delta_x = packet_byte;
        break;
    case 2:
        mouse_packet.bytes[2] = packet_byte;
        if ((mouse_packet.bytes[0] & BIT(5)))
        {
            packet = (packet_byte | 65280);
            mouse_packet.delta_y = packet;
        }
        else
            mouse_packet.delta_y = packet_byte;
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

