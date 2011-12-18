/*
	SnakeDuel
	Copyright (C) 2011 Daniel Roggen, droggen@gmail.com

	All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef __SNAKEENGINE_H
#define __SNAKEENGINE_H

#include "../GfxLib/GfxLibSDL.h"

#define SPAWNRADIUS 12
#define CONTROLSIZEDIVIDER 5

// Set touchcontrol if the touchscreen area to control the snake should be displayed
#define	TOUCHCONTROL


void rotr(int *x,int *y);
void rotl(int *x,int *y);


extern GfxLib *snakegfxlib;



void ClearScreen(int color);

void DrawSnake(int x,int y,int color);
void DrawRocket(int x,int y,int color);
void DrawEraser(int x,int y,int color);
void DrawStatusBar();
void InitScreen();
void InitSnake(int player);
void InitGame();
void InitScreenFirsttime();

void fire(int player,int firetype);
void PlotFireUpdate();

void DrawDead(int p);

void SnakeInit(GfxLib *gl);
void SnakeUpdate();
void SnakeKeyStart();

void SnakeKeyl(int p);
void SnakeKeyr(int p);
void SnakeKeyfire(int player,int firetype);

#endif


