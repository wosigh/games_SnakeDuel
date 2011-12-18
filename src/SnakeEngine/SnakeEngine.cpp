/*
	SnakeDuel
	Copyright (C) 2011 Daniel Roggen, droggen@gmail.com

	All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include <stdio.h>
#include "SnakeEngine.h"
#include "../GfxLib/GfxLibSDL.h"

/*#define BLACK 255
#define WHITE 0
#define RED 125
#define LRED 123
#define BLUE 95
#define LBLUE 92
#define GREY 219*/

#define SPAWNRADIUS 12

int color[2]={RED,BLUE};
int colorf[2]={LRED,LBLUE};

/*Coord _w,_h; //  bitmap/true screen size
Coord _sx,_sy; // Palm window size*/
int _w,_h,_sx,_sy;

int sbh = 7; // status bar height 5+1+1
int sbspc=5; // status bar item spacing
int pnl[2]={3,3};
int pnr[2]={3,3};
int pnz[2]={3,3};
int maxlife=3;
int maxrocket=3;
int maxerazer=3;
int pstate[2]={SPAWNRADIUS,SPAWNRADIUS};
int arenax[2],arenay[2]; // arena limits inclusive (white area)

int xc[2],yc[2],xd[2],yd[2]; // snake coordinate and direction
int state=0; // game state (0=blocked, 1=run)
int score1=0,score2=0;
int xf[2],yf[2],xfd[2],yfd[2],f[2],ft[2]; // fire coordinate, direction and type
int firedist1=10;

GfxLib *snakegfxlib;

const char *key[]=
{
	"          SnakeDuel         ",
	"                            ",
	"                            ",
	"                            ",
	"      Keyboard controls     ",
	"      -----------------     ",
	"                            ",
	"            P 1         P 2 ",
	"Left         q          ret ",
	"Right       sym          p  ",
	"Fire rocket  s           l  ",
	"Fire eraser  a          bksp",
	"                            ",
	"                            ",
	"                            ",
	"                            ",
	"       Touch controls       ",
	"       --------------       ",
	"                            ",
   "    Four touch areas are    ",
   "displayed in light overlay. ",
   "                            ",
   "    The two areas on the    ",
   " player's left control the  ",
   "         direction.         ",
   "                            ",
   "    The two areas on the    ",
   " player's right control the ",
   "rockets. The inner one fires",
   "a rocket. The rightmost one ",
   "      fires an eraser.      ",
	"                            ",
	"                            ",
	"     Press space to start   ",
	"                            ",
	"                            ",
	"                            ",
	"----------------------------",
	"      by Daniel Roggen      ",
	"        v 1.0, 2011         ",
	"        BSD license         ",
	"----------------------------",
	""
};



void rotr(int *x,int *y)
{
	 int xx,yy;
	xx=-*y;
	yy=*x;
	*x=xx;
	*y=yy;
}
void rotl(int *x,int *y)
{
	 int xx,yy;
	xx=*y;
	yy=-*x;
	*x=xx;
	*y=yy;
}

void ClearScreen(int color)
{
	snakegfxlib->GfxFillRectangle(0,0,_sx,_sy,color);
}
void SnakeInit(GfxLib *gl)
{

	snakegfxlib = gl;
	//snakegfxlib->GfxInit();
	snakegfxlib->GfxGetSize(&_sx,&_sy);
	printf("%d %d\n",_sx,_sy);
	
	// arena limits, inclusive (ie white area)
	arenax[0]=1;
	arenax[1]=_sx-2;
	arenay[0]=1;
	arenay[1]=_sy-1-sbh-1-1;
	InitGame();
	InitScreenFirsttime();	
}

