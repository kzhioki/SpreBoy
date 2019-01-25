# Spreboy

Spresense�{�[�h�ŃQ�[���R���\�[���𓮂������߂̊ȒP�ȃT���v���R�[�h�ł��B

```
|-- README.md
|-- library.properties
|-- examples
|   |-- Breakout
|   `-- Tetris
|-- patches
|   |-- 0001-Update-for-repeat-playback.patch
|   |-- 0002-Remove-mute-control-from-audio-library.patch
|   |-- 0003-Reduce-application-RAM-size-for-audio-buffer.patch
|   |-- 0004-Reduce-shared-RAM-size-for-audio-buffer.patch
|   `-- AudioCustom.zip
`-- src
    |-- SpreAudio.cpp
    `-- SpreAudio.h
```

examples �̃e�g���X�ƃu���b�N������ Handheld-Color-Console ���x�[�X�� Spresense �p�ɈڐA�������̂ł��B

* https://github.com/vilaca/Handheld-Color-Console

ILI9341 LCD �ւ̕`��͈ȉ��̃��C�u�������g�p���Ă��܂��B
�����̃��C�u�������C���X�g�[�����Ă��������B

* https://github.com/kzhioki/Adafruit_ILI9341
* https://github.com/kzhioki/Adafruit-GFX-Library

patches �ȉ��̓I�v�V�����Ƃ��ėp�ӂ��Ă��܂��B

* 0001-Update-for-repeat-playback.patch
 * ���ʉ��ȂǏ��T�C�Y��MP3�t�@�C�����Đ�����ꍇ�ɓK�p���Ă�������
* 0002-Remove-mute-control-from-audio-library.patch
 * Audio���C�u��������mute�������߂āA�X�P�b�`����mute���䂵�܂�
 * ������Ԃœd������؂肵���Ƃ��̃{�c�����C�ɂȂ�ꍇ�ɓK�p���Ă�������
* 0003-Reduce-application-RAM-size-for-audio-buffer.patch
 * Audio���C�u�������g�p���郁�����T�C�Y������Ă��܂�
 * �������T�C�Y������Ȃ��Ȃ����ꍇ�ɓK�p���Ă�������
* 0004-Reduce-shared-RAM-size-for-audio-buffer.patch
 * Audio���C�u�������g�p���郁�����T�C�Y������Ă��܂�
 * �������T�C�Y������Ȃ��Ȃ����ꍇ�ɓK�p���Ă�������

�܂��A�S patch �K�p�ς݂� Audio ���C�u������ zip �����Ă��܂�
* AudioCustom.zip
 * %USERPROFILE%\AppData\Local\Arduino15\packages\SPRESENSE\hardware\spresense\1.1.3\libraries\Audio �ƍ����ւ��Ďg�p���Ă�������

