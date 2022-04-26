#ifndef __VBE_H
#define __VBE_H

#include <lcom/lcf.h>

int r;
struct minix_mem_range mr; /* Physical memory range */
unsigned int vram_base;    /* VRAM's physical addresss */
unsigned int vram_size;    /* VRAM's size, but you can use the frame-buffer size, instead */
uint8_t *video_mem;        /* Frame-buffer VM address */
uint16_t h_res;            /* Horizontal resolution in pixels */
uint16_t v_res;            /* Vertical resolution in pixels */
uint16_t bits_per_pixel;
uint16_t bytes_per_pixel;
uint8_t red_s;
uint8_t red_p;
uint8_t green_s;
uint8_t green_p;
uint8_t blue_s;
uint8_t blue_p;
uint8_t memoryModel;
phys_bytes phys_addr;


void *(vg_init)(uint16_t mode);
int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int(vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);


#endif /* __VBE_H */
