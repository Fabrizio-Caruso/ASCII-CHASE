/*

push bp
    mov bp, sp
    push es
    xor ax, ax
    mov al, [bp+8] ; y
    mov di, ax
    shl ax, 1
    shl ax, 1
    add ax, di ; ax = y*5
    mov cl, 6
    shl ax, cl ; ax = y*320
    xor ax, ax
    mov al, [bp+10]  ; ax = x
    add ax, di ; ax = y*320+x
    mov cl, 3
    shl ax, cl ; ax = (y*8)*320 + x*8
    mov di, ax
    mov si, [bp+2] ; tile base pointer
    xor ax, ax
    mov al, [bp+6]
    shl ax, cl ; cl was still 3, ax = idx*8 - 8 bytes per tile
    mov si, ax
    mov ax, 0a000h
    push ax
    pop es
    xor ax, ax
    mov al, [bp+4]
    mov cx, 8
  outer_loop:
    push cx
    mov cx, 8
    mov bl, [si]
  inner_loop:
    shl bl, 1
    jc write_label
    mov [es:di], ah
    inc di
    loop loop_label
    jmp out_inner_loop
  write_label:
    stosb
    loop loop_label
  out_inner_loop:
    pop cx
    add di, 312
    inc si
    loop outer_loop
    pop es
    pop bp
    ret
    
*/

/* Entry points
$F3D8 - HGR2
$F3E2 - HGR
$F605 - XDRAW
$F661 - DRAW
$F6E9 - HCOLOR
$F6FD - HPLOT
$F721 - ROT
$F727 - SCALE 
*/


#include "standard_libs.h"

/* provides base address for page2 hires scanlines  */
/*
unsigned HB2[]={
0x4000, 0x4400, 0x4800, 0x4C00, 0x5000, 0x5400, 0x5800, 0x5C00,
0x4080, 0x4480, 0x4880, 0x4C80, 0x5080, 0x5480, 0x5880, 0x5C80,
0x4100, 0x4500, 0x4900, 0x4D00, 0x5100, 0x5500, 0x5900, 0x5D00,
0x4180, 0x4580, 0x4980, 0x4D80, 0x5180, 0x5580, 0x5980, 0x5D80,
0x4200, 0x4600, 0x4A00, 0x4E00, 0x5200, 0x5600, 0x5A00, 0x5E00,
0x4280, 0x4680, 0x4A80, 0x4E80, 0x5280, 0x5680, 0x5A80, 0x5E80,
0x4300, 0x4700, 0x4B00, 0x4F00, 0x5300, 0x5700, 0x5B00, 0x5F00,
0x4380, 0x4780, 0x4B80, 0x4F80, 0x5380, 0x5780, 0x5B80, 0x5F80,
0x4028, 0x4428, 0x4828, 0x4C28, 0x5028, 0x5428, 0x5828, 0x5C28,
0x40A8, 0x44A8, 0x48A8, 0x4CA8, 0x50A8, 0x54A8, 0x58A8, 0x5CA8,
0x4128, 0x4528, 0x4928, 0x4D28, 0x5128, 0x5528, 0x5928, 0x5D28,
0x41A8, 0x45A8, 0x49A8, 0x4DA8, 0x51A8, 0x55A8, 0x59A8, 0x5DA8,
0x4228, 0x4628, 0x4A28, 0x4E28, 0x5228, 0x5628, 0x5A28, 0x5E28,
0x42A8, 0x46A8, 0x4AA8, 0x4EA8, 0x52A8, 0x56A8, 0x5AA8, 0x5EA8,
0x4328, 0x4728, 0x4B28, 0x4F28, 0x5328, 0x5728, 0x5B28, 0x5F28,
0x43A8, 0x47A8, 0x4BA8, 0x4FA8, 0x53A8, 0x57A8, 0x5BA8, 0x5FA8,
0x4050, 0x4450, 0x4850, 0x4C50, 0x5050, 0x5450, 0x5850, 0x5C50,
0x40D0, 0x44D0, 0x48D0, 0x4CD0, 0x50D0, 0x54D0, 0x58D0, 0x5CD0,
0x4150, 0x4550, 0x4950, 0x4D50, 0x5150, 0x5550, 0x5950, 0x5D50,
0x41D0, 0x45D0, 0x49D0, 0x4DD0, 0x51D0, 0x55D0, 0x59D0, 0x5DD0,
0x4250, 0x4650, 0x4A50, 0x4E50, 0x5250, 0x5650, 0x5A50, 0x5E50,
0x42D0, 0x46D0, 0x4AD0, 0x4ED0, 0x52D0, 0x56D0, 0x5AD0, 0x5ED0,
0x4350, 0x4750, 0x4B50, 0x4F50, 0x5350, 0x5750, 0x5B50, 0x5F50,
0x43D0, 0x47D0, 0x4BD0, 0x4FD0, 0x53D0, 0x57D0, 0x5BD0, 0x5FD0};
*/

