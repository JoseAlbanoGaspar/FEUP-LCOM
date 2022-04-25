#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

//Timer to be incremented by the timer interrupts
int count = 0;
//Hook id to be used to set the interrupt policy
int hook_id = 0; //hook_id do timer
//o do teclado é =1


//implement for timer_test_time_base
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if(timer > 2 || timer < 0 || freq <= 0 )
    return 1; //error

  // Remember that the frequency of the Clock input of all timers is 1 193 181 Hz
  uint16_t initial_value = TIMER_FREQ / freq;
  uint8_t control; //vai ser preenchido com a configuração do timer através da função timer_get_conf
  uint8_t timer_port = TIMER_0 + timer; //registo do timer (timer 0 = 0x40)

  //Write control word to configure Timer 0:
  if(timer_get_conf(timer,&control) == 0){ //se não deu erro
    //Do not change 4 least-significant bits:
    control = control & 15; // 15 => F => 1111 --> mantem apenas os 4 primeiros bits
    //mantém os bits 3,2,1 e 0 correspondentes ao modo de operação do timer (modo 0 a 5) e ao booleano que indica se está em binário ou não

    //This function should work for every timer, not only Timer 0:
    switch(timer){
      case 0:
        // You need to read the Timer 0 configuration first
        // Preferably, LSB followed by MSB
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

  //LOAD initial_value (que é o valor da frequência desejada) into timer (lsb followed by msb)
  uint8_t lsb;
  if(util_get_LSB(initial_value,&lsb) == 0) //se não houver erro:
    sys_outb(timer_port,lsb); //sending lsb to the timer_port (que no caso do timer 0 é para o registo 0x40)

  uint8_t msb;
  if(util_get_MSB(initial_value,&msb) == 0) //se não houver erro:
    sys_outb(timer_port,msb); //sending lsb to the timer_port (que no caso do timer 0 é para o registo 0x40)

  return 0;

}

//implement for timer_test_int()
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

//implement for timer_test_int()
int (timer_unsubscribe_int)() {
  //Unsubscribing the interruptions
  if(sys_irqrmpolicy(&hook_id))
    return 1; //error

  return 0; //success
}

//implement for timer_test_int()
void (timer_int_handler)() {
  count++;
}

///vai preencher st com a configuração do timer, que vai ser usado como parametro na timer_display_conf (no caso da primeira função do lab2)
int (timer_get_conf)(uint8_t timer, uint8_t *st) { //st - Address of memory position to be filled with the timer config
  //uint8_t timer - Timer whose configuration to read (Ranges from 0 to 2)
  if (timer > 2 || timer < 0)
    return 1; //error

  //Make sure 2 MSBs are both 1 (bits 7 e 6) | Select only the status (not the counting value) | ...
  uint8_t readBackCommand = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  //ambos os bits 6 e 7 a 1 = é um RB command e não uma control word| bit 5 a 1 = não vai ler o valor do counter (active low) | bit timer + 1 a 1 = timer do argumento ativo/selecionado

  //Read the timer port
  uint8_t tmr = TIMER_0 + timer; //registo do timer = 0x40 + 0 fica o do timer 0, 0x40 + 1 fica o do timer 1 ...

  //Analisar a conf. lida(?)
  if (sys_outb(TIMER_CTRL, readBackCommand) == OK) //porque o RB command é escrito no control register tal como a control word
    //chamamos o util_sys_inb porque queremos fazer um sys_inb mas o argumento tem 8 bits e o sys_inb tem como parametro um de 32 bits
    if (util_sys_inb(tmr, st) == OK) //util_sys_inb faz um sys_inb mas com um argumento de 8 bits; escreve o timer na pos de memória st
      return 0; //success

  return 1; //error
}


///usa o st preenchido pela função timer_get_conf e com ele preenche a union timer_status_field_val, depois pode chamar a função timer_print_config
int (timer_display_conf)(uint8_t timer,
                        uint8_t st /* tem a conf. do timer (proveniente da timer_get_conf)*/,
                        enum timer_status_field field /*status field to display in human friendly way*/) {

  /** - stuff provided to us:
   * enum - is a user-defined type that can take one of a finite number of values
   * enum timer_status_field { //são as opções de input da função timer_test_read_config e da timer_display_conf
        tsf_all,        // configuration in hexadecimal
        tsf_initial,    // timer initialization mode
        tsf_mode,       // timer counting mode
        tsf_base        // timer counting base
    };
   */

  if (timer > 2 || timer < 0)
    return 1; //error

  /** - stuff provided to us:

   * union timer_status_field_val {
        uint8_t byte;               // status, in hexadecimal
        enum timer_init in_mode;    // initialization mode; LSB, MSB ou os dois
        uint8_t count_mode;         // counting mode: 0, 1, ..., 5; escolher o moddo de operação do timer
        bool bcd;                   // true, if counting in BCD; porque 1 => true
    };

   //Union - contains space to store any of its members, but not all of its members simultaneously
   enum timer_init { //vai se buscar à tabela do control word
        INVAL_val,      //Invalid initialization mode
        LSB_only,       //Initialization only of the LSB
        MSB_only,       //Initialization only of the MSB
        MSB_after_LSB   //Initialization of LSB and MSB, in this order
    };

   */
  union timer_status_field_val config;

  if (field == tsf_all)
    config.byte = st;

  if (field == tsf_initial)
    config.in_mode = (st & (BIT(4) | BIT(5))) >> 4; //deslocamento para os bits 5 e 4 ficarem como os bits menos significativos

  if (field == tsf_mode) {
    config.count_mode = (st & (BIT(1) | BIT(2) | (BIT(3)))) >> 1;

    ///esta parte não percebi!
    switch ((st & (BIT(1) | BIT(2) | (BIT(3)))) >> 1) {
    case 6:
      config.count_mode = 2;
      break;
    case 7:
      config.count_mode = 3;
      break;
    }
  }

  if (field == tsf_base)
    config.bcd = (st & BIT(0)) >> 0; //aqui não era preciso o >> 0

  if (timer_print_config(timer, field, config))
    return 0; //success

  return 1; //error
}

