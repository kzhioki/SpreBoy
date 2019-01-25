/*
    Arduino Breakout
    Copyright (C) 2015 João André Esteves Vilaça

    https://github.com/vilaca/Handheld-Color-Console

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <SDHCI.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "SpreAudio.h"
#include "joystick.h"
#include "TFTv2_extended.h"
#include "breakout.h"

SDClass theSD;

#define TFT_RST 8
#define TFT_DC  9
#define TFT_CS -1
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void drawWallTiles()
{
  for ( int c = 0; c <  320; c+=40)
  {
    
    tft.fillRect(c+0, 0, 39, 240, RED);

    for ( int i = 20; i < 240; i+= 20)
    {
      tft.drawFastHLine(c+0, i, 40, GRAY2);
      tft.drawFastHLine(c+0, i+1, 40, GRAY2);
    }

    for ( int i = 0; i < 240; i+= 40)
    {
      tft.drawFastVLine(c+20, i, 20, GRAY2 );
      tft.drawFastVLine(c+21, i, 20, GRAY2 );
      tft.drawFastVLine(c+39, i+20, 20, GRAY2 );
      tft.drawFastVLine(c+38, i+20, 20, GRAY2 );
    }
  }
}

void animateScreen()
{
  int addr = 320;
  while(addr >= 0 && !Joystick::pressed(BUTTON_A))
  {
    tft.scrollTo(addr);
    addr-=2;
    
    delay(10);
  }
  tft.scrollTo(0);
}

void breakout_main() {

  drawWallTiles();

  const char* bo ="BREAKOUT";
  const int len = strlen (bo) * FONT_SPACE * 6;
  const int left = (MAX_X - len ) / 2;

  drawStringWithShadow(bo, left, 78, 6, WHITE, BLACK);

  boolean go = false;
  do
  {
    animateScreen();

    drawCenteredString("Press A to play!", 136, 2, CYAN);

    unsigned long last = millis();
    do
    {
      go = Joystick::pressed(BUTTON_A);

      if ( millis() - last > 8000 ) break;

    } while (!go);

  } while (!go);

  Breakout game;
  game.run();
}

void breakout_start()
{
  struct msg_s smsg;

  SpreAudio::play(0, "Cat.mp3");
  SpreAudio::play(1, "Xylo1.mp3");
  SpreAudio::repeat(1);

  for (int i = 250; i < 2500; i += 100)
    {
      SpreAudio::beep(i, 10);
      delay(10);
      SpreAudio::beep(i / 2, 10);
      delay(20);
    }

  SpreAudio::beep(NOTE_C4,40);
  SpreAudio::beep(NOTE_F4,40);
  SpreAudio::beep(NOTE_G4,40);
  SpreAudio::beep(NOTE_D5,40);
  SpreAudio::beep(NOTE_G5,40);
  SpreAudio::beep(NOTE_A5,40);
  SpreAudio::beep(NOTE_C6,40);
  SpreAudio::beep(NOTE_F6,40);

  breakout_main();

  SpreAudio::stop(1);
}

void setup()
{
  SpreAudio::begin();
  Joystick::init();

  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
}

void loop()
{
  breakout_start();

  usleep(1);
}
