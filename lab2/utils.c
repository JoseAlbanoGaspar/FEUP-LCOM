#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t) (val & 0x00FF);
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (uint8_t) (val >> 8) & 0x00FF;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t value2;
  if (sys_inb(port, &value2) != OK) return 1;
  *value = value2 & 255; 
  return 0;
}
