#ifndef _VESA_H_
#define _VESA_H_
#include "defs.h"

struct vbe_mode_info
{
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

struct color
{
	uint8_t b,g,r;
	#ifdef __cplusplus
	constexpr 
	#endif
	color():r(0),g(0),b(0){};
	#ifdef __cplusplus
	constexpr 
	#endif
	color(uint8_t R,uint8_t G,uint8_t B):r(R),g(G),b(B){}
}__attribute__ ((packed));

struct fontstyle
{
	bool itatic,bold,underline,delete_line;
	size_t fntsize;
};

struct point
{
	int x,y;

	#ifdef __cplusplus
	constexpr 
	#endif
	point():x(0),y(0){}

	#ifdef __cplusplus
	constexpr 
	#endif
	point(int _x,int _y):x(_x),y(_y){}
};

struct size
{
	size_t width,height;

	#ifdef __cplusplus
	constexpr 
	#endif
	size():width(0),height(0){}

	#ifdef __cplusplus
	constexpr 
	#endif
	size(size_t _x,size_t _y):width(_x),height(_y){}
};


using pixel=color;

extern pixel *screen;
extern pixel *screenbuf;

void init_vesa(void);
void init_graphic(void);
int get_mode_info(vbe_mode_info *info);

void put_pixel(pixel *buf,color clr,point pos);
void put_rect(pixel *buf,color fill,point pos,size sz);
size_t put_char(pixel *buf,color c,point pos,char ch);
void put_str(pixel *buf,color c,point pos,char *str);
void put_bitmap(pixel *buf,const pixel *bitmap,point pos,size sz);

void sync_rect(pixel *dist,pixel *src,point pos,size sz);

#endif