uint16_t HB1[]={
0x2000, 0x2400, 0x2800, 0x2C00, 0x3000, 0x3400, 0x3800, 0x3C00,
0x2080, 0x2480, 0x2880, 0x2C80, 0x3080, 0x3480, 0x3880, 0x3C80,
0x2100, 0x2500, 0x2900, 0x2D00, 0x3100, 0x3500, 0x3900, 0x3D00,
0x2180, 0x2580, 0x2980, 0x2D80, 0x3180, 0x3580, 0x3980, 0x3D80,
0x2200, 0x2600, 0x2A00, 0x2E00, 0x3200, 0x3600, 0x3A00, 0x3E00,
0x2280, 0x2680, 0x2A80, 0x2E80, 0x3280, 0x3680, 0x3A80, 0x3E80,
0x2300, 0x2700, 0x2B00, 0x2F00, 0x3300, 0x3700, 0x3B00, 0x3F00,
0x2380, 0x2780, 0x2B80, 0x2F80, 0x3380, 0x3780, 0x3B80, 0x3F80,
0x2028, 0x2428, 0x2828, 0x2C28, 0x3028, 0x3428, 0x3828, 0x3C28,
0x20A8, 0x24A8, 0x28A8, 0x2CA8, 0x30A8, 0x34A8, 0x38A8, 0x3CA8,
0x2128, 0x2528, 0x2928, 0x2D28, 0x3128, 0x3528, 0x3928, 0x3D28,
0x21A8, 0x25A8, 0x29A8, 0x2DA8, 0x31A8, 0x35A8, 0x39A8, 0x3DA8,
0x2228, 0x2628, 0x2A28, 0x2E28, 0x3228, 0x3628, 0x3A28, 0x3E28,
0x22A8, 0x26A8, 0x2AA8, 0x2EA8, 0x32A8, 0x36A8, 0x3AA8, 0x3EA8,
0x2328, 0x2728, 0x2B28, 0x2F28, 0x3328, 0x3728, 0x3B28, 0x3F28,
0x23A8, 0x27A8, 0x2BA8, 0x2FA8, 0x33A8, 0x37A8, 0x3BA8, 0x3FA8,
0x2050, 0x2450, 0x2850, 0x2C50, 0x3050, 0x3450, 0x3850, 0x3C50,
0x20D0, 0x24D0, 0x28D0, 0x2CD0, 0x30D0, 0x34D0, 0x38D0, 0x3CD0,
0x2150, 0x2550, 0x2950, 0x2D50, 0x3150, 0x3550, 0x3950, 0x3D50,
0x21D0, 0x25D0, 0x29D0, 0x2DD0, 0x31D0, 0x35D0, 0x39D0, 0x3DD0,
0x2250, 0x2650, 0x2A50, 0x2E50, 0x3250, 0x3650, 0x3A50, 0x3E50,
0x22D0, 0x26D0, 0x2AD0, 0x2ED0, 0x32D0, 0x36D0, 0x3AD0, 0x3ED0,
0x2350, 0x2750, 0x2B50, 0x2F50, 0x3350, 0x3750, 0x3B50, 0x3F50,
0x23D0, 0x27D0, 0x2BD0, 0x2FD0, 0x33D0, 0x37D0, 0x3BD0, 0x3FD0};

#include <conio.h>
#include <peekpoke.h>


void _XL_INIT_GRAPHICS(void)
{
    // HIGH RES ON
    POKE(0xC057,0);

    // HGR PAGE 2
    // POKE(0xC055,0);

    // HGR PAGE 1
    POKE(0xC054,0);
 
    // TEXT OFF
    POKE(0xC050,0);
    
    // MIXED OFF
    POKE(0xC052,0);
    
    {
        uint8_t i;
        uint8_t j;
        uint8_t k;
        
        for(i=0;i<192;i+=8)
        {
            for(j=0;j<20;++j)
            {

                for(k=0;k<8;++k)
                {
                    POKE(HB1[i+k]+2*j,255-3-64);
                    POKE(HB1[i+k]+2*j+1,255-64-32-1);
                }
                cgetc();
            }
            cgetc();
        }
    
        // while(1){}
    }
}
