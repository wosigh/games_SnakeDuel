/*
	Copyright (C) 2011 Daniel Roggen, droggen@gmail.com

	All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __GFXLIB_H
#define __GFXLIB_H



class GfxLib
{
public:
	GfxLib(){atexit(SDL_Quit);};
	virtual ~GfxLib() {};

	virtual void GfxInit(int sx,int sy)=0;
	virtual void GfxGetSize(int *sx,int *sy)=0;
	virtual void GfxPutPixel(int x,int y,int color)=0;
	virtual int GfxGetPixel(int x,int y)=0;
	virtual void GfxFillRectangle(int x1,int y1,int sx, int sy,int color)=0;
	virtual void GfxLine(int x1,int y1,int x2,int y2,int color)=0;
	virtual void GfxString(int x,int y,const char *string, int color)=0;
	virtual void GfxUpdate()=0;	
	virtual int GetSx()=0;
	virtual int GetSy()=0;

};


#endif
