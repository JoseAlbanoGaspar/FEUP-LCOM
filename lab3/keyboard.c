#include <lcom/lcf.h>

#include <stdint.h>
#include "keyboard.h"

uint8_t status;
uint16_t scancode = 0x0000;
int hook_id = 1; //hook_id do teclado (o do timer é =0)

int (kbd_subscribe_int)(uint8_t *bit_no){
    int aux = (int)*bit_no;
    
    if(sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &aux))
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

  if(!util_sys_inb(OUT_BUF,&temp)) //OUT_BUF = output buffer port
      return 1; //error

  //scancode tem 2 bytes ou não
  if (temp == 0xE0)
    *scancode |= 0xE000;
  else
    *scancode |= temp;

  return 0; //success
}

int (kbd_read_status)(uint8_t * status){
  uint8_t temp;

  if(!util_sys_inb(STATUS_REG,&temp))
    return 1; //error

  *status = (uint8_t) temp;

  return 0; //success
}

int (isValidStatus)(){
  //first reads the status register
  if (kbd_read_status(&status) == 0)
    return 1; //error

  //looking for communications errors..
  if (status & KBC_OUTB) { //checks if the output buffer has data to read
    if ((status & (KBC_TIMEO | KBC_PARE | KBC_AUXB)) == 0) //checks if the data is valid
      return 0; //valid data
    else
        return 1; //data has errors
  }
  else
    return 1; //no data to read
}

<<<<<<< HEAD
=======
/**
 * The IH to implement must be a function that takes no arguments and returns no value.
 * The passing of data between the IH and the other functions of your implementation must be done via global variables.
 * The name of the IH must be kbc_ih()
 */
>>>>>>> d870e7f952e29869904bac62488adb808dcb41cb
void (kbc_ih)(void) {
    /**
     * The IH should:
        1- read the status register and check if there was some communications error;
        2- to read the scancode byte from the output buffer;
     */
  if(isValidStatus()){ //1
    kbd_read_buffer(&scancode); //2
    //printf("Scancode: %X\n", scancode); //não é aqui que se deve dar print dos scancodes
  }
}

int (kbc_poll)(){ //com os meus updates, deixa de ser precisa

  util_sys_inb(STATUS_REG, &status); //(0x64,status) //incrementa o counter

  kbc_ih();
  if (scancode != FIRST_OF_TWO_BYTES) //se tiver 2 bytes?
      kbc_print(); //FIRST_OF_TWO_BYTES - 0xE000 (First byte of two byte scan code)
  else { //se só tiver 1 byte?
      kbd_read_buffer(&scancode);
      kbc_print();
  }
  return 0; //success

  ///versão mais simples disto acima
  /*if ((status & (KBC_TIMEO | KBC_PARE | KBC_AUXB)) == 0){ //ver se tem erros
    if (status & KBC_OUTB){ //ver se o buffer está vazio
      kbd_read_buffer(&scancode); //reads the buffer
      if (scancode != FIRST_OF_TWO_BYTES) //se tiver 2 bytes?
          kbc_print(); //FIRST_OF_TWO_BYTES - 0xE000 (First byte of two byte scan code)
      else { //se só tiver 1 byte?
        kbd_read_buffer(&scancode);
        kbc_print();
      }
      return 0;
    }
    else return 0;
  }
  else return 1;*/
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
    bytes[0] = temp2; //bytes[0] = 1ºbyte de scancode, que é o mais significativo
    bytes[1] = temp1; //bytes[1] = 2ºbyte de scancode
    size = 0x02;
    if ((bytes[1] & 0x80) == 0x80) make = false; //0x80 = 10000000
    //make = false = breakcode
    /**
     * @make indicates whether this is a make or a break code
     * @size is the size of the scancode in bytes
     * @bytes is an array with the bytes of the scancode, in order
     */
    kbd_print_scancode(make, size, bytes);
  }
  else{
    if ((temp1 & 0x80) == 0x80) make = false;
    kbd_print_scancode(make, size, &temp1);
  }
  if (scancode != 0x81) scancode = 0x0000; 
}

int (kbc_commandByte)(uint8_t commandByte){

  //to read command byte
  sys_outb(STATUS_REG, READ_CMD_B); //write command 0x20 no port 0x64

  //lê o command byte do buffer
  if (!util_sys_inb(OUT_BUF, &commandByte)) //command byte vai ser igual ao valor que está no buffer
    return 1;

  //to write command byte
  sys_outb(STATUS_REG, WRITE_CMD_B); //write command 0x60 no port 0x64

  commandByte = commandByte | BIT(0); //poe o bit 0 do command byte a 1??

  //escreve o command byte no buffer
  sys_outb(OUT_BUF, commandByte); //OUT_BUF vai ter o valor de commandByte

  return 0; //success
}

