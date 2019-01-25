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

#ifndef JOYSTICKCPP
#define JOYSTICKCPP

#include <Arduino.h>

// analog pins

#define XPIN        A4
#define YPIN        A5

// digital pin

#define BUTTON_A        PIN_D02
#define BUTTON_B        PIN_D03
#define BUTTON_START    PIN_D04
#define BUTTON_SELECT   PIN_D07
#define SHOCK_PIN       PIN_D05

// joystick center for both axis

#define CENTER        128

class Joystick
{
  public:

    // joystick position constants 
    
    static const int NEUTRAL = 0;
    static const int SOFT = 1;
    static const int HARD = 2;
    static const int HARDER = 3;

    static void init ()
    {
      pinMode ( BUTTON_A, INPUT_PULLUP );
      pinMode ( BUTTON_B, INPUT_PULLUP );
      pinMode ( BUTTON_START, INPUT_PULLUP );
      pinMode ( BUTTON_SELECT, INPUT_PULLUP );
    }

    static int getX()
    {
      return getPosition(XPIN) * -1;
    }

    static int getY()
    {
      return getPosition(YPIN) * -1;
    }

    static boolean pressed(uint8_t pin)
    {
      return digitalRead(pin) == LOW;
    }

    static void waitForRelease(uint8_t pin)
    {
      while (pressed(pin));
    }

    static void waitForRelease(uint8_t pin, int howLong)
    {
      int c = 0;
      do
      {
        delay (10);
        c += 10;
      }
      while ((pressed(pin) || getY() != 0 || getX() != 0) && c < howLong);
    }

    static void waitForClick(uint8_t pin)
    {
      while (!pressed(pin));
    }

    static void shock(int ms)
    {
        pinMode(SHOCK_PIN, OUTPUT);
        digitalWrite(SHOCK_PIN, LOW);
        delay(ms);
        digitalWrite(SHOCK_PIN, HIGH);
    }
  private:

    static int getPosition (int pin)
    {
      int n = analogRead(pin);
    
      n -= CENTER;
      if (n > 0) {
        n /= 150;
      } else {
        n /= 50;
      }
      if (n > 4) n = 4;
      return n;
    }
};

#endif
