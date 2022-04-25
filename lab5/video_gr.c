#include <lcom/lcf.h>
#include <lcom/video_gr.h>

extern char *video_mem;
extern unsigned h_res;
extern unsigned v_res;
extern unsigned bits_per_pixel;
extern unsigned bytes_per_pixel;

void *(vg_init)(uint16_t mode)
{
    printf("USING OUR INIT VERSION!\n");
  struct minix_mem_range mr;
  unsigned int vram_base;  
  unsigned int vram_size;  
              
  //mmap_t map;
  vbe_mode_info_t info;
  //lm_alloc(sizeof(vbe_mode_info_t), &map);
  int r;

  vbe_get_mode_info(mode, &info);
  vram_base = info.PhysBasePtr;
  vram_size = info.WinSize;
  h_res = info.XResolution;
  v_res = info.YResolution;
  bits_per_pixel = info.BitsPerPixel;
  bytes_per_pixel = bits_per_pixel / 8;
  if(bits_per_pixel % 8 != 0)
    bytes_per_pixel++;
  //Use VBE function 0x01 to initialize vram_base and vram_size 

  // Allow memory mapping 

  mr.mr_base = (phys_bytes) vram_base;	
  mr.mr_limit = mr.mr_base + vram_size;  

  if(OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  // Map memory 

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  reg86_t reg86;
  memset(&reg86, 0, sizeof(reg86));
  reg86.intno = 0x10;
  reg86.ah = 0x4F;   
  reg86.al = 0x02;    
  reg86.bx = 1<<14 | mode;
  
  if (sys_int86(&reg86) != OK) return NULL;

  return video_mem;
}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color)
{
    if (x < h_res && y < v_res)
    {   
        color = color & (255 * bytes_per_pixel); //not sure if this works!!!
        /*printf("num_bits :%x\n",bits_per_pixel);
        printf("num_bytes :%x\n",bytes_per_pixel);
        printf("x :%x\n",x);
        printf("y :%x\n",y);
        printf("color :%x\n",color);
        printf("h_Res :%x\n",h_res);
        printf("v_res :%x\n",v_res);*/
        *(video_mem + (y * h_res + x) * bytes_per_pixel) = color;
        return 0;
    }
    
    return 1;
}

int(vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color)
{
    for (unsigned int i = y; i < (y + len); i++)
    {
        vg_draw_pixel(x, i, color);
    }
    return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color)
{
    for (unsigned int i = x; i < (x + len); i++)
    {
        vg_draw_pixel(i, y, color);
    }   
    return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    for (unsigned int i = y; i < (y + height); i++)
    {
        vg_draw_hline(x, i, width, color);
    }
    return 0;
}
