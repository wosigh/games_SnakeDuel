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

#ifndef __GFXLIBSDL_H
#define __GFXLIBSDL_H

#include "SDL.h"
#include "GfxLib.h"
#include "SDL_gfxPrimitives.h"


#define BLACK 0x000000ffl
#define WHITE 0xffffffffl
#define RED 0xff0000ffl
#define LRED 0x800000ffl
#define BLUE 0x0000ffffl
#define LBLUE 0x000080ffl
#define GREY 0x808080ffl
#define LGREY 0xC0C0C0ffl

extern int GfxSx,GfxSy;
extern bool GfxFs;
extern SDL_Surface *GfxSurface;




class GfxLibSDL : public GfxLib
{
protected:
	SDL_Surface *GfxSurface;
	int GfxSx;
	int GfxSy;
	bool GfxFs;

public:
	GfxLibSDL();
	virtual ~GfxLibSDL();

	virtual void GfxInit(int sx,int sy);
	virtual void GfxGetSize(int *sx,int *sy);
	virtual void GfxPutPixel(int x,int y,int color);
	virtual int GfxGetPixel(int x,int y);
	virtual void GfxFillRectangle(int x1,int y1,int sx, int sy,int color);
	virtual void GfxLine(int x1,int y1,int x2,int y2,int color);
	virtual void GfxString(int x,int y,const char *string, int color);
	virtual void GfxUpdate();
	virtual int GetSx();
	virtual int GetSy();
	virtual SDL_Surface *GetSurface();
};



#endif


