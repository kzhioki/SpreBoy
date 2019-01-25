#ifndef _SPREAUDIO_H_
#define _SPREAUDIO_H_

#include <Audio.h>
#include <SDHCI.h>
#include <errno.h>
#include <mqueue.h>
#include <semaphore.h>
#include <arch/board/board.h>

extern mqd_t mqd0;
extern mqd_t mqd1;
extern mqd_t mqdb;

enum cmd_e
{
  PLAY,
  PLAYCONT,
  STOP,
  REPEAT,
  VOL,
  BEEP,
  BEEP_VOL,
  BEEP_MUTE,
  INVALID = 0xffffffff,
};

struct msg_s
{
  enum cmd_e cmd;
  void* arg;
};

#define NOTE_B2     123
#define NOTE_C3     131
#define NOTE_CS3    139
#define NOTE_D3     147
#define NOTE_DS3    156
#define NOTE_E3     165
#define NOTE_F3     175
#define NOTE_FS3    185
#define NOTE_G3     196
#define NOTE_GS3    208
#define NOTE_A3     220
#define NOTE_AS3    233
#define NOTE_B3     247
#define NOTE_C4     262
#define NOTE_CS4    277
#define NOTE_D4     294
#define NOTE_DS4    311
#define NOTE_E4     330
#define NOTE_F4     349
#define NOTE_FS4    370
#define NOTE_G4     392
#define NOTE_GS4    415
#define NOTE_A4     440
#define NOTE_AS4    466
#define NOTE_B4     494
#define NOTE_C5     523
#define NOTE_CS5    554
#define NOTE_D5     587
#define NOTE_DS5    622
#define NOTE_E5     659
#define NOTE_F5     698
#define NOTE_FS5    740
#define NOTE_G5     784
#define NOTE_GS5    831
#define NOTE_A5     880
#define NOTE_AS5    932
#define NOTE_B5     988
#define NOTE_C6     1047
#define NOTE_CS6    1109
#define NOTE_D6     1175
#define NOTE_DS6    1245
#define NOTE_E6     1319
#define NOTE_F6     1397
#define NOTE_FS6    1480
#define NOTE_G6     1568
#define NOTE_GS6    1661
#define NOTE_A6     1760
#define NOTE_AS6    1865
#define NOTE_B6     1976
#define NOTE_C7     2093
#define NOTE_CS7    2217
#define NOTE_D7     2349
#define NOTE_DS7    2489
#define NOTE_E7     2637
#define NOTE_F7     2794
#define NOTE_FS7    2960
#define NOTE_G7     3136
#define NOTE_GS7    3322
#define NOTE_A7     3520
#define NOTE_AS7    3729
#define NOTE_B7     3951
#define NOTE_C8     4186
#define NOTE_CS8    4435
#define NOTE_D8     4699
#define NOTE_DS8    4978

class SpreAudio
{
public:

  static void begin();

  static void beep(int frq, int d) {
    struct msg_s smsg;

    smsg.cmd = BEEP;
    smsg.arg = (void*)(d << 16 | frq);
    mq_send(mqdb, (const char*)&smsg, sizeof(msg_s), 1);
  }

  static void noBeep() {
    struct msg_s smsg;

    smsg.cmd = BEEP;
    smsg.arg = 0;
    mq_send(mqdb, (const char*)&smsg, sizeof(msg_s), 1);
  }

  static void play(int id, const char *tune) {
    struct msg_s smsg;
    mqd_t mqd = (id == 0) ? mqd0 : mqd1;

    smsg.cmd = PLAY;
    smsg.arg = (void*)tune;

    mq_send(mqd, (const char*)&smsg, sizeof(msg_s), 0);
  }

  static void stop(int id) {
    struct msg_s smsg;
    mqd_t mqd = (id == 0) ? mqd0 : mqd1;

    smsg.cmd = STOP;
    smsg.arg = 0;

    mq_send(mqd, (const char*)&smsg, sizeof(msg_s), 0);
  }

  static void repeat(int id) {
    struct msg_s smsg;
    mqd_t mqd = (id == 0) ? mqd0 : mqd1;

    smsg.cmd = REPEAT;
    smsg.arg = (void*)1;

    mq_send(mqd, (const char*)&smsg, sizeof(msg_s), 0);
  }

  static void noRepeat(int id) {
    struct msg_s smsg;
    mqd_t mqd = (id == 0) ? mqd0 : mqd1;

    smsg.cmd = REPEAT;
    smsg.arg = (void*)0;

    mq_send(mqd, (const char*)&smsg, sizeof(msg_s), 0);
  }
};

#endif // _SPREAUDIO_H_
