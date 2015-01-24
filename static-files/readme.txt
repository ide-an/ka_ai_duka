花AI塚 ～ 東方花映塚AI自作ツール
Version: 1.4
Author: いで(@ide_an)
対応バージョン: 東方花映塚ver 1.50a
                東方花映塚ver 1.00
----------------------------------------

このアプリケーションは東方花映塚のAIを自作できるツールです。

1. インストール
  zipを解凍するだけ。このreadmeを読めてるならOKですね。

2. アンインストール
  レジストリは使用していないので、フォルダごと削除するだけでOKです。

3. ファイル概要

  ka_ai_duka.exe
    このファイルを実行するとAIを実行できる状態で花映塚を起動します。
    この状態でStory modeやMatch modeを選ぶとAIがプレイします。
  setting.exe
    後述のka_ai_duka.iniを編集するツールです。
  ka_ai_duka.ini
    設定ファイル。ka_ai_duka.exeを実行する前に予め設定しておきます。
  inject.dll
    花映塚の実行ファイルにロードされるDLL。
  sample-script
    AIスクリプトのサンプルです。
      random-walk
      passive-avoid
      shoot-nearest
    の3種類があります。
    それぞれmain.luaをロードして実行します。
  doc
    AIの記述に関するドキュメントです。

4. 使い方

  ※ 事故ったときに備えてscore.datをバックアップしておくことを推奨します。

  1) setting.exeを起動し、
       * 花映塚の実行ファイルのパス
       * 実行したいAIのスクリプトのパス
     を設定する。
     (※いずれも絶対パスで指定して下さい)
     例えばsample-script\random-walkのAIを動かしたい場合は、実行したいAIのスクリプトのパスを
       (花AI塚のあるフォルダのパス)\sample-script\random-walk\main.lua
     に設定します。
  2) setting.exeを閉じ、ka_ai_duka.exeを起動する。
  3) Story modeやMatch modeを選択してゲームを開始。
  4) あとはご自由に。

5. FAQ

  Q. VsyncPatchやAdonisと併用できますか?
  A. できません。対応予定無いです。

  Q. ネット対戦で使用できますか?
  A. できません。これは意図的に使用できなくしています。

  Q. 花AI塚を用いて作成したリプレイのアップロードはOK?
  A. 外部ツールを使用したリプレイのアップロードを認めない場所へのアップロードはNGです。
     それ以外の場所であればおそらく問題ないと思いますが、アップロードする際には
     (人間ではなく)AIによるリプレイである旨を明記するのが望ましいです。

  Q. 再配布してもいい?
  A. どうぞ。 配布するときは必ずLICENSE.txtも同封して下さい。

  Q. バグ踏んだ。
  Q. 要望がある。
  A. 報告お願いします。幻想板ツールスレあたりに投下すれば反応します。
     Twitterでも可です。@ide_anに投げれば反応します。

6. TODO

  * EXアタックが1P/2P間をまたぐときの速度取得

7. 動作確認環境

  Windows7(64bit)
  Celeron SU2300 1.20GHz
  RAM 4.00GB

  割とつらい環境でもぼちぼち動きます。

8. 更新履歴

  2015/01/24 ver1.4リリース
    修正
      ver1.2およびver1.3でtable.remove, table.insert, selectが正しく動作しなくなっていたのを修正。

  2015/01/12 ver1.3リリース
    修正
      射命丸EXアタックの座標がズレてたので修正。

  2014/12/21 ver1.2リリース
    パフォーマンス改善
      LuaJITを導入。その他少し。
    修正
      ドキュメントのフォーマットが崩れてたので修正。

  2014/05/05 ver1.1リリース
    追加
      花映塚製品版ver1.00に対応。
      Bullet.isErasableを追加、消せる弾かどうか識別可能に。
      ChargeTypeおよびGameSide.chargeTypeを追加、チャージタイプの識別を可能に。
    修正
      hitTest関数の自機vsレーザーの当たり判定処理のバグを修正。
      GameSide.round_win → GameSide.roundWin にフィールド名を変更。

  2014/04/25 ver1.0リリース
