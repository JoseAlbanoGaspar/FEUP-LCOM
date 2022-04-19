#include <lcom/lcf.h>

#include <stdint.h>

int (util_sys_int86)(reg86_t *reg86p){
  if(sys_int86(reg86p) != OK ) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return 1;
  }
  return 0;
}