/*
*++
+**
*++
***+
*++
*/
void DrawSnake(int x,int y,int color)
{
   snakegfxlib->GfxPutPixel(x+1,y,color);
   snakegfxlib->GfxPutPixel(x+2,y,color);
   snakegfxlib->GfxPutPixel(x,y+1,color);
   snakegfxlib->GfxPutPixel(x+1,y+2,color);
   snakegfxlib->GfxPutPixel(x+2,y+2,color);
   snakegfxlib->GfxPutPixel(x+3,y+3,color);
   snakegfxlib->GfxPutPixel(x+1,y+4,color);
   snakegfxlib->GfxPutPixel(x+2,y+4,color);
}
/*
+
*++
***+
*++
+
*/
void DrawRocket(int x,int y,int color)
{
   snakegfxlib->GfxPutPixel(x,y,color);
   snakegfxlib->GfxPutPixel(x+1,y+1,color);
   snakegfxlib->GfxPutPixel(x+2,y+1,color);
   snakegfxlib->GfxPutPixel(x+3,y+2,color);
   snakegfxlib->GfxPutPixel(x+1,y+3,color);
   snakegfxlib->GfxPutPixel(x+2,y+3,color);
   snakegfxlib->GfxPutPixel(x,y+4,color);
}
/*
+
+*+
*
+*+
+
*/
void DrawEraser(int x,int y,int color)
{
	snakegfxlib->GfxPutPixel(x+1,y,color);
	snakegfxlib->GfxPutPixel(x+2,y,color);
	snakegfxlib->GfxPutPixel(x+1,y+1,color);
	snakegfxlib->GfxPutPixel(x+2,y+1,color);
	snakegfxlib->GfxPutPixel(x+1,y+3,color);
	snakegfxlib->GfxPutPixel(x+2,y+3,color);
	snakegfxlib->GfxPutPixel(x+1,y+4,color);
	snakegfxlib->GfxPutPixel(x+2,y+4,color);
}
void DrawStatusBar()
{
	int i;
	int p;
	int top;
	int offset;
	top=_sy-sbh;
	offset=2;
	for(p=0;p<2;p++)
	{
		for(i=0;i<pnl[p];i++)
			DrawSnake(offset+i*sbspc,top,BLACK);
		for(i=pnl[p];i<maxlife;i++)
			DrawSnake(offset+i*sbspc,top,WHITE);
		offset+=(maxlife+1)*sbspc;
		for(i=0;i<pnr[p];i++)
			DrawRocket(offset+i*sbspc,top,BLACK);
		for(i=pnr[p];i<maxrocket;i++)
			DrawRocket(offset+i*sbspc,top,WHITE);
		offset+=(maxrocket+1)*sbspc;
		for(i=0;i<pnz[p];i++)
			DrawEraser(offset+i*sbspc,top,BLACK);
		for(i=pnz[p];i<maxerazer;i++)
			DrawEraser(offset+i*sbspc,top,WHITE);
		offset=_sx/2+2;
	}
}
void InitScreenFirsttime()
{
	InitScreen();

	// Display how to handle the game
	int i=0;
	while(key[i][0])
	{
	//for(i=0;i<
	//{
		snakegfxlib->GfxString((snakegfxlib->GetSx()-strlen(key[0])*8)>>1,10+i*8,key[i],LBLUE);
		i++;
	}


	//}
}
void InitScreen()
{
	ClearScreen(WHITE);
	snakegfxlib->GfxLine(0,0,_sx-1,0,BLACK);
	snakegfxlib->GfxLine(_sx-1,0,_sx-1,_sy-1,BLACK);
	snakegfxlib->GfxLine(_sx-1,_sy-1,0,_sy-1,BLACK);
	snakegfxlib->GfxLine(0,_sy-1,0,0,BLACK);
	// score/status bar
	snakegfxlib->GfxLine(0,_sy-1-sbh-1,_sx-1,_sy-1-sbh-1,BLACK);
	snakegfxlib->GfxLine(_sx/2,_sy-sbh-2,_sx/2,_sy-1,BLACK);
	DrawStatusBar();

	#ifdef TOUCHCONTROL
		// Draw control area
		// Vert
		snakegfxlib->GfxLine(_sx/CONTROLSIZEDIVIDER,1,_sx/CONTROLSIZEDIVIDER,_sy-1-sbh-1-1,LGREY);
		snakegfxlib->GfxLine(_sx*(CONTROLSIZEDIVIDER-1)/CONTROLSIZEDIVIDER,1,_sx*(CONTROLSIZEDIVIDER-1)/CONTROLSIZEDIVIDER,_sy-1-sbh-1-1,LGREY);
		// Hor
		snakegfxlib->GfxLine(1,_sy/4,_sx/CONTROLSIZEDIVIDER,_sy/4,LGREY);
		snakegfxlib->GfxLine(1,_sy*2/4,_sx/CONTROLSIZEDIVIDER,_sy*2/4,LGREY);
		snakegfxlib->GfxLine(1,_sy*3/4,_sx/CONTROLSIZEDIVIDER,_sy*3/4,LGREY);
		snakegfxlib->GfxLine(_sx*(CONTROLSIZEDIVIDER-1)/CONTROLSIZEDIVIDER,_sy/4,_sx-1-1,_sy/4,LGREY);
		snakegfxlib->GfxLine(_sx*(CONTROLSIZEDIVIDER-1)/CONTROLSIZEDIVIDER,_sy*2/4,_sx-1-1,_sy*2/4,LGREY);
		snakegfxlib->GfxLine(_sx*(CONTROLSIZEDIVIDER-1)/CONTROLSIZEDIVIDER,_sy*3/4,_sx-1-1,_sy*3/4,LGREY);
	#endif

}



void InitSnake(int player)
{
	xc[player]=_sx*2/6*(1-player)+_sx*4/6*player;
	yc[player]=_sy/2;
	xd[player]=0;
	yd[player]=-1;
	pstate[player]=SPAWNRADIUS;
}
void InitGame()
{
	int i;
	state=0;
	InitSnake(0);
	InitSnake(1);
	
	for(i=0;i<2;i++)
	{
		pnl[i]=maxlife;
		pnr[i]=3;
		pnz[i]=3;
		f[i]=0;
	}
}

