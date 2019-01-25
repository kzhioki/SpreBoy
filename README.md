# Spreboy

Spresenseボードでゲームコンソールを動かすための簡単なサンプルコードです。

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

examples のテトリスとブロック崩しは Handheld-Color-Console をベースに Spresense 用に移植したものです。

* https://github.com/vilaca/Handheld-Color-Console

ILI9341 LCD への描画は以下のライブラリを使用しています。
これらのライブラリもインストールしてください。

* https://github.com/kzhioki/Adafruit_ILI9341
* https://github.com/kzhioki/Adafruit-GFX-Library

patches 以下はオプションとして用意しています。

* 0001-Update-for-repeat-playback.patch
 - 効果音など小サイズのMP3ファイルを再生する場合に適用してください
* 0002-Remove-mute-control-from-audio-library.patch
 - Audioライブラリ内のmute制御をやめて、スケッチ側でmute制御します
 - 無音状態で電源入り切りしたときのボツ音が気になる場合に適用してください
* 0003-Reduce-application-RAM-size-for-audio-buffer.patch
 - Audioライブラリが使用するメモリサイズを削っています
 - メモリサイズが足りなくなった場合に適用してください
* 0004-Reduce-shared-RAM-size-for-audio-buffer.patch
 - Audioライブラリが使用するメモリサイズを削っています
 - メモリサイズが足りなくなった場合に適用してください

また、全 patch 適用済みの Audio ライブラリを zip 化しています
* AudioCustom.zip
 - %USERPROFILE%\AppData\Local\Arduino15\packages\SPRESENSE\hardware\spresense\1.1.3\libraries\Audio と差し替えて使用してください

