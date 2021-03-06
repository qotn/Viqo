Viqo
====

Viqo is a cross-platform "Nico Nama" comment viewer.

Viqo 是跨平台"nico生放"留言查看程序。

Viqo はクロスプラットフォームなニコニコ生放送用 コメントビューア（ニコ生 コメビュ）です。

* クロスプラットフォーム GUI アプリケーション
* Qt5 / C++11
* オープンソース (MIT license)

このコメビュは *自由* であることを目指しています。

ダウンロード
------------

### [Version 2.0](https://github.com/diginatu/Viqo/releases/tag/v2.0)

Ubuntu 14.04, Debian 7.7, OSX Yosemite, Windows 7, 8.1 などで動作確認されています。

使い方
------


[詳しくはWikiへ](https://github.com/diginatu/Viqo/wiki)

サイトからURLのアイコンをドロップして受信できます。

ライセンス
----------

MIT ライセンス

注意
----

#### Nov 25, 2014 のコミットからセーブファイルの保存場所が変更されました。
それ以前からのコミットへバージョンアップする場合で以前のセーブファイルを使いたい場合はファイルを移動する必要があります。

 以前のセーブファイルは実行したカレントディレクトリに作成されていました。

 今後のバージョンでは QStandardPaths を使うため、

  * settings.json
  * user.sqlite

 の２ファイルを移動してください。

[QStandardPaths](http://qt-project.org/doc/qt-5/qstandardpaths.html#LocateOption-enum)

"The following table gives examples of paths on different operating systems." 以降のパス参考


### Viqoの目指す自由
 自由なソフトウェアというだけでなく、クロスプラットフォームで沢山の人が使えることや、多くの目的に素早く適応できるようにコマンドを入力できるようにするなどといった、使用面での自由度など、あらゆる自由を目標にしています。

画面説明
--------

* 画面下部のタブ画面
  + コメント タブ
    - 受信されたコメントが、ここに表示されます。
    - 自動名前取得 にチェックを入れておくとコメント受信時にユーザ名を自動取得します
  + コマンド タブ
    - それぞれのイベント時に任意のOSコマンドを実行できる機能です
    - 最後にある「コマンド実行文字列」が使えます

* メニュー
  + ファイル
    - 状態セーブ、ロード (現在の設定状態を保存することができます)
      + デフォルト(1番)に保存した内容は起動時に適応されます。
    - 設定
      + アカウント (セッション、ユーザ情報)
        * コメント受信にはセッションが必須です
        * ユーザ情報はお気に入りコミュ取得、次枠移動、コメント送信の一部などに必要なので必ず(両方とも)設定してください。
  + 編集
    - コメント一覧を消去
      + 非常にコメントが多い放送や長時間でコメントがたまった場合にコメント欄を消去できます。（新たに受信したコメントのみ表示されます）


###コマンド実行文字列

####特殊文字列一覧

```
%newTitle%    :  放送開始した放送のタイトル
%userID%      :  ユーザID (数字)
%commentNo%   :  コメント番号
%userName%    :  ユーザ名 (登録済みのみ)
%comment%     :  コメント内容
%lastTime%    :  終了前何分に設定されているか（数字のみ）
%wakuURL%     :  現在受信しているの放送URL
```

####使用可能コマンド

```
            開始  コメント 終了 次枠
%newTitle%    O      X      X    X
%userID%      X      O      X    X  
%commentNo%   X      O      X    X  
%userName%    X      O      X    X  
%comment%     X      O      X    X  
%lastTime%    X      X      O    X  
%wakuURL%     X      X      X    O  
```
順次増やしていく予定です

コンパイル
----------

Qt 5.4 の環境でこのプロジェクトをコンパイルしてください。
実行はできた実行ファイルを実行するだけです。

コントリビュート
----------------

小さなバグ修正から機能追加など、プルリクエストは歓迎します。

使い方・解説を書いてくれる方がいると嬉しいです。

