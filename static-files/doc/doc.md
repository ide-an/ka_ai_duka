% 花AI塚 AIスクリプト ドキュメント

##概要

花AI塚のAIはLua5.1([本家サイト](http://www.lua.org/)、[日本語版リファレンス](http://milkpot.sakura.ne.jp/lua/lua51_manual_ja.html))で記述します。
花映塚の対戦が開始するとスクリプトがロードされ、以後対戦が終了するまで毎フレームmain関数が呼び出されます。
大戦終了後はスクリプトがアンロードされます。

##Lua5.1標準ライブラリからの制限

セキュリティ上の都合により、提供する標準ライブラリを制限しています。

### 使用できないライブラリ

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
- package.*
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
- debug.*

### 使用できるが、制限されているライブラリ

以下の関数はAIスクリプト内で使用できますが、本来の関数よりも機能が制限されています。

- dofile
- loadfile
- io.open

これらはいずれも指定できるファイルパスが制限されています。
これらの関数に渡すファイルパスfilenameは

    string.match(filename, '^[%w_%-]+%.%w+$')

にマッチしなければいけません。マッチしない場合はファイルオープンに失敗するなどのエラーが発生します。

以下のfilenameはマッチします。

- hoge-fuga_foo.bar
- 20140401-log.1

以下のfilenameはマッチしません。

- ./file.txt
- dir/foo.txt
- bar

なお、これらの関数で指定したファイルパスはAIスクリプトのある場所からの相対パスとして解釈されます。
したがって、これらの関数はAIスクリプトのあるフォルダ内のファイルへのみアクセスできます。

また、dofileおよびloadfileでバイトコードをロードすることはできません。

##API

花映塚のゲーム情報を取得したり、キー入力を送信するための関数やオブジェクトがAIスクリプトへ提供されます。

提供されるものとしては以下のものがあります。

- 関数
    - [sendKeys](#sendKeys)
    - [hitTest](#hitTest)
- グローバル変数
    - [game_sides](#game_sides)
    - [player_side](#player_side)
    - [difficulty](#difficulty)
    - [round](#round)
- 定数群
    - [ExAttackType](#ExAttackType).*
    - [ItemType](#ItemType).*
    - [HitType](#HitType).*
    - [CharacterType](#CharacterType).*

### 関数

#### sendKeys(key_state) {#sendKeys}

現在プレイしている自機に対するキー入力を花映塚へ送ります。
`key_state`には整数値を指定し、各ビットがそれぞれどのキーを押すかを表します。

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

#### hitTest(hitbody_a, hitbody_b) {#hitTest}

2つの[HitBody](#HitBody)オブジェクト`hitbody_a`と`hitbody_b`の当たり判定処理を行います。
`hitbody_a`と`hitbody_b`とがぶつかっている時はtrueを、そうでなければfalseを返します。

### グローバル変数

####game_sides {#game_sides}
  
2つの[GameSide](#GameSide) からなる配列で、`game_sides[1]`は1P側の、`game_sides[2]`は2P側のゲーム状態を表します。
このグローバル変数は毎フレーム更新されます。

この変数を経由して得られるすべての値は次フレームで破壊的に変更される可能性があります。
例えばある時点で`game_sides[1].enemies[1]`が敵Aを表していたとして、
次のフレームでは敵Bのデータに書き換えられている可能性があります。
そのため`game_sides`のフィールドへの参照を保存して次フレームで参照するのは*非推奨*です。
ある時点でのデータを次フレームに渡したい場合は深いコピーを行うなどして下さい。

####player_side {#player_side}
  
この変数はこのAIスクリプトが1P側か2P側かを表します。
1P側ならば

    player_side == 1

2P側ならば

    player_side == 2

です。

####difficulty {#difficulty}

この変数は現在のゲームの難易度を表します。

------------------------
難易度    difficultyの値
--------- --------------
Easy       1

Normal     2

Hard       3

Lunatic    4

Extra      5

------------------------
  
####round {#round}

この変数は対戦開始からの経過ラウンド数を表します。

### オブジェクト

####GameSide {#GameSide}

GameSideオブジェクトは対戦の各サイドのゲーム状態を表します。
このオブジェクトは以下のフィールドを持ちます。

GameSide.player
:   [Player](#Player) オブジェクトを持つフィールドで、自機に関する情報を表します。
GameSide.enemies
:   [Enemy](#Enemy) オブジェクトの配列で、画面上の敵に関する情報を表します。
GameSide.bullets
:   [Bullet](#Bullet) オブジェクトの配列で、画面上の敵弾に関する情報を表します。
GameSide.exAttacks
:   [ExAttack](#ExAttack) オブジェクトの配列で、画面上のEXアタックに関する情報を表します。
GameSide.items
:   [Item](#Item) オブジェクトの配列で、画面上のアイテムに関する情報を表します。
GameSide.round_win
:   現在の対戦で勝利したラウンド数を表します。
GameSide.score
:   現在のスコアを表します。

####Player {#Player}

Playerオブジェクトは自機に関する情報を表します。
このオブジェクトは以下のフィールドを持ちます。
  
Player.x / Player.y
:   自機の位置を表します。
Player.life
:   残りライフを表します。
Player.cardAttackLevel
:   現在のカードアタックレベルを表します。
Player.bossAttackLevel
:   現在のボスカードアタックレベルを表します。
Player.currentCharge / Player.currentChargeMax
:   currentChargeは現在のチャージゲージのチャージ量、currentChargeMaxは現在チャージゲージがチャージできる最大値を表します。
    チャージ量は0~400までの浮動小数点数で表されます。
Player.chargeSpeed
:   チャージゲージのチャージ速度を表します。
    チャージ速度は1フレームあたりにチャージされる量で表されます。
Player.spellPoint
:   現在のスペルポイントを表します。
Player.combo
:   現在のコンボ数を表します。
Player.character
:   自機キャラクターの種別を表します。
    [CharacterType](#CharacterType) の定数のいずれかの値を取ります。
Player.speedFast
:   高速移動時の自機の速さを表します。
Player.speedSlow
:   低速移動時の自機の速さを表します。
Player.hitBodyRect / Player.hitBodyCircle / Player.hitBodyForItem
:   これらはいずれも[HitBody](#HitBody)オブジェクトで、自機の当たり判定を表します。
    弾や敵、EXアタックとの当たり判定処理にはhitBodyRectまたはhitBodyCircleを使用します。
    hitBodyRectは矩形の当たり判定、hitBodyCircleは円形の当たり判定です。
    一方、hitBodyForItemはアイテムとの当たり判定処理で使用する当たり判定です。

####Enemy    {#Enemy}

Enemyオブジェクトは個々の敵に関する情報を表します。
このオブジェクトは以下のフィールドを持ちます。

Enemy.id
:   それぞれの敵ごとに1つずつ付与されるIDです。
    ある敵について、その敵に対応するEnemyオブジェクトは必ず同じidを持ちます。
Enemy.x / Enemy.y
:   敵の位置を表します。
Enemy.vx / Enemy.vy
:   現在の速度を表します。
Enemy.enabled
:   現在生きている敵かどうかをブール値で表します。

    このフィールドは以前の設計において必要だったのが惰性で残った感じの代物で、今後廃止される恐れがあります。
Enemy.isSpirit
:   この敵が幽霊かどうかをブール値で表します。
Enemy.isActivatedSpirit
:   この敵が吸霊によって活性化した幽霊かどうかをブール値で表します。
Enemy.isBoss
:   この敵がボスかどうかをブール値で表します。
Enemy.isLily
:   この敵がリリー・ホワイト(またはブラック)かどうかをブール値で表します。
Enemy.isPseudoEnemy
:   この敵が擬似的な敵かどうかをブール値で表します。

    擬似的な敵とは弾幕の設置のためだけに存在する(多くの場合不可視な)敵のことです。
    たとえば咲夜さんのC2やC3でナイフが配置される際には、擬似的な敵が出現しナイフを配置しています。

Enemy.hitBody
:   敵の当たり判定を表す[HitBody](#HitBody)オブジェクトです。
    この当たり判定は敵vs自機の衝突判定で用いるものです。

####Bullet   {#Bullet}

Bulletオブジェクトは個々の弾やレーザーに関する情報を表します。
EXアタックによって発生する弾のいくつかについては[ExAttack](#ExAttack)オブジェクトとして扱われます。
Bulletオブジェクトは以下のフィールドを持ちます。

Bullet.id
:   それぞれの弾ごとに1つずつ付与されるIDです。
    ある弾について、その弾に対応するBulletオブジェクトは必ず同じidを持ちます。
Bullet.x / Bullet.y
:   弾の位置を表します。
Bullet.vx / Bullet.vy
:   現在の速度を表します。
Bullet.enabled
:   現在有効な弾かどうかをブール値で表します。

    このフィールドは以前の設計において必要だったのが惰性で残った感じの代物で、今後廃止される恐れがあります。
Bullet.hitBody
:   弾の当たり判定を表す[HitBody](#HitBody)オブジェクトです。
    この当たり判定は弾vs自機の衝突判定で用いるものです。

####ExAttack {#ExAttack}

####Item     {#Item}

Itemオブジェクトは画面中のアイテムに関する情報を表します。
このオブジェクトは以下のフィールドを持ちます。

Item.id
:   それぞれのアイテムごとに1つずつ付与されるIDです。
Item.x / Item.y
:   アイテムの位置を表します。
Item.vx / Item.vy
:   現在の速度を表します。
Item.type
:   アイテムの種別を表します。
    [ItemType](#ItemType)の定数のいずれかの値を取ります。
Item.enabled
:   現在有効なアイテムかどうかをブール値で表します。

    このフィールドは以前の設計において必要だったのが惰性で残った感じの代物で、今後廃止される恐れがあります。
Item.hitBody
:   アイテムの当たり判定を表す[HitBody](#HitBody)オブジェクトです。

####HitBody  {#HitBody}

### 定数群

##雑多

