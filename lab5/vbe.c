#include <vbe.h>


int (vg_get_mode_info)(uint16_t mode, vbe_mode_info_t* info){
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86)); // Zero the structure
    reg86.ax = 0x4F01;                // VBE call, function 02: set VBE mode
    reg86.cx = mode;   
    reg86.ebp = *info;
    reg86.intno = 0x10;
    if (sys_int86(&reg86) != OK){
        printf("vbe_mode_info: sys_int86() failed \n");
        return 1;
    }

    return 0;
}

void *(vg_init)(uint16_t mode)
{
    /* */
    mr.mr_base = 0;
    mr.mr_limit = 1048576;
    if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    vbe_mode_info_t info;
    vg_get_mode_info(mode, &info);
    phys_addr = (phys_bytes)info.PhysBasePtr;
    h_res = info.XResolution;
    v_res = info.YResolution;
    bitsPerPixel = info.BitsPerPixel;
    bytesPerPixel = bitsPerPixel / 8;
    if (bitsPerPixel % 8 != 0)
        bytesPerPixel++;
    red_s = info.RedMaskSize;
    red_p = info.RedFieldPosition;
    green_s = info.GreenMaskSize;
    green_p = info.GreenFieldPosition;
    blue_s = info.BlueMaskSize;
    blue_p = info.BlueFieldPosition;
    memoryModel = info.MemoryModel;
    /* */

    /* Allow memory mapping */
    mr.mr_base = phys_addr;
    mr.mr_limit = phys_addr + (h_res * v_res * bytesPerPixel);
    if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    /* */

    /* Map memory */
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, (h_res * v_res * bytesPerPixel));
    if (video_mem == MAP_FAILED)
        panic("couldn’t map video memory");
    /* */

    /* Set VBE graphics mode */
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86)); // Zero the structure
    reg86.ax = 0x4F02;                // VBE call, function 02: set VBE mode
    reg86.bx = 1 << 14 | mode;        // Set bit 14: linear framebuffer
    reg86.intno = 0x10;
    if (sys_int86(&reg86) != OK)
    {
        printf("set_vbe_mode: sys_int86() failed \n");
        return NULL;
    }
    /* */

    return video_mem;
}
uint32_t getmask(int k){
  uint32_t mask = 0xFF << 8*k;
  return mask; 
}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color)
{
    if (x < h_res && y < v_res)
    {
      char* init_address = (char*) video_mem + (y * h_res + x)* bytesPerPixel;
      
      for(int k = 0; k < bytesPerPixel; k++){ 
       init_address[k] = (color & getmask(k)) >> (8*k);
      }
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
    for (unsigned int i = y; i < y + height; i++)
    {
        vg_draw_hline(x, i, width, color);
    }
    return 0;
}

int(vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step)
{
    uint32_t color_temp/*,red,green,blue;
    uint32_t initial_blue = (first & getmask(1)) >> 8;
    uint32_t initial_green = (first & getmask(2)) >> 16;
    uint32_t initial_red = (first & getmask(3)) >> 24*/;
    for (unsigned int col = 0; col < no_rectangles; col++) // Col
    {
        for (unsigned int row = 0; row < no_rectangles; row++) // Row
        {
            if (memoryModel != 0x06) // Indexed Color Model
            {
                color_temp = (first + (row * no_rectangles + col) * step) % (1 << bitsPerPixel);
            }
            else // Direct Color Model
            {
                color_temp = ((uint8_t) first + (col + row) * step) % (1 << blue_s);
                color_temp += (((uint8_t)(first >> blue_s) + row * step) % (1 << (green_s)) << blue_s);
                color_temp += (((uint8_t)(first >> (blue_s + green_s)) + col * step) % (1 << (red_s)) << (blue_s + green_s));
            }
            printf("COLOR : %x\n",color_temp);
            vg_draw_rectangle(col * (h_res / no_rectangles), row * (v_res / no_rectangles), (h_res / no_rectangles), (v_res / no_rectangles), color_temp);
        }
    }
    return 0;
}

int(vg_draw_pixmap)(xpm_map_t xpm, uint16_t x, uint16_t y)
{
    // Can be XPM_INDEXED, XPM_1_5_5_5, XPM_5_6_5, XPM_8_8_8 or XPM_8_8_8_8
    enum xpm_image_type type = XPM_INDEXED;
    xpm_image_t img;
    uint8_t *map = xpm_load(xpm, type, &img);
    if(map == NULL) {
        printf("ERROR::xpm_load failed!\n");
        return 1;
    }
    /* Print Sprite */
    for (unsigned int i = 0; i < img.height; i++)
    {
        for (unsigned int j = 0; j < img.width; j++)
        {
            vg_draw_pixel(x + j, y + i, *(map + img.width * i + j));
        }
    }
    /* */
    return 0;
}

void(vg_erase_pixmap)(xpm_map_t xpm, uint16_t x, uint16_t y)
{
    if (x < h_res && y < v_res)
    {
        // Can be XPM_INDEXED, XPM_1_5_5_5, XPM_5_6_5, XPM_8_8_8 or XPM_8_8_8_8
        enum xpm_image_type type = XPM_INDEXED;
        xpm_image_t img;
        uint8_t *sprite = xpm_load(xpm, type, &img);

        /*Print Sprite */
        for (unsigned int i = 0; i < img.height; i++)
        {
            for (unsigned int j = 0; j < img.width; j++)
            {
                vg_draw_pixel(j + x, i + y, 0 * (*sprite + 1));
            }
        }
    }
}

uint8_t *(vg_create_sprite)(xpm_map_t xpm, xpm_image_t *img)
{
    // Can be XPM_INDEXED, XPM_1_5_5_5, XPM_5_6_5, XPM_8_8_8 or XPM_8_8_8_8
    enum xpm_image_type type = XPM_INDEXED;
    uint8_t *sprite = xpm_load(xpm, type, img);
    return sprite;
}

int(vg_draw_sprite)(uint8_t *sprite, xpm_image_t img, uint16_t x, uint16_t y)
{
    for (unsigned int i = 0; i < img.height; i++)
    {
        for (unsigned int j = 0; j < img.width; j++)
        {
            vg_draw_pixel(j + x, i + y, *(sprite + i * img.width + j));
        }
    }
    return 0;
}

int(vg_erase_sprite)(uint8_t *sprite, xpm_image_t img, uint16_t x, uint16_t y)
{
    for (unsigned int i = 0; i < img.height; i++)
    {
        for (unsigned int j = 0; j < img.width; j++)
        {
            vg_draw_pixel(j + x, i + y, 0);
        }
    }
    return 0;
}
