#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
#include "keyboard.h"

extern int hook_id;
extern int hook_id_timer;
extern int count; //used in 3rd function
extern uint16_t scancode; //
extern int global_counter;
extern uint8_t status;


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {

  //Here we select the bit in the hook_id needed to check if we got the right interruption
  uint32_t irq_set = BIT(hook_id); //bit mask para poder saber qual a interrupt que ocorreu
  uint8_t aux = (uint8_t)hook_id;
  if(kbd_subscribe_int(&aux)) //Subscription of the interruption
    return 1; //error
  hook_id = (int)aux; //aqui deve estar ativado por causa da subscrição
  
  int ipc_status;
  message msg;
  int r;

  while (scancode != ESC_KEY) {
    // Get a request message
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification: Every time the IH receives a byte from the keyboard,
                                     // kbd_test_scan() should try to assemble a scancode.
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: // hardware interrupt notification:
          if (msg.m_notify.interrupts & irq_set) { // subscribed interrupt

            kbc_ih();
            if (scancode != FIRST_OF_TWO_BYTES) // se não tiver 2 bytes?
              kbc_print(); //If it succeeds it should print it on the console, by calling kbd_print_scancode()

          }
          break;
        default:
          break; // no other notifications expected: do nothing
      }
    }
    else { //received a standard message, not a notification
      // no standard messages expected: do nothing
    }
  }

  if (kbd_unsubscribe_int()) //Cancel the subscription of the KBC interrupt, for the reasons described in the previous section;
    return 1; //error

  /**
   * @global_counter is the total count of sys_inb() kernel calls by your program.
   */
  kbd_print_no_sysinb(global_counter); //Print the number of sys_inb kernel calls

  return 0; //success
}

int(kbd_test_poll)() { //aqui não há subscrições
  uint8_t command_byte = 0x00; //inicializar o command byte

  while(scancode != ESC_KEY){
    //polls the KBC for new scancodes
    //kbc_poll();
    ///mudei um bocadinho da estrutura..

    if (!util_sys_inb(STATUS_REG, &status)) //(0x64,status) //incrementa o counter
        return 1; //error

    kbc_ih();

    if (scancode != FIRST_OF_TWO_BYTES) //se tiver 2 bytes?
        kbc_print(); //FIRST_OF_TWO_BYTES - 0xE000 (First byte of two byte scan code)
    else { //se só tiver 1 byte?
        if (kbd_read_buffer(&scancode))
            return 1; //error
        kbc_print();
    }

  }

  if (kbd_print_no_sysinb(global_counter))
    return 1; //error

  //resets the command byte
  if (kbc_commandByte(command_byte))
      return 1; //error

  return 0; //success

}

int(kbd_test_timed_scan)(uint8_t n){

    uint32_t irq_set = BIT(hook_id); //Here we select the bit in the hook_id needed to check if we got the right interruption
    uint32_t irq_set_timer = BIT(hook_id_timer);

    uint8_t aux = (uint8_t)hook_id;
    if(kbd_subscribe_int(&aux)) //subscribe a notification on every interrupt in the input irq_line of kbd
        return 1; //error
    hook_id = (int)aux; //aqui deve estar ativado por causa da subscrição

    aux = (uint8_t)hook_id_timer;
    if(timer_subscribe_int(&aux)) //subscribe a notification on every interrupt in the input irq_line of timer
        return 1;
    hook_id_timer = (int)aux;


    int ipc_status;
    message msg;
    int r;

    while (scancode != ESC_KEY && (count < n * 60)) {
        // Get a request message
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { // received notification: Every time the IH receives a byte from the keyboard,
            // kbd_test_scan() should try to assemble a scancode.
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    // hardware interrupt notification:

                    if (msg.m_notify.interrupts & irq_set) {
                        kbc_ih();
                        if (scancode != FIRST_OF_TWO_BYTES) // se não tiver 2 bytes?
                            kbc_print(); //If it succeeds it should print it on the console, by calling kbd_print_scancode()
                        count = 0;
                    }

                    if (msg.m_notify.interrupts & irq_set_timer)
                        timer_int_handler();

                    break;

                default:
                    break; // no other notifications expected: do nothing
            }
        }
        else { //received a standard message, not a notification
            // no standard messages expected: do nothing
        }
    }

    //In the end we unsubscribe from the timer interruptions
    if(timer_unsubscribe_int())
        return 1; //error

    if (kbd_unsubscribe_int()) //Cancel the subscription of the KBC interrupt, for the reasons described in the previous section;
        return 1; //error

    /**
     * @global_counter is the total count of sys_inb() kernel calls by your program.
     */
    kbd_print_no_sysinb(global_counter); //Print the number of sys_inb kernel calls

    return 0; //success
}

