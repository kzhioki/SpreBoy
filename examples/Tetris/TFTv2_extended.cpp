/*

Edited by João Vilaça.

Currently an work in progress, a few changes were done:
- LED pin moved to pin 9 on Atmega 328;
- Screen orientation rotated 90/270º on initialization;
- Added a few new methods;
- Changed how text is written;
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
Foundation, Inc.,51 Franklin St,Fifth Floor, Boston, MA 02110-1301 USA

*/

#include "TFTv2_extended.h"

void drawStringWithShadow(const char *string, int16_t poX, int16_t poY, uint8_t size, uint16_t fgcolor, uint16_t shcolor)
{
  tft.setCursor(poX-1, poY+1);
  tft.setTextColor(shcolor);
  tft.setTextSize(size);
  tft.println(string);

  tft.setCursor(poX, poY);
  tft.setTextColor(fgcolor);
  tft.setTextSize(size);
  tft.println(string);
}

void drawString(const char *string, int16_t poX, int16_t poY, uint8_t size, uint16_t fgcolor)
{
  tft.setCursor(poX, poY);
  tft.setTextColor(fgcolor);
  tft.setTextSize(size);
  tft.println(string);
}

void drawCenteredString(const char *string, int16_t poY, uint8_t size, uint16_t fgcolor)
{
  int len = strlen (string)* FONT_SPACE * size;
  int left = (ILI9341_TFTHEIGHT - 1 - len ) / 2;

  drawString(string, left, poY, size, fgcolor);
}

uint8_t drawNumber(long long_num, int16_t poX, int16_t poY, uint8_t size, uint16_t fgcolor)
{
  tft.setCursor(poX, poY);
  tft.setTextColor(fgcolor);
  tft.setTextSize(size);
  tft.println(long_num);
  int f = 1;
  while (long_num > 0) {
    long_num /= 10;
    f++;
  }
  return f;
}

uint8_t drawNumber(long long_num, int16_t poX, int16_t poY, uint8_t size, uint16_t fgcolor, uint16_t bgcolor)
{
  tft.setCursor(poX, poY);
  tft.setTextColor(fgcolor, bgcolor);
  tft.setTextSize(size);
  tft.println(long_num);
  int f = 1;
  while (long_num > 0) {
    long_num /= 10;
    f++;
  }
  return f;
}

/*********************************************************************************************************
END FILE
*********************************************************************************************************/
