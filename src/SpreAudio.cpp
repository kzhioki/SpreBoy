
#include <Audio.h>
#include <SDHCI.h>
#include <errno.h>
#include <mqueue.h>
#include <semaphore.h>
#include <arch/board/board.h>

#include "SpreAudio.h"

extern SDClass theSD;
AudioClass *theAudio;

File myFile0;
File myFile1;

mqd_t mqd0;
mqd_t mqd1;
mqd_t mqdb;

int vol0 = -200;
int vol1 = -200;
short beep_vol = -40;

#define MUTE_PLAYER0 (0x00000001)
#define MUTE_PLAYER1 (0x00000002)
#define MUTE_BEEP    (0x00000004)

static void mute_control(uint32_t unmute, uint32_t mute)
{
  static sem_t g_sem_mute = SEM_INITIALIZER(1);
  static uint32_t g_unmute = 0;
  uint32_t old_unmute;

  sem_wait(&g_sem_mute);

  old_unmute = g_unmute;

  if (unmute) {
    g_unmute |= unmute;
  }
  if (mute) {
    g_unmute &= ~mute;
  }

  if (!old_unmute && g_unmute) {
    board_external_amp_mute_control(false);
  }
  if (old_unmute && !g_unmute) {
    board_external_amp_mute_control(true);
  }

  sem_post(&g_sem_mute);
}

int playerStatus[2] = {0, 0};

static int aplay(AudioClass::PlayerId id, File& myFile)
{
  ssize_t ret;
  struct msg_s rmsg;
  struct timespec ts;
  enum state_s {
    STOPPED = 0,
    PLAYING,
  } state = STOPPED;
  err_t err;
  bool repeat = false;

  int index = (id == AudioClass::Player0) ? 0 : 1;
  mqd_t mqd = (id == AudioClass::Player0) ? mqd0 : mqd1;

  while (1) {
    err = AUDIOLIB_ECODE_OK;

    if (state == PLAYING) {
      clock_gettime(CLOCK_REALTIME, &ts);
      ts.tv_nsec += 40000;
      if (ts.tv_nsec >= 1000000000) {
        ts.tv_sec++;
        ts.tv_nsec -= 1000000000;
      }
      ret = mq_timedreceive(mqd, (char*)&rmsg, sizeof(struct msg_s), NULL, &ts);
    } else {
      ret = mq_receive(mqd, (char*)&rmsg, sizeof(struct msg_s), NULL);
    }

    if (ret < 0) {
      if ((errno == ETIMEDOUT) && (state == PLAYING)) {
        rmsg.cmd = PLAYCONT;
        rmsg.arg = 0;
      } else {
        rmsg.cmd = INVALID;
        rmsg.arg = 0;
      }
    }

    //printf("cmd=%d arg=%d\n", rmsg.cmd, rmsg.arg);
    switch (rmsg.cmd) {
    case PLAY:
      if (state == PLAYING) {
#if 1 // ignore play command during playing
        //puts("IGNORE!!");
        break;
#else
        sleep(1);
        theAudio->stopPlayer(id);
        myFile.close();
#endif
      }
      //puts("PLAY!!");
      //printf("%s\n", (const char*)rmsg.arg);
      myFile = theSD.open((const char*)rmsg.arg);
      err = theAudio->writeFrames(id, myFile);
      mute_control(((index) ? MUTE_PLAYER1 : MUTE_PLAYER0), 0);
      theAudio->startPlayer(id);
      state = PLAYING;
      playerStatus[index] = PLAYING;
      /* FALLTHRU */
      //break;
    case PLAYCONT:
      //puts("PLAYCONT!!");
      if (err != AUDIOLIB_ECODE_FILEEND) {
        err = theAudio->writeFrames(id, myFile);
      }
      if (err == AUDIOLIB_ECODE_FILEEND) {
        if (repeat) {
          myFile.seek(0);
        } else {
          goto stop;
        }
      }
      break;
    case STOP:
    stop:
      //puts("STOP!!");
      if (state == PLAYING) {
        theAudio->stopPlayer(id);
        mute_control(0, ((index) ? MUTE_PLAYER1 : MUTE_PLAYER0));
        myFile.close();
      }
      state = STOPPED;
      playerStatus[index] = STOPPED;
      break;
    case VOL:
      //puts("VOL!!");
      if (id == AudioClass::Player0) {
        vol0 = (int)rmsg.arg;
      } else {
        vol1 = (int)rmsg.arg;
      }
      theAudio->setVolume(-2, vol0, vol1);
      break;
    case REPEAT:
      //puts("REPEAT!!");
      repeat = ((int)rmsg.arg) ? true : false;
      break;
    case INVALID:
    default:
      //puts("INVALID!!");
      break;
    }
  }

  return 0;
}

