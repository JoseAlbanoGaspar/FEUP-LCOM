// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <iMouse.h>

extern struct packet mouse_packet;
uint32_t mouse_status;
extern int bytes_count;
int bit_no_global_mouse;
int hook_id = MOUSE_IRQ;
uint8_t packet_byte;

int(mouse_subscribe_int)(uint8_t *bit_no);
int(mouse_unsubscribe_int)();
int(mouse_get_status)();
int(mouse_check_status)();
void(parse)();
void(mouse_ih)();
int (re_enable_mouse_interrupts)();
int (mouse_reset)();

