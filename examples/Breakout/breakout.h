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
#ifndef BREAKOUTCPP
#define BREAKOUTCPP

#include "TFTv2_extended.h"
#include "ball.cpp"
#include "paddle.cpp"
#include "joystick.h"
#include "SpreAudio.h"
#include "config.h"
#include "scoreboard.cpp"
#include "tiles.cpp"

class Breakout
{
  Joystick js;

  Ball ball;
  
  Paddle paddle;

  Scoreboard score;

  Tiles tiles;

  int beep;

  public:
  
    unsigned long last;
  
    Breakout(): ball(BOARD_LEFT, BOARD_RIGHT - BALL_R * 2, BOARD_TOP, MAX_Y - BALL_R){
    }

    void run()
    {

      // blank board

      tft.fillRect(0,            0, MAX_X,             BOARD_TOP,            SCOREBOARD_COLOR);
      tft.fillRect(0,            0, BOARD_LEFT - 1,    MAX_Y - PADDLE_H - 2, SCOREBOARD_COLOR);
      tft.fillRect(BOARD_RIGHT,  0, TILE_W / 2,        MAX_Y - PADDLE_H - 2, SCOREBOARD_COLOR);
      tft.fillRect(BOARD_LEFT, BOARD_TOP, BOARD_RIGHT - BOARD_LEFT , MAX_Y - BOARD_TOP - PADDLE_H, BACKGROUND_COLOR);
      tft.fillRect(0, MAX_Y - PADDLE_H, MAX_X, MAX_Y, BACKGROUND_COLOR);

      score.init();

      // initialize tile matrix
      newLevel();
      paddle.setXY((MAX_X -PADDLE_W)/ 2, MAX_Y - PADDLE_H);
      paddle.draw();

      do
      {
        serve();
        play();

      } while (score.hasLivesLeft());
    
      const char *go ="GAME OVER";
      const int font_size = 5;
      int cursor = (MAX_X - font_size * FONT_SPACE * 9) / 2;

      boolean skip = false;

      drawStringWithShadow(go, cursor - FONT_SPACE / 2, 94 + FONT_SPACE / 2, font_size, WHITE, RED);

      Joystick::shock(200);

      for ( int i= 0; i < 9; i++)
      {
        skip = skip || Joystick::pressed(BUTTON_A);

        if ( skip ) continue;

        for (int j = (10-i) * 100; j > 200; j-=1500 / (i+1))
        {
          SpreAudio::beep(j, 100);
          usleep(100*1000);
        }
      }

      Joystick::waitForRelease(BUTTON_A);
    }

  private:

    void serve()
    {
      ball.setXY(paddle.getX(), MAX_Y - PADDLE_H - BALL_R * 2 - 2);
      ball.draw();
      do
      {
        const int m = js.getX();
        if ( m != 0)
        {
          paddle.draw(m);
          ball.clear();
          ball.setX(paddle.getX());
          ball.draw();
          delay(5);
        }
      } while (!Joystick::pressed(BUTTON_A));
    }

    void play()
    {
      for(;;)
      {

        unsigned long waited = millis() - last;

        if ( waited > BALL_MOVE_WAIT )
        {
          last = millis();
        
          ball.clear();
          ball.move();
          ball.draw();
        
          if (ball.collision(paddle))
          {
            beep = 200;
            SpreAudio::beep(400,30);
          }
        
          // check collision w/ tiles
        
          int scored = ball.collision(tiles);

          if ( scored > 0)
          {
            beep *=1.1;
            if ( beep > 3000) beep = 3000;
            SpreAudio::beep(beep,25);
          }

          if (scored > 0)
          {
            score.update(scored);
          
            if ( tiles.allGone() )
            {
              ball.clear();
              newLevel();
              score.nextLevel();
              break;
            }
          }

          if (ball.escaped(paddle))
          {
            score.died();

            for (int i = 54; i > 30; i--)
            {
              SpreAudio::beep(random(i,i*i), 3);
              delay(6);
            }
          
            ball.clear();
          
            break;
          }
        }

        const int m = js.getX();
        if (waited > PADDLE_MOVE_WAIT && m != 0)
        {
          last = millis();
          paddle.draw(m);
        }
      }
    }

    void newLevel()
    {
      randomSeed((unsigned long)(millis() & 0xffffffff));

      tiles.drawAll();

      ball.setXY(MAX_X / 2, MAX_Y - PADDLE_H - BALL_R * 2 - 2);
      ball.setXiYi(-BALL_SPEED_H, -BALL_SPEED_V);
    
      beep = 200;
    }
};

#endif