void fire(int player,int firetype)
{
 // check if rocket/eraser available
 if((firetype==1 & pnr[player]==0) || (firetype==0 & pnz[player]==0))
  return;
  if(f[player]==0)
  {
    f[player]=1;
    ft[player]=firetype;
    if(firetype==0)
     pnz[player]--;
    else
     pnr[player]--;
    DrawStatusBar();
    xf[player]=xc[player]+firedist1*xd[player];
    yf[player]=yc[player]+firedist1*yd[player];
    xfd[player]=xd[player];
    yfd[player]=yd[player];
  }
}
void PlotFireUpdate()
{
 int i;
 for(i=0;i<2;i++)
  {
  if(f[i]==1)
  {
    // check range
    if(xf[i]>arenax[1] || xf[i]<arenax[0] || yf[i]> arenay[1] || yf[i]<arenay[0] )
    {
      f[i]=0;
      break;
    }
    if(ft[i]==1)
     	snakegfxlib->GfxPutPixel(xf[i],yf[i],colorf[i]);
    else
     	snakegfxlib->GfxPutPixel(xf[i],yf[i],WHITE);
    xf[i]=xf[i]+xfd[i];
    yf[i]=yf[i]+yfd[i];  
  }
 }
}

void DrawDead(int p)
{
   int w=(arenax[1]-arenax[0]+1)/2;
   snakegfxlib->GfxLine(arenax[0]+w*p,arenay[0],(arenax[0]+w)*(1-p)+arenax[1]*p,arenay[1],BLACK);
   snakegfxlib->GfxLine(arenax[0]+w*p,arenay[1],(arenax[0]+w)*(1-p)+arenax[1]*p,arenay[0],BLACK);
}
void SnakeUpdate()
{
//	char s[128];		
	int cx,cy;
	int player;
	int p[2];

	if(state!=1)
		return;

	// get snake state first
	// bug: snake at 90 deg synchr 
	p[0]=snakegfxlib->GfxGetPixel(xc[0],yc[0]);
	p[1]=snakegfxlib->GfxGetPixel(xc[1],yc[1]);

	for(player=0;player<2;player++)
	{
		if(pstate[player])
		{
			// snake spawn
			pstate[player]--;
			cx=_sx*2/6*(1-player)+_sx*4/6*player;
			cy=_sy/2;
			snakegfxlib->GfxFillRectangle(
				cx-pstate[player]-1,
				cy-pstate[player]-1,
				pstate[player]*2+3,
				pstate[player]*2+3,
				WHITE);
			if(pstate[player])
			snakegfxlib->GfxFillRectangle(
				cx-pstate[player],
				cy-pstate[player],
				pstate[player]*2+1,
				pstate[player]*2+1,
				color[player]);
		}
		else
		{
		 	// snake move
		 	snakegfxlib->GfxPutPixel(xc[player],yc[player],color[player]);
			xc[player]+=xd[player];
			yc[player]+=yd[player];
			
		//	char s[128];
			//printf("%X: %X\n",p,p[player]);
//			snakegfxlib->GfxFillRectangle(10,10,60,10,LRED);
	//		stringRGBA(snakegfxlib->GetSurface(),10,10, s, 255,127,30,255);
			
			#ifdef TOUCHCONTROL
				if(p[player]!=WHITE && p[player]!=LGREY)     // Allow light gray...
			#else		
				if(p[player]!=WHITE)
			#endif
			{
				if(pnl[player]) 
				{
					pnl[player]--;
					DrawStatusBar();
					InitSnake(player);
				}
			}
		}
	}
	PlotFireUpdate();
	PlotFireUpdate();
	
	if(pnl[0] && pnl[1])
	return;
	  
	  
	
	if(pnl[0]==0)
	{
		// pprintf("xPlayer 1 dead!");
		score2++;
		DrawDead(0);
	}
	if(pnl[1]==0)
	{			 
		// pprintf("xPlayer 2 dead!");
		score1++;
		DrawDead(1);
	}
	
	   
	/*
	StrPrintF(s,"%d - %d pnl %d %d",score1,score2,pnl[0],pnl[1]);
	pprintf(s);
	*/
	
	InitGame();
	state=0;
}
void SnakeKeyStart()
{
 if(state==0)
		   {
		      state=1;
			    InitScreen();	
			 }
}
void SnakeKeyl(int p)
{
   if(state!=1)
      return;
   rotl(&xd[p],&yd[p]);
}
void SnakeKeyr(int p)
{
   if(state!=1)
      return;
   rotr(&xd[p],&yd[p]);
}
void SnakeKeyfire(int player,int firetype)
{
   if(state!=1)
      return;
   fire(player,firetype);
}



