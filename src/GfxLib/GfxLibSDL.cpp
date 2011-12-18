/*
	Copyright (C) 2011 Daniel Roggen, droggen@gmail.com

	All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// GfxLib: SDL version
// Version 0.01

#include "GfxLibSDL.h"

#include "SDL.h"
#include "SDL_gfxPrimitives.h"


extern "C" void ExitFunc();									// Function to call upon program termination

GfxLibSDL::GfxLibSDL() : GfxLib()
{
	GfxSurface=0;
	GfxSx=320;
	GfxSy=200;
	GfxFs=false;
}
GfxLibSDL::~GfxLibSDL()
{
}

void GfxLibSDL::GfxInit(int sx,int sy)
{
	GfxSx = sx;
	GfxSy = sy;
	GfxFs = false;
	SDL_version compiled;
	
	// Initialize SDL 
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
		printf("Couldn't initialize SDL: %s\n",SDL_GetError());
		exit(1);
	}
		SDL_VERSION(&compiled);
	printf("Compiled version: %d.%d.%d\n",
			compiled.major, compiled.minor, compiled.patch);
	printf("Linked version: %d.%d.%d\n",
			SDL_Linked_Version()->major,
			SDL_Linked_Version()->minor,
			SDL_Linked_Version()->patch);
	// On exit: clean SDL
	atexit(ExitFunc);
	//atexit(SDL_Quit);
	

	if(GfxFs)
		GfxSurface = SDL_SetVideoMode(GfxSx, GfxSy, 32, SDL_SWSURFACE | SDL_FULLSCREEN );
	else
		GfxSurface = SDL_SetVideoMode(GfxSx, GfxSy, 32, SDL_SWSURFACE );
	if( GfxSurface== NULL )
	{
		printf("Couldn't set video mode: %s\n",SDL_GetError());
		exit(1);
	}
	SDL_ShowCursor(SDL_DISABLE);
}
void GfxLibSDL::GfxGetSize(int *sx,int *sy)
{
	*sx=GfxSx;
	*sy=GfxSy;
}
void GfxLibSDL::GfxPutPixel(int x,int y,int color)
{
	pixelColor(GfxSurface,x,y,color);	
}
int GfxLibSDL::GfxGetPixel(int x,int y)
{
	unsigned int c;
	
	// Window clip
	if(x<0 || y<0 || x>(GfxSx-1) || y>(GfxSy-1))
		return 0;
			
	SDL_LockSurface(GfxSurface);
	int bpp = GfxSurface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)GfxSurface->pixels + y * GfxSurface->pitch + x * bpp;

	switch(bpp) {
		case 1:
			c=*p;
			break;
		
		case 2:
			c=*(Uint16 *)p;
			break;
		
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
		   	c=p[0] << 16 | p[1] << 8 | p[2];
			else
				c=p[0] | p[1] << 8 | p[2] << 16;
			break;
		
		case 4:
			c=*(Uint32 *)p;
			break;
		
		default:
			c=0;       /* shouldn't happen, but avoids warnings */
	}
	SDL_UnlockSurface(GfxSurface);
	unsigned char r,g,b;
	SDL_GetRGB(c,GfxSurface->format,&r,&g,&b);
	
	unsigned cc;
	cc = ((unsigned int)r)<<24 | ((unsigned int)g)<<16 | ((unsigned int)b)<<8 | 0xff;
	
	//printf("read (%d %d) -> %X (%x %x %x) %X\n",x,y,c,r,g,b,cc);
	
	
	
	return cc;
	//return WHITE;
}

void GfxLibSDL::GfxFillRectangle(int x1,int y1,int sx, int sy,int color)
{
	
	boxColor(GfxSurface, x1, y1,x1+sx,y1+sy,color);
}
void GfxLibSDL::GfxLine(int x1,int y1,int x2,int y2,int color)
{
	lineColor(GfxSurface,x1,y1,x2,y2,color);	
}

void GfxLibSDL::GfxUpdate()
{
	SDL_UpdateRect(GfxSurface, 0,0,0,0);
}

int GfxLibSDL::GetSx()
{
	return GfxSx;
}
int GfxLibSDL::GetSy()
{
	return GfxSy;
}

SDL_Surface *GfxLibSDL::GetSurface()
{
	return GfxSurface;
}
void GfxLibSDL::GfxString(int x,int y,const char *string, int color)
{
	stringColor(GfxSurface,x,y,string,color);
}


