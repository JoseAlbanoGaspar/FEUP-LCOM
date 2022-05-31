// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/lab5.h>
#include <stdint.h>
#include <stdio.h>
#include <vbe.h>
#include <lcom/timer.h>
#include <i8254.h>
#include <i8042.h>
#include <keyboard.h>


unsigned int counter_global = 0;
extern uint16_t scancode;
extern int hook_id;
extern int hook_id_timer;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[])
{
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay)
{
  /* Initialize Graphics Mode */
  vg_init(mode);

  /* Waits delay */
  sleep(delay);

  /* Exit Graphics Mode */
  vg_exit();

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color)
{
  /* Initialize Graphics Mode */
  vg_init(mode);

  /* Draw Rectangle */
  vg_draw_rectangle(x, y, width, height, color);
  

  /* Wait for ESC key */
  //Here we select the bit in the hook_id needed to check if we got the right interruption
  uint32_t irq_set = BIT(hook_id);
  uint8_t aux = (uint8_t)hook_id;

  //Subscription of the interruption
  if(kbd_subscribe_int(&aux))
    return 1;

  hook_id = (int)aux;
//--------------------------------
  int ipc_status;
  message msg;
  //1 is true
  int r;

  while (scancode != ESC_KEY) {
    // Get a request message
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:

          // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set) { // subscribed interrupt
            kbc_ih();
            if (scancode != ESC_KEY) kbc_reset_scancode();
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
  kbd_unsubscribe_int();
  
  vg_exit();

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step)
{
  int ipc_status;
  message msg;
  uint32_t irq_set = BIT(1);
  uint32_t r;

  /* Initialize Graphics Mode */
  vg_init(mode);

  /* Draw Pattern */
  if (vg_draw_pattern(no_rectangles, first, step) != 0)
  {
    scancode = ESC_KEY;
  }

  /* Keyboard Subscribe With Bit_No = 1 */
  uint8_t aux = (uint8_t)hook_id;

  //Subscription of the interruption
  if(kbd_subscribe_int(&aux))
    return 1;

  hook_id = (int)aux; 
  /* Device Driver Loop Start */
  while (scancode != ESC_KEY)
  { /* You may want to use a different condition */
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
    {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status))
    { /* received notification */
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & irq_set)
        { /* subscribed interrupt */
          /* process it */
          kbc_ih();
          if (scancode != ESC_KEY) kbc_reset_scancode();
        }
        break;
      default:
        break; /* no other notifications expected: do nothing */
      }
    }
    else
    { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }
  /* Device Driver Loop End */

  /* Keyboard Unsubscribe */
  if (kbd_unsubscribe_int())
    return 1;
  /* */

  /* Exit Graphics Mode */
  vg_exit();

  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  
  vg_init(0x105); //in this exercise is asked to initialize the vram in 0x105 mode 


  vg_draw_pixmap(xpm,x,y);

  /* Wait for ESC key */
  //Here we select the bit in the hook_id needed to check if we got the right interruption
  uint32_t irq_set = BIT(hook_id);
  uint8_t aux = (uint8_t)hook_id;

  //Subscription of the interruption
  if(kbd_subscribe_int(&aux))
    return 1;

  hook_id = (int)aux;
//--------------------------------
  int ipc_status;
  message msg;
  //1 is true
  int r;

  while (scancode != ESC_KEY) {
    // Get a request message
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:

          // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set) { // subscribed interrupt
            kbc_ih();
            printf("scancode: 0x%x\n",scancode);
            if (scancode != ESC_KEY) kbc_reset_scancode();
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
  kbd_unsubscribe_int();
  
  if(vg_exit()) return 1;
  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {

  /* Wait for ESC key */
  //Here we select the bit in the hook_id needed to check if we got the right interruption
  uint32_t irq_set = BIT(hook_id);
  uint8_t aux = (uint8_t)hook_id;

  uint32_t irq_set1 = BIT(hook_id_timer);
  uint8_t aux1 = (uint8_t) hook_id_timer;

  //Subscription of the interruption
  if(kbd_subscribe_int(&aux))
    return 1;
  if(timer_subscribe_int(&aux1))
    return 1;

  hook_id = (int)aux;
  hook_id_timer = (int)aux1;
//--------------------------------
  int ipc_status;
  message msg;
  //1 is true
  int r;

  uint16_t x = xi;
  uint16_t y = yi;
  int fr_counter = 0;
  bool finished = false;
  timer_set_frequency(0, fr_rate);
  vg_init(0x105); //in this exercise is asked to initialize the vram in 0x105 mode 
  vg_draw_pixmap(xpm, x, y);

  while (scancode != ESC_KEY) {
    printf("Scancode: %x\n", scancode);
    // Get a request message
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:

          // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set) { // subscribed interrupt
            kbc_ih();
            if (scancode != ESC_KEY) kbc_reset_scancode();
          }

          if (msg.m_notify.interrupts & irq_set1) { // subscribed interrupt
            if (finished) break;
            if (x == xf && y == yf) {
              vg_erase_pixmap(xpm, x, y);
              vg_draw_pixmap(xpm, x, y);
              finished = true;
              break;
            }
            
            if (speed < 0){
              fr_counter++;
              if (abs(speed) == fr_counter){
                fr_counter = 0;
                vg_erase_pixmap(xpm, x, y);
                if(xi == xf){
                  if (yi > yf) y--;
                  else y++;
                }
                else{
                  if (xi < xf) x++;
                  else x--;
                }
                vg_draw_pixmap(xpm, x, y);
              }
            }
            else{
              vg_erase_pixmap(xpm, x, y);
              if(xi == xf){
                if (yi > yf) {
                  if (y - speed < yf) y = yf;
                  else y -= speed;
                }
                else {
                  if (y + speed > yf) y = yf;
                  else y += speed;
                }
              }
              else{
                if (xi < xf) {
                  if (x + speed > xf) x = xf;
                  else x += speed;
                }
                else {
                  if (x - speed < xf) x = xf;
                  else x -= speed;
                }
              }
              vg_draw_pixmap(xpm, x, y);
            }
            
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
  kbd_unsubscribe_int();
  timer_unsubscribe_int();
  
  if(vg_exit()) return 1;
  return 0;
}

int(video_test_controller)() {
  /* Not To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
