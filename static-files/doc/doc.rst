花AI塚 AIスクリプト ドキュメント
==================================================

概要
--------------------------------------------------

花AI塚のAIはLua5.1(本家サイト_、日本語版リファレンス_)で記述します。
花映塚の対戦が開始するとスクリプトがロードされ、以後対戦が終了するまで毎フレームmain関数が呼び出されます。
対戦終了後はスクリプトがアンロードされます。

.. _本家サイト: http://www.lua.org/
.. _日本語版リファレンス: http://milkpot.sakura.ne.jp/lua/lua51_manual_ja.html

Lua5.1標準ライブラリからの制限
--------------------------------------------------

セキュリティ上の都合により、提供する標準ライブラリを制限しています。

使用できないライブラリ
::::::::::::::::::::::::::::::::::::::::::::::::::

以下の関数はAIスクリプト内で使用できません。

- collectgarbage
- getfenv 
- getmetatable 
- load
- loadstring
- print
- rawequal
- rawget
- setfenv
- setmetatable 
- module
- require
- packageモジュールのすべての関数
- string.dump
- ioモジュールのうち
  - io.close
  - io.flush
  - io.input
  - io.lines
  - io.output
  - io.popen
  - io.read
  - io.tmpfile
  - io.write

- osモジュールのうち
  - os.execute
  - os.exit
  - os.getenv
  - os.remove
  - os.rename
  - os.setlocale
  - os.tmpname

- debugモジュールのすべての関数

使用できるが、制限されているライブラリ
::::::::::::::::::::::::::::::::::::::::::::::::::

以下の関数はAIスクリプト内で使用できますが、本来の関数よりも機能が制限されています。

- dofile
- loadfile
- io.open

これらはいずれも指定できるファイルパスが制限されています。
これらの関数に渡すファイルパスfilenameは::

  string.match(filename, '^[%w_%-]+%.%w+$')

にマッチしなければいけません。マッチしない場合はファイルオープンに失敗するなどのエラーが発生します。

以下のfilenameはマッチします。

- ``hoge-fuga_foo.bar``
- ``20140401-log.1``

以下のfilenameはマッチしません。

- ``./file.txt``
- ``dir/foo.txt``
- ``bar``

なお、これらの関数で指定したファイルパスはAIスクリプトのある場所からの相対パスとして解釈されます。
したがって、これらの関数はAIスクリプトのあるフォルダ内のファイルへのみアクセスできます。

また、dofileおよびloadfileでバイトコードをロードすることはできません。

API
--------------------------------------------------


花映塚のゲーム情報を取得したり、キー入力を送信するための関数やオブジェクトがAIスクリプトへ提供されます。

提供されるものとしては以下のものがあります。

- 関数
  - `sendKeys(key_state)`_
  - hitTest_

- グローバル変数
  - game_sides_
  - player_side_
  - difficulty_
  - round_

- 定数群
  - ExAttackType_.*
  - ItemType_.*
  - HitType_.*
  - CharacterType_.*

関数
::::::::::::::::::::::::::::::::::::::::::::::::::

sendKeys(key_state)
______________________

現在プレイしている自機に対するキー入力を花映塚へ送ります。
key_stateには整数値を指定し、各ビットがそれぞれどのキーを押すかを表します。

最下位ビットからそれぞれ

- 1bit目: Zキー(射撃)
- 2bit目: Xキー(緊急ボム)
- 3bit目: Shiftキー(低速)
- 4bit目: 未使用
- 5bit目: ↑キー(上移動)
- 6bit目: ↓キー(下移動)
- 7bit目: ←キー(左移動)
- 8bit目: →キー(右移動)

となっています。

hitTest(hitbody_a, hitbody_b)
__________________________________

当たり判定オブジェクトhitbody_aとhitbody_bとの当たり判定処理を行います。
hitbody_aとhitbody_bとがぶつかっている時はtrueを、そうでなければfalseを返します。

グローバル変数
::::::::::::::::::::::::::::::::::::::::::::::::::

game_sides#GameSide@game_sides
  
  2つのGameSide#GameSide からなる配列で、game_sides[1]は1P側の、game_sides[2]は2P側のゲーム状態を表します。
  このグローバル変数は毎フレーム更新されます。

  この変数を経由して得られるすべての値は次フレームで破壊的に変更される可能性があります。
  例えばある時点でgame_sides[1].enemies[1]が敵Aを表していたとして、
  次のフレームでは敵Bのデータに書き換えられている可能性があります。
  そのためgame_sidesのフィールドへの参照を保存して次フレームで参照するのは非推奨です。
  ある時点でのデータを次フレームに渡したい場合は深いコピーを行うなどして下さい。

player_side@player_side
  
  この変数はこのAIスクリプトが1P側か2P側かを表します。
  1P側ならば
  <pre>player_side == 1</pre>
  2P側ならば
  <pre>player_side == 2</pre>
  です。

difficulty@difficulty

  この変数は現在のゲームの難易度を表します。
  TODO: 難易度と値の対応表
  
round@round

  この変数は対戦開始からの経過ラウンド数を表します。

オブジェクト
::::::::::::::::::::::::::::::::::::::::::::::::::

GameSide@GameSide

  GameSideオブジェクトは対戦の各サイドのゲーム状態を表します。
  このオブジェクトは以下のフィールドを持ちます。

  GameSide.player
    Player#Player オブジェクトを持つフィールドで、自機に関する情報を表します。
  GameSide.enemies
    Enemy#Enemy オブジェクトの配列で、画面上の敵に関する情報を表します。
  GameSide.bullets
    Bullet#Bullet オブジェクトの配列で、画面上の敵弾に関する情報を表します。
  GameSide.exAttacks
    ExAttack#ExAttack オブジェクトの配列で、画面上のEXアタックに関する情報を表します。
  GameSide.items
    Item#Item オブジェクトの配列で、画面上のイテムに関する情報を表します。
  GameSide.round_win
    現在の対戦で勝利したラウンド数を表します。
  GameSide.score
    現在のスコアを表します。

Player
_________

Playerオブジェクトは自機に関する情報を表します。
このオブジェクトは以下のフィールドを持ちます。
  
Player.x / Player.y
  自機の位置を表します。

Player.life
  残りライフを表します。

Player.cardAttackLevel
  現在のカードアタックレベルを表します。

Player.bossAttackLevel
  現在のボスカードアタックレベルを表します。

Player.currentCharge / Player.currentChargeMax / Player.chargeSpeed
  currentChargeは現在のチャージ量、currentChargeMaxは現在チャージできる最大値、chargeSpeedは1フレームの間にチャージされる量を表します。
  チャージ量は0~400までの浮動小数点数で表されます。

Player.spellPoint
  現在のSpell Pointを表します。

Player.combo
  現在のコンボ数を表します。

Player.character
  自機キャラクターの種別を表します。
  CharacterType#CharacterType の定数のいずれかの値を取ります。

Player.speedFast
  高速時の自機の速さを表します。

Player.speedSlow
  低速時の自機の速さを表します。

Player.hitBodyRect　//TODO
  TODO

Player.hitBodyCircle
  TODO

Player.hitBodyForItem
  TODO

Enemy
Bullet
ExAttack
Item
HitBody

定数群
::::::::::::::::::::::::::::::::::::::::::::::::::

雑多
--------------------------------------------------

