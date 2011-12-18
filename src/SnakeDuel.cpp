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
#include <math.h>
//#include <GLES2/gl2.h>
#include "SDL.h"
#include "PDL.h"

#include "GfxLib/GfxLibSDL.h"
#include "SnakeEngine/SnakeEngine.h"
#include "SDL/SDL_gfxPrimitives.h"


#ifdef WIN32
extern "C" 
#endif
	
extern "C" {
void ExitFunc()
{
	printf("atexit\n");
}
}

bool ProcessKey(SDLKey sym);
void ProcessTouch(int x,int y);
GfxLibSDL gfxlib;

int main(int argc, char** argv)
{
	bool run;
	int numkey=0;
	PDL_ScreenMetrics ScreenMetrics;
	int sx,sy;
	
	
	atexit(ExitFunc);
  
	  
     	
	printf("DanSnakes: SDL version\n");
	
	// start the PDL library
	PDL_Init(0);
	PDL_GetScreenMetrics(&ScreenMetrics);
	printf("Screen size: %d %d\n",ScreenMetrics.horizontalPixels,ScreenMetrics.verticalPixels);

	// We actually don't use the full resolution because the snake is 1 pixel in size. We rely on upscaling to make this look less tiny
	printf("Hardware ID: %d\n",PDL_GetHardwareID());
	switch(PDL_GetHardwareID())
	{
		case HARDWARE_PRE:
		case HARDWARE_PRE_PLUS:
		case HARDWARE_PRE_2:
			sx = 320;
			sy = 480;
			break;
		case HARDWARE_PIXI:
		case HARDWARE_VEER:
			sx = 320;
			sy = 400;
			break;
		case HARDWARE_PRE_3:
			// Tweaked not to be too small
			sx = 240;
			sy = 400;
			break;
		default:
			sx = 320;
			sy = 200;
	}

	printf("Initializing snake with resolution %d %d\n",sx,sy);
	
	
	
	//gfxlib.GfxInit(320,400);
	//gfxlib.GfxInit(480,800);
	gfxlib.GfxInit(sx,sy);
	

  
  
  atexit(PDL_Quit);
  
  
	
#ifdef WIN32
	SDL_WM_SetCaption("DanSnakes", NULL);
#endif

	SnakeInit(&gfxlib);
	
	
	run=true;
	while(run)
	{
		// UI
		SDL_Event event;
		while( SDL_PollEvent( &event ) )
		{
			switch( event.type )
			{
				case SDL_MOUSEBUTTONDOWN:
					if(event.button.button==SDL_BUTTON_LEFT)
					{
						SnakeKeyStart();
						#ifdef TOUCHCONTROL 
							ProcessTouch(event.button.x,event.button.y);
						#endif
					}
					break;
				case SDL_KEYDOWN:
					/*char s[128];
					sprintf(s,"%d: %d",numkey++,event.key.keysym.sym);
					gfxlib.GfxFillRectangle(10,10,60,10,LRED);
					stringRGBA(gfxlib.GetSurface(),10,10, s, 255,127,30,255);*/
					run = ProcessKey(event.key.keysym.sym);

					break;
			}
		}

		SnakeUpdate();

		

		gfxlib.GfxUpdate();
		SDL_Delay(25);		
	}
	
	printf("End\n");
	
	
	
	return 0;
}


bool ProcessKey(SDLKey sym)
{
	bool run=true;
	switch(sym)
	{
		case SDLK_ESCAPE:
			run=false;
			break;
			
		case SDLK_SPACE:
			SnakeKeyStart();
			break;
	
		// Right player
	
		case SDLK_l:
			SnakeKeyfire(1,1);		// rocket
			break;
		case SDLK_BACKSPACE:		
			SnakeKeyfire(1,0);		// eraser
			break;
		case SDLK_RETURN:
			SnakeKeyl(1);					// left
			break;
		case SDLK_p:
			SnakeKeyr(1);					// right
			break;
			
		// left player
			// 307
		case SDLK_s:
			SnakeKeyfire(0,1);
			break;
		case SDLK_a:
			SnakeKeyfire(0,0);
			break;
		case SDLK_q:
			SnakeKeyl(0);
			break;
		case SDLK_RALT:
			SnakeKeyr(0);
			break;
	}
	return run;
}

void ProcessTouch(int x,int y)
{
	
	int player;
	int where;
    
	// Identify if it's a screen press on the left or right border
	player=-1;
	if(x<gfxlib.GetSx()/CONTROLSIZEDIVIDER)
		player = 0;
	if(x>gfxlib.GetSx()*(CONTROLSIZEDIVIDER-1)/CONTROLSIZEDIVIDER)
		player = 1;
	if(player==-1)
		return;
	
	// Identify which third of the screen is tapped
	if(y<gfxlib.GetSy()/4)
	 where=0;
	else
	 if(y<gfxlib.GetSy()*2/4)
	   where=1;
	else
	    if(y<gfxlib.GetSy()*3/4)
	      where=2;
	    else
	       where=3;
	
	// Act on the mouse press
	if(player==0)
	{
	 if(where==0)
	    SnakeKeyl(0);
	 if(where==1)
	    SnakeKeyr(0);
	 if(where==2)
	    SnakeKeyfire(0,1);
	 if(where==3)
	    SnakeKeyfire(0,0);
	}
	if(player==1)
	{
	 if(where==3)
	    SnakeKeyl(1);
	 if(where==2)
	    SnakeKeyr(1);
	 if(where==1)
	    SnakeKeyfire(1,1);
	 if(where==0)
	    SnakeKeyfire(1,0);
	}
}
