#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

extern int hook_id;
extern int count;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
<<<<<<< HEAD
  uint8_t st;
  if (timer_get_conf(timer, &st) == 0 && timer_display_conf(timer, st, field)== 0)
    return 0;

  return 1;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  if (timer_set_frequency(timer, freq) == OK) return 0; 


  return 1;
=======
  uint8_t r;
  if(timer_get_conf(timer,&r) == 0 && timer_display_conf(timer,r,field) == 0)
    return 0; //success
  return 1; //error
}

//Tests change of Timer O interrupt frequency, but also works for other timers
int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  if(timer_set_frequency(timer,freq) == 0)
    return 0; //success
  return 1; //error
>>>>>>> 0a5f24c698faf98c8241aff8609b31dd625e802e
}

//Subscribes Timer 0 interrupts and prints a message once per second for the specified time interval in seconds.
int(timer_test_int)(uint8_t time) {
<<<<<<< HEAD
  uint8_t r;
  int ipc_status;
  message msg;
  uint8_t irq_set = BIT(hook_id);
  uint8_t aux = (uint8_t) hook_id;
  if (timer_subscribe_int(&aux) != OK) return 1;
  hook_id = (int) aux;

  while(count < time * 60) { /* You may want to use a different condition */
     /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */				
          if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
=======

  //Here we select the bit in the hook_id needed to check if we got the right interruption
  uint32_t irq_set = BIT(hook_id); //começa com hook_id = 0, logo irq_set começa com o bit 0 a 1

  uint8_t aux = (uint8_t)hook_id; //aux = hook_id só que no tipo uint8_t, para poder ser usado como um registo e ser usado como parametro da função timer_subscribe_int
  if(timer_subscribe_int(&aux)) //Subscription of the interruption
    return 1; //error
  hook_id = (int)aux;

  int ipc_status; //it is throw IPC that the device driver receives the notification of the GIH (Generic IH)
  message msg;
  int r;

  //The while only runs 60 * the time requested because the timer interrupts 60 times per second
  while(count < time * 60){ //count começa = 0
    /* Get a request message */
    if((r=driver_receive(ANY,&msg,&ipc_status)) != 0){
      /** - é usado pelos DD para receber menaagens incluindo notificações (do kernel ou de outros processos):
        ANY: The value ANY means that the driver accepts messages from any process
        &msg: message that is received
        &ipc_status: IPC - This is essentially a message based mechanism
       */
// => Device Driver receive the notification of the GIH just by use the IPC mechanism
      printf("driver_receive failed with: %d",r);
      continue;
    }
    /** is_ipc_notify()
     * returns true if the message received is a notification or false otherwise,
     * example: if it is a standard message.
     */
    if(is_ipc_notify(ipc_status)){ /* Received notication */
      /**
       * O endpoint é um "endereço" usado pelo Minix 3 IPC para especificar a origem e o destino de uma mensagem.
       */
      switch(_ENDPOINT_P(msg.m_source)){
        /**
         * HARDWARE: é um valor de identificador de processo especial para indicar uma notificação de interrupção HW.
         */
        case HARDWARE:  /* Hardware interrupt notification */
          /**
           * .m_notify.interrupts: é uma máscara de 32 bits com os bits de interrupções assinadas pendentes definidas como 1
           * irq_set: é uma máscara de bits e é usada para descobrir qual interrupção ocorreu, a fim de executar o interrupt handler apropriado.
           */
          if(msg.m_notify.interrupts & irq_set){
            /**
             * In this lab, we will subscribe only one interrupt, and therefore we will need
             * to test the occurrence of that interrupt, and process it
             */
            //In each interruption we increment the counter
>>>>>>> 0a5f24c698faf98c8241aff8609b31dd625e802e
            timer_int_handler();
      

<<<<<<< HEAD
            if (count % 60 == 0) timer_print_elapsed_time();
=======
            //It only prints each second that is calculated by making the modulo operation with 60, because we assume the timer is operating at a 60Hz frequency
            if (count % 60 == 0)
              timer_print_elapsed_time();
>>>>>>> 0a5f24c698faf98c8241aff8609b31dd625e802e
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */	
      }
    } 
    else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }
 }

  if (timer_unsubscribe_int() != OK) return 1;

<<<<<<< HEAD
=======
  //In the end we unsubscribe from the timer interruptions
  if(timer_unsubscribe_int())
    return 1; //error
>>>>>>> 0a5f24c698faf98c8241aff8609b31dd625e802e

  return 0; //success
}
