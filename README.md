# Flappy Zone

1-key action game for PC-8001

## 開発環境

SDCC 4.0.0 (https://sdcc.sourceforge.net/) の他に以下のツールをインストールします。

なお、次のようにフォルダを構成することを想定しています。
(パス等が異なるの場合、Makefile を適宜修正してください。)

```
 -+- _utils
  +- n80dev
  +- flappyzone
```

- hex2bin (https://sourceforge.net/projects/hex2bin/)

SDCC ではインテル HEX が出力されるので、これをバイナリに変換するのに使います。
_utils に配置します。

- t88tool (http://bugfire2009.ojaru.jp/download.html#t88tool)

上記ツールで出力されたバイナリを cmt 形式のファイルに変換します。
_utils に配置します。

- n80dev (https://github.com/tako2/n80dev)

ビルド時に一緒にコンパイルされるライブラリです。

## コンパイル方法

このプロジェクトを github からダウンロードし、
コマンドラインから ./flappyzone/build に移動して make コマンドを実行します。

```
$ cd ./flappyzone/build
$ make
```

出力された cmt ファイルは、エミュレーター等で実行して動作を確認できます。
プログラムは mon か L を実行すれば自動的に実行されます。

```
mon
*L
```