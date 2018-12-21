#include "vesa.h"
#include "asm.h"
#include "spinlock.h"
#include "defs.h"
#include "x86.h"
#include "font.h"
#include "algorithm.h"


constexpr size_t char_lookup(char ch)
{
	return ch-0x20;
}

constexpr bool char_validpos(size_t pos)
{
	return pos>=0&&pos<CHARACTER_NUMBER-1; 
}

static vbe_mode_info modeInfo;
static inline size_t screenbuf_size(void){return modeInfo.width*modeInfo.height*(modeInfo.bpp/8);}


pixel *screen = nullptr,*screenbuf=nullptr;

spinlock screen_lock,screenbuf_lock;
bool use_default_font=false;

static inline void init_locks()
{
	initlock(&screen_lock,"screen");
	initlock(&screenbuf_lock,"scrnbuf");
}

static inline void lock_screen(color *buf)
{
	if(buf==screen)acquire(&screen_lock);
	else if(buf == screenbuf)acquire(&screenbuf_lock);
}

static inline void release_screen(color *buf)
{
	if(buf==screen)release(&screen_lock);
	else if(buf == screenbuf)release(&screenbuf_lock);
}


void init_vesa(void)
{
	modeInfo = *((vbe_mode_info*)(GRAPHIC_BUF<<4));
	screen=(pixel *)modeInfo.framebuffer;
	screenbuf=(pixel *)(modeInfo.framebuffer+screenbuf_size());

	init_locks();
}

void init_graphic(void)
{
	cprintf("vesa supported:%dx%d@%d\n",modeInfo.width,modeInfo.height,modeInfo.bpp);
	memset(screen,0,screenbuf_size());
	memset(screenbuf,0,screenbuf_size());
}

int get_mode_info(vbe_mode_info *info)
{
	if(info==nullptr)
	{
		return sizeof(vbe_mode_info);
	}

	*info=modeInfo;
	return 0;
}

void put_pixel(pixel *buf,color clr,point p)
{
	pixel *pixel=buf+p.y*modeInfo.width+p.x;
	*pixel=clr;
}

void put_rect(pixel *buf,color fill,point pos,size sz)
{
	lock_screen(buf);
	for(int i=pos.x;i<pos.x+sz.width;i++)
	{
		for(int j=pos.y;j<pos.y+sz.height;j++)
		{
			if(i>=0&&i<=modeInfo.width&&j>=0&&j<=modeInfo.height)
			{
				put_pixel(buf,fill,point(i,j));
			}
		}
	}
	release_screen(buf);
}

size_t put_char(pixel *buf,color c,point pos,char ch)
{
	int id=char_lookup(ch);
	if(!char_validpos(id))return -1;
	lock_screen(buf);
	for(int i=0;i<CHARACTER_HEIGHT;i++)
	{
		for(int j=0;j<CHARACTER_WIDTH;j++)
		{
			int x=pos.x+j,y=pos.y+i;
			if(x>=0&&x<=modeInfo.width&&y>=0&&y<=modeInfo.height)
			{
				if(chardefs[id][i][j])
				{
					put_pixel(buf,c,point(x,y));
				}
			}
		}
	}
	release_screen(buf);
	return CHARACTER_WIDTH;
}

void put_str(pixel *buf,color c,point pos,char *str)
{
	int dx=0;
	do
	{
		point np=pos;
		np.x+=dx;
		dx+=put_char(buf,c,np,*str);
	}while(*str++!='\0');
}

void put_bitmap(pixel *buf,const pixel *bitmap,point pos,size sz)
{
	lock_screen(buf);
	for(int i=0;i<sz.height;i++)
	{
		for(int j=0;j<sz.width;j++)
		{
			int x=pos.x+j,y=pos.y+i;
			if(x>=0&&x<=modeInfo.width&&y>=0&&y<=modeInfo.height)
			{
				pixel *dist=buf+y*modeInfo.width+x;
				const pixel *src =bitmap+(sz.height-i)*sz.width+j;
				*dist=*src;
			}
		}
	}
	release_screen(buf);
}

void sync_rect(pixel *dist,pixel *src,point pos,size sz)
{
	size_t max_line=min(modeInfo.width-pos.x,modeInfo.width);
	lock_screen(dist);
	for(int i=0;i<sz.height;i++)
	{
		int ny=pos.y+i;
		if(ny>=0&&ny<=modeInfo.height)
		{
			pixel *d=dist+ny*modeInfo.width+pos.x;
			pixel *s=src+ny*modeInfo.width+pos.x;
			memmove(d,s,max_line*3);
		}
	}
	release_screen(dist);
}