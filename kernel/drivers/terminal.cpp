#include "terminal.h"
#include "vesa.h"
#include "spinlock.h"
#include "font_info.h"
#include "console.h"

constexpr auto MAX_CHARS = 256;
constexpr auto main_freground = color(255,255,255);
constexpr auto main_background = color(20,20,200);
constexpr auto bar_background=color(200,200,200);
constexpr auto bar_foreground=color(0,0,0);
constexpr auto bar_height=32;

spinlock chars_lock;
size_t scheight=0,scwidth=0,
	winheight=0,winwidth=0,
	rowcnt=0,columncnt=0;

static inline void terminal_pageup(void);
static inline void bar_show(char *str);
static inline void terminal_putchar(char c);

extern bool _is_terminal_ready=false;

struct cursor
{
	int x,y;
	bool next(void)
	{
		x++;
		if(x>=rowcnt)return new_line();
		else return false;
	}

	bool new_line(void)
	{
		x=0;y++;
		if(y>=columncnt)
		{
			y=columncnt-1;
			return true;
		}
		return false;
	}
}cur;

struct chblock
{
	char ch;
	color clr;
	int r,c;
	chblock(void):r(0),c(0)
	{
		clr=main_freground;
		ch='\0';
	}

	chblock(int row,int column):r(row),c(column)
	{
		clr=main_freground;
		ch='\0';
	}

	void show()
	{
		auto pos=point(r*CHARACTER_WIDTH,c*CHARACTER_HEIGHT);
		put_rect(screen,main_background,pos,size(CHARACTER_WIDTH,CHARACTER_HEIGHT));
		put_char(screen,clr,pos,ch);
	}
}chbuf[MAX_CHARS][MAX_CHARS];


void init_terminal(void)
{
	vbe_mode_info info;
	get_mode_info(&info);
	scheight=info.height;
	scwidth=info.width;

	winheight=scheight-bar_height;
	winwidth=scwidth;

	rowcnt=winwidth/CHARACTER_WIDTH;
	columncnt=winheight/CHARACTER_HEIGHT;

	initlock(&chars_lock,"chars");

	for(int i=0;i<rowcnt;i++)
	{
		for(int j=0;j<columncnt;j++)
		{
			chbuf[i][j]=chblock(i,j);
		}
	}

	cprintf("client area:%dx%d\n",winwidth,winheight);
	cprintf("terminal:%dx%d words\n",rowcnt,columncnt);
	
	put_rect(screenbuf,main_background,point(0,0),size(winwidth,winheight)); //main
	bar_show("Rexv6 CLI terminal");
	sync_rect(screen,screenbuf,point(0,0),size(scwidth,scheight));

	cprintf2("_is_terminal_ready=%d\n",_is_terminal_ready);
	_is_terminal_ready=true;
	cprintf2("_is_terminal_ready=%d\n",_is_terminal_ready);
	for(int i=1;i<=10000;i++)
	{
		//cprintf2("fuck %dtime(s)\n",10040-i);
	}
}

size_t bar_strlen=0;
static inline void bar_show(char *str)
{
	put_rect(screenbuf,bar_background,point(0,winheight),(!bar_strlen)?size(winwidth,bar_height):size(bar_strlen*CHARACTER_WIDTH,bar_height)); 
	put_str(screenbuf,bar_foreground,point(5,winheight+5),str);
	bar_strlen=strlen(str);
}

static inline void terminal_pageup(void)
{
	for(int i=1;i<columncnt;i++)
	{
		for(int j=0;j<rowcnt;j++)
		{
			chbuf[j][i-1].ch=chbuf[j][i].ch;
			chbuf[j][i-1].show();
		}
	}
}

static inline void terminal_putchar(char c)
{
	if(c=='\n')
	{
		if(cur.new_line())terminal_pageup();
	}
	else
	{
		chbuf[cur.x][cur.y].ch=c;
		chbuf[cur.x][cur.y].show();
	}
	if(cur.next())terminal_pageup();
}


void terminal_putc(char c)
{
	terminal_putchar(c);
}
