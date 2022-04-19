#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  val = (0x00FF & val);
  *lsb = (uint8_t) val; //lsb fica com os 8 bits menos significativos de val
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  /* To be implemented by the students */
  val = (val & 0xFF00);
  *msb = val >> 8;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t value2; //temporary value for the return of the sys_inb funtion
  if (sys_inb(port, &value2) == OK) {
    *value = value2 & 255/*ou & 0xFF*/; //*value = (uint8_t)value2;
    return 0; //success
  }
  printf("Error in util_sys_inb\n");
  return 1; //error
}
