#include "menu.h"
#include "sprite.h"
#include "macros/constants.h"


extern int count;
extern uint16_t scancode;
extern struct packet mouse_packet; // data packet of 3 bytes
extern int mouseCount; //used to say in which byte of the mouse package we are in
extern int mouseX;
extern int mouseY;

extern uint16_t mode;

extern uint16_t h_res;
extern uint16_t v_res;

int sel[] = {0,1}; //0 -> play; 1->exit
int selected = 0;

bool game = false;

int (menu_loop)(uint32_t irq_set_keyboard, uint32_t irq_set_mouse, uint32_t irq_set_timer)
{ 
  int isSel = 0;
  init_menu();
  swapBuffer();

//--------------------------------
  int ipc_status;
  message msg;
  //1 is true
  int r;
  scancode = 0x0000;
  int ctn = 0;
  //Main loop variable
  //bool running = true;
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
          if (msg.m_notify.interrupts & irq_set_timer) { // subscribed timer interrupt
            ctn++;
          }

          // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set_keyboard) { // subscribed keyboard interrupt
            kbc_ih();

            selected = selectedOpt(scancode,selected);
            update_menu(selected);
            swapBuffer();
            //check if enter was pressed
            if(scancode == ENTER && ctn > 60){
                switch (selected)
                {
                case 0:
                    game = true;
                    scancode = ESC_KEY;
                    break;
                case 1:
                    game = false;
                    scancode = ESC_KEY;
                    break;
                default:
                    break;
                }
            }
            kbc_reset_scancode();
          }
          // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set_mouse) { // subscribed mouse interrupt
            mouse_ih();
            mouseCount++; //received another packet
            if (mouseCount == 3){ //upon receiving the 3rd byte of a mouse packet, the program should parse it and print it on the console
                mouseCount = 0;
                updateMouse();
                drawMouse();
                swapBuffer();

                if(isInOption(h_res / 2 - (OPTIONS_WIDTH / 2), 4* (v_res / 8) ,OPTIONS_WIDTH,v_res / 7 )){ //checks play option
                  if(!isSel){
                      update_menu(sel[0]);
                      selected = 0;
                      isSel = 1;
                      swapBuffer();
                  }
                  if(onPress()){
                      game = true; 
                      scancode = ESC_KEY;
                  }
                  continue;
                }
                else if(isInOption(h_res / 2 - (OPTIONS_WIDTH / 2), 6* (v_res / 8) ,OPTIONS_WIDTH,v_res / 7 )){  // checks exit option
                  if(!isSel){
                      update_menu(sel[1]);
                      selected = 1;
                      isSel = 1;
                      swapBuffer();
                  }
                  if(onPress()){
                      game = false;
                      scancode = ESC_KEY;
                  }
                  continue;
                }
                else isSel = 0; //if it ends here the mouse is selecting nothing
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
  scancode = 0x0000;
  return 0;
}


