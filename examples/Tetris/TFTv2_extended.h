/*

Edited by João Vilaça.

Currently an work in progress, a few changes were done:
- LED pin moved to pin 9 on Atmega 328;
- Screen orientation rotated 90/270º on initialization;
- Added a few new methods;
- ...

*/
/*
2012 Copyright (c) Seeed Technology Inc.

Authors: Albert.Miao & Loovee,
Visweswara R (with initializtion code from TFT vendor)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/
#ifndef TFTv2_h
#define TFTv2_h

#define FONT_SPACE 6
#define FONT_X 8

#if defined(ARDUINO) && ARDUINO >= 100
#define SEEEDUINO
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <avr/pgmspace.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library

extern Adafruit_ILI9341 tft;

#define BLACK       0x0000
#define BLUE	      0x001f
#define CYAN		    0x07ff
#define DARKCYAN    0x03EF
#define DARKGREEN	  0x03E0
#define DARKGREY    0x7BEF
#define GRAY1		    0x8410
#define GRAY2		    0x4208
#define GRAY3		    0x2104
#define GREEN       0x07e0
#define LIGHTGREEN  0xAFE5
#define LIGHTGREY   0xC618
#define MAGENTA     0xF81F
#define MAROON      0x7800
#define NAVY        0x000F
#define OLIVE       0x7BE0
#define ORANGE      0xFD20
#define PURPLE      0x780F
#define RED         0xf800
#define WHITE       0xffff
#define YELLOW      0xffe0

#define MIN_X	0
#define MIN_Y	0
#define MAX_X	319
#define MAX_Y	239

#define TS_MINX 116*2
#define TS_MAXX 890*2
#define TS_MINY 83*2
#define TS_MAXY 913*2

void drawStringWithShadow(const char *string, int16_t poX, int16_t poY, uint8_t size, uint16_t fgcolor, uint16_t shcolor);
void drawCenteredString(const char *string, int16_t poY, uint8_t size, uint16_t fgcolor);
void drawString(const char *string, int16_t poX, int16_t poY, uint8_t size, uint16_t fgcolor);
uint8_t drawNumber(long long_num, int16_t poX, int16_t poY, uint8_t size, uint16_t fgcolor);
uint8_t drawNumber(long long_num, int16_t poX, int16_t poY, uint8_t size, uint16_t fgcolor, uint16_t bgcolor);

#endif

/*********************************************************************************************************
END FILE
*********************************************************************************************************/
