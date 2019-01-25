/*
    Arduino Tetris
    Copyright (C) 2015 Joao Andre Esteves Vilaca

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

#include "Tetris.h"

SDClass theSD;

#define TFT_RST 8
#define TFT_DC  9
#define TFT_CS -1
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void drawPreGameScreen()
{
  tft.fillScreen(BLACK);
  drawCenteredString("Tetris", 40, 4, BLUE);
  drawCenteredString("Click A to play", 110, 2, WHITE);
}

void gameOver()
{
  tft.fillRect(32, 84, 256, 52, BLACK);
  drawString("Game Over", 48, 94, 4, WHITE);

  tft.drawRect(32, 84, 256, 52, RED);

  Joystick::shock(200);
  SpreAudio::beep(600, 200);
  usleep(300*1000);
  SpreAudio::beep(600, 200);
  usleep(300*1000);
  SpreAudio::beep(200, 600);
  usleep(1500*1000);

  SpreAudio::stop(0);
  SpreAudio::stop(1);

  Joystick::waitForClick(BUTTON_A);
}

void gameStart()
{
  SpreAudio::play(0, "Cat.mp3");
  SpreAudio::play(1, "Xylo1.mp3");
  SpreAudio::repeat(1);

  for (int i = 250; i < 2500; i += 100) {
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

  // title screen
  drawPreGameScreen();

  // wait a bit before playing sounds
  delay(700);

  // wait for click
  while (!Joystick::pressed(BUTTON_A));
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
  // game start
  gameStart();

  // load game
  Tetris t;
  t.run();

  // game ended
  gameOver();
}