int (init_menu)(){
    uint32_t background_col=0X0,opt_color=0X0;
    if (mode == 0x115 || mode == 0x14C){
      background_col = MENU_BACKGORUND;
      opt_color = MENU_OPTION;
  }
  else if (mode == 0x110){
      background_col = MENU_BACKGORUND_110;
      opt_color = MENU_OPTION_110;
  }
  else if (mode == 0x105){   
      background_col = MENU_BACKGORUND_105;
      opt_color = MENU_OPTION_105;  
    }
  else if (mode == 0x11A){
      background_col = MENU_BACKGORUND_11A;
      opt_color = MENU_OPTION_11A;
  }

    vg_draw_rectangle(0, 0, h_res, v_res, background_col);
    //DRAW TITLE
    /*
    //S
    vg_draw_rectangle(150,50,90,20,0xff0000);
    vg_draw_rectangle(150,70,20,30,0xff0000);
    vg_draw_rectangle(150,90,90,20,0xff0000);
    vg_draw_rectangle(220,100,20,30,0xff0000);
    vg_draw_rectangle(150,130,90,20,0xff0000);
    //N
    vg_draw_rectangle(260,50,20,100,0xff0000);
    int x = 280;
    int y = 50;
    for(int i = 0; i < 4; i++){
        vg_draw_rectangle(x,y,15,25,0xff0000);
        x+=15;
        y += 25;
    }
    vg_draw_rectangle(340,50,20,100,0xff0000);
    //A
    vg_draw_rectangle(380,50,20,100,0xff0000);
    vg_draw_rectangle(400,50,20,20,0xff0000);
    vg_draw_rectangle(420,50,20,100,0xff0000);
    vg_draw_rectangle(400,100,20,20,0xff0000);
    //K
    //...*/
    //vg_draw_rectangle(h_res / 2 - (TITLE_WIDTH / 2), v_res / 8, TITLE_WIDTH, v_res / 7,opt_color);
    vg_ultimate_pixmap_handler(h_res / 2 - (TITLE_WIDTH / 2), v_res / 8,mode, TITLE);
    //vg_draw_pixmap(sprite2,50,50);
    //vg_ultimate_pixmap_handler(50,50,115,49,30);
    //filling some squares where the opitons will appear
    vg_draw_rectangle(h_res / 2 - (OPTIONS_WIDTH / 2), 4* (v_res / 8) ,OPTIONS_WIDTH,v_res / 7 ,opt_color);
    vg_draw_rectangle(h_res / 2 - (OPTIONS_WIDTH / 2), 6* (v_res / 8) ,OPTIONS_WIDTH,v_res / 7 ,background_col);
    
    //DRAW PLAY
    vg_ultimate_pixmap_handler(h_res / 2 - (OPTIONS_WIDTH / 2) + (OPTIONS_WIDTH/7), 4* (v_res / 8) + ((v_res / 7)/4), mode, PLAY);
    //DRAW EXIT
    vg_ultimate_pixmap_handler(h_res / 2 - (OPTIONS_WIDTH / 2) + (OPTIONS_WIDTH/4), 6* (v_res / 8) + ((v_res / 7)/4), mode, EXIT);
    return 0;
}

int update_menu(int sel){
  uint32_t background_col=0X0,opt_color=0X0;
    if (mode == 0x115 || mode == 0x14C){
      background_col = MENU_BACKGORUND;
      opt_color = MENU_OPTION;
  }
  else if (mode == 0x110){
      background_col = MENU_BACKGORUND_110;
      opt_color = MENU_OPTION_110;
  }
  else if (mode == 0x105){   
      background_col = MENU_BACKGORUND_105;
      opt_color = MENU_OPTION_105;  
    }
  else if (mode == 0x11A){
      background_col = MENU_BACKGORUND_11A;
      opt_color = MENU_OPTION_11A;
  }
    
    if(sel == 0){
        vg_draw_rectangle(h_res / 2 - (OPTIONS_WIDTH / 2), 4* (v_res / 8) ,OPTIONS_WIDTH,v_res / 7 ,opt_color);
        vg_draw_rectangle(h_res / 2 - (OPTIONS_WIDTH / 2), 6* (v_res / 8) ,OPTIONS_WIDTH,v_res / 7 ,background_col);
    }
    else{
        vg_draw_rectangle(h_res / 2 - (OPTIONS_WIDTH / 2), 4* (v_res / 8) ,OPTIONS_WIDTH,v_res / 7 ,background_col);
        vg_draw_rectangle(h_res / 2 - (OPTIONS_WIDTH / 2), 6* (v_res / 8) ,OPTIONS_WIDTH,v_res / 7 ,opt_color);
    }
    //DRAW PLAY
    vg_ultimate_pixmap_handler(h_res / 2 - (OPTIONS_WIDTH / 2) + (OPTIONS_WIDTH/7), 4* (v_res / 8) + ((v_res / 7)/4), mode, PLAY);
    //DRAW EXIT
    vg_ultimate_pixmap_handler(h_res / 2 - (OPTIONS_WIDTH / 2) + (OPTIONS_WIDTH/4), 6* (v_res / 8) + ((v_res / 7)/4), mode, EXIT);
    return 0;
}

int (selectedOpt)(uint16_t scan,int sel){
    switch (scan)
    {
    case UP_ARROW:
        if(sel == 1) sel = 0;
        break;
    case DOWN_ARROW:
        if(sel == 0) sel = 1;
    default:
        break;
    }
    return sel;
}


int (onPress)(){
    return mouse_packet.lb;
}

