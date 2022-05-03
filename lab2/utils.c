#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
<<<<<<< HEAD
<<<<<<< HEAD
  *lsb = (uint8_t) (val & 0x00FF);
=======
  val = (0x00FF & val);
  *lsb = (uint8_t) val; //lsb fica com os 8 bits menos significativos de val
>>>>>>> 0a5f24c698faf98c8241aff8609b31dd625e802e
=======
  val = (0x00FF & val);
  *lsb = (uint8_t) val;
>>>>>>> e859c9b01cc41cb400a0531ff5fc07eae080891b
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
<<<<<<< HEAD
<<<<<<< HEAD
  *msb = (uint8_t) (val >> 8) & 0x00FF;
=======
  /* To be implemented by the students */
  val = (val & 0xFF00);
  *msb = val >> 8;
>>>>>>> 0a5f24c698faf98c8241aff8609b31dd625e802e
=======
  val = (val & 0xFF00);
  *msb = val >> 8;
>>>>>>> e859c9b01cc41cb400a0531ff5fc07eae080891b
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
<<<<<<< HEAD
<<<<<<< HEAD
  uint32_t value2;
  if (sys_inb(port, &value2) != OK) return 1;
  *value = value2 & 255; 
  return 0;
=======
  uint32_t value2; //temporary value for the return of the sys_inb funtion
  if (sys_inb(port, &value2) == OK) {
    *value = value2 & 255/*ou & 0xFF*/; //*value = (uint8_t)value2;
    return 0; //success
  }
  printf("Error in util_sys_inb\n");
  return 1; //error
>>>>>>> 0a5f24c698faf98c8241aff8609b31dd625e802e
=======
  uint32_t value2;
  if (sys_inb(port, &value2) == OK)
  {
    *value = value2 & 255;
    return 0;
  }
  return 1;
>>>>>>> e859c9b01cc41cb400a0531ff5fc07eae080891b
}