static int aplay0(int argc, FAR char *argv[])
{
  return aplay(AudioClass::Player0, myFile0);
}

static int aplay1(int argc, FAR char *argv[])
{
  return aplay(AudioClass::Player1, myFile1);
}

static int bplay(int argc, FAR char *argv[])
{
  ssize_t ret;
  struct msg_s rmsg;
  struct timespec ts;
  enum state_s {
    MUTED = 0,
    UNMUTED,
  } state = MUTED;
  short frequency;
  uint16_t duration;

  clock_gettime(CLOCK_REALTIME, &ts);

  while (1) {
    if (state == UNMUTED) {
      ts.tv_sec += 5;
      ret = mq_timedreceive(mqdb, (char*)&rmsg, sizeof(struct msg_s), NULL, &ts);
    } else {
      ret = mq_receive(mqdb, (char*)&rmsg, sizeof(struct msg_s), NULL);
    }

    if (ret < 0) {
      if ((errno == ETIMEDOUT) && (state == UNMUTED)) {
        rmsg.cmd = BEEP_MUTE;
        rmsg.arg = 0;
      } else {
        rmsg.cmd = INVALID;
        rmsg.arg = 0;
      }
    }

    //printf("cmd=%d arg=%d\n", rmsg.cmd, rmsg.arg);
    switch (rmsg.cmd) {
    case BEEP:
      //puts("BEEP!!");
      frequency = (short)((int)rmsg.arg & 0xffff);
      duration = (uint16_t)(((int)rmsg.arg >> 16) & 0xffff);
      if (frequency) {
        clock_gettime(CLOCK_REALTIME, &ts);
        mute_control(MUTE_BEEP, 0);
        theAudio->setBeep(1, beep_vol, frequency);
        state = UNMUTED;
        if (duration) {
          usleep(duration * 1000);
          theAudio->setBeep(0, 0, 0);
        }
      } else {
        theAudio->setBeep(0, 0, 0);
      }
      break;
    case BEEP_VOL:
      //puts("BEEP_VOL!!");
      beep_vol = (short)((int)rmsg.arg);
      break;
    case BEEP_MUTE:
      //puts("BEEP_MUTE!!");
      // delayed mute
      mute_control(0, MUTE_BEEP);
      state = MUTED;
      break;
    case INVALID:
    default:
      //puts("INVALID!!");
      break;
    }
  }

  return 0;
}

void SpreAudio::begin()
{
  theAudio = AudioClass::getInstance();
  theAudio->begin();
  theAudio->setRenderingClockMode(AS_CLKMODE_NORMAL);
  theAudio->setPlayerMode(AS_SETPLAYER_OUTPUTDEVICE_SPHP);

  theAudio->initPlayer(AudioClass::Player0,
                       AS_CODECTYPE_MP3,
                       "/mnt/sd0/BIN",
                       AS_SAMPLINGRATE_AUTO,
                       AS_CHANNEL_MONO);
  theAudio->initPlayer(AudioClass::Player1,
                       AS_CODECTYPE_MP3,
                       "/mnt/sd0/BIN",
                       AS_SAMPLINGRATE_AUTO,
                       AS_CHANNEL_MONO);
  theAudio->setVolume(-10, vol0, vol1);
  /* Create audio player message queue */
  struct mq_attr mq_attr;
  //mq_attr.mq_maxmsg  = 1;
  mq_attr.mq_maxmsg  = 100;
  mq_attr.mq_msgsize = sizeof(struct msg_s);
  mq_attr.mq_flags   = 0;
  mqd0 = mq_open("MQD0", O_CREAT | O_RDWR, 0666, &mq_attr);
  mqd1 = mq_open("MQD1", O_CREAT | O_RDWR, 0666, &mq_attr);
  mqdb = mq_open("MQDB", O_CREAT | O_RDWR, 0666, &mq_attr);

  /* Create audio player task */
  task_create("aplay0", 100, 1024, aplay0, NULL);
  task_create("aplay1", 100, 1024, aplay1, NULL);
  task_create("bplay", 105, 1024, bplay, NULL);
}
