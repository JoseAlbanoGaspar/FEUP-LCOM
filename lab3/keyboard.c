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
  if(!util_sys_inb(OUT_BUF,&temp)) return 1; //OUT_BUF = output buffer port

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

int (kbc_poll)(){

  util_sys_inb(STATUS_REG, &status); //(0x64,status) //incrementa o counter

  if ((status & (KBC_TIMEO | KBC_PARE | KBC_AUXB)) == 0){ //ver se tem erros
    if (status & KBC_OUTB){ //ver se o buffer está vazio
      kbd_read_buffer(&scancode); //reads the buffer
      if (scancode != FIRST_OF_TWO_BYTES) kbc_print(); //FIRST_OF_TWO_BYTES - 0xE000 (First byte of two byte scan code)
      else {
        kbd_read_buffer(&scancode);
        kbc_print();
      }
      return 0;
    }
    else return 0;
  }
  else return 1;
}

void (kbc_print)(){
  uint8_t size = 0x01;
  bool make = true;
  uint8_t temp1;
  uint8_t temp2;
  util_get_LSB(scancode, &temp1);  
  util_get_MSB(scancode, &temp2);

  if (temp2 != 0x00) { //se o primeiro byte for != 0, o scancode tem 2 bytes
    uint8_t bytes[2]; //array bytes de uint8_t
    bytes[0] = temp2; //bytes[0] = 1ºbyte de scancode
    bytes[1] = temp1; //bytes[1] = 2ºbyte de scancode
    size = 0x02;
    if ((bytes[1] & 0x80) == 0x80) make = false; //0x80 = 10000000
    kbd_print_scancode(make, size, bytes);
  }
  else{
    if ((temp1 & 0x80) == 0x80) make = false;
    kbd_print_scancode(make, size, &temp1);
  }
  if (scancode != 0x81) scancode = 0x0000; 
}

int (kbc_commandByte)(uint8_t commandByte){
  sys_outb(STATUS_REG, READ_CMD_B);
  //uint32_t commandByte;
  util_sys_inb(OUT_BUF, &commandByte);
  sys_outb(STATUS_REG, WRITE_CMD_B);
  commandByte = commandByte | BIT(0);
  sys_outb(OUT_BUF, commandByte);
  return 1;
}


