% 花AI塚ver 1.7 AIスクリプト ドキュメント

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
- packageモジュールのすべて
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
- debugモジュールのすべて

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
    - [saveSnapshot](#saveSnapshot)
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

#### saveSnapshot() {#saveSnapshot}

呼び出すと、呼び出したフレームでのスナップショットを保存します。保存先は花映塚のデフォルトの保存先です。
この関数は設定で「スクリプトからのスナップショット取得を許可」を有効にしたときだけ使用できます。

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

-----------------------
難易度    difficultyの値
-------- --------------
Easy        0

Normal      1

Hard        2

Lunatic     3

Extra       4

-----------------------

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
GameSide.roundWin
:   現在の対戦で勝利したラウンド数を表します。
GameSide.score
:   現在のスコアを表します。
GameSide.chargeType
:   現在のチャージタイプを表します。
    [ChargeType](#ChargeType) の定数のいずれかの値を取ります。

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
Bullet.isErasable
:   敵撃破時の爆風で消せる弾かどうかをブール値で表します。
Bullet.hitBody
:   弾の当たり判定を表す[HitBody](#HitBody)オブジェクトです。
    この当たり判定は弾vs自機の衝突判定で用いるものです。

####ExAttack {#ExAttack}

ExAttackオブジェクトはEXアタックによって発生する弾やメディの毒霧、四季映姫のなんかもやもやのやつなどに関する情報を表します。

EXアタックによって発生する弾のすべてがExAttackオブジェクトで表されるとは限りません。
たとえば小野塚小町のEXアタックでは個々の弾はBulletオブジェクトで表され、弾源のみがExAttackオブジェクトで表されます。

ExAttackオブジェクトは以下のフィールドを持ちます。

ExAttack.id
:   それぞれのEXアタックの弾ごとに1つずつ付与されるIDです。
    ある弾について、その弾に対応するExAttackオブジェクトは必ず同じidを持ちます。
ExAttack.x / ExAttack.y
:   EXアタックの位置を表します。
ExAttack.vx / ExAttack.vy
:   現在の速度を表します。

    **BUG:** 1P/2P間をまたぐ間、速度が正しく取得できない(咲夜さんのEXアタックのみ取得可)。
ExAttack.type
:   EXアタックの種別を表します。
    [ExAttackType](#ExAttackType) の定数のいずれかの値を取ります。
ExAttack.enabled
:   現在有効なEXアタックかどうかをブール値で表します。

    このフィールドは以前の設計において必要だったのが惰性で残った感じの代物で、今後廃止される恐れがあります。
ExAttack.hittable
:   有効な当たり判定を持つかどうかをブール値で表します。

    EXアタックは必ずしも有効な当たり判定を持つとは限りません(特に弾源としてのみ機能する場合)。
ExAttack.hitBody
:   EXアタックの当たり判定を表す[HitBody](#HitBody)オブジェクトです。
    この当たり判定はEXアタックvs自機の衝突判定で用いるものです。

    **注意** `hittable = false`のときはhitBodyが`nil`であるケースがあります。

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

HitBodyオブジェクトは当たり判定に関する情報を表します。
このオブジェクトは以下のフィールドを持ちます。

HitBody.type
:   この当たり判定の種別を表します。
    [HitType](#HitType) の定数のいずれかの値を取ります。
HitBody.x / HitBody.y
:   この当たり判定の現在の位置を表します。
HitBody.width
:   当たり判定の幅を表します。
HitBody.height
:   当たり判定の高さを表します。
HitBody.radius
:   `type = HitType.Circle`のとき、当たり判定の半径を表します。
HitBody.angle
:   `type = HitType.RotatableRect`のとき、当たり判定の回転角を表します。

HitBody.typeの値によって、当たり判定処理のときにどのフィールドを用いるかが異なります。

-----------------------------------------------------------------
HitBody.type           x    y    width   height   radius   angle
--------------------- ---- ---- ------- -------- -------- -------
HitType.Rect           ◯    ◯     ◯      ◯

HitType.Circle         ◯    ◯                      ◯

HitType.RotatableRect  ◯    ◯     ◯      ◯                ◯

-----------------------------------------------------------------

それぞれの種別の当たり判定の形状を図示すると以下のようになります。

![当たり判定の形状一覧](res/hittypes.png)

### 定数群

####ExAttackType {#ExAttackType}

EXアタックの種別を表す定数群です。

---------------------------- ----------------------------------------------
ExAttackType.Reimu           霊夢のEXアタック(陰陽玉)
ExAttackType.Marisa          魔理沙のEXアタック(レーザーを発するビット)
ExAttackType.Sakuya          咲夜さんのEXアタック(ナイフ)
ExAttackType.Youmu           妖夢のEXアタック(なんか設置されるやつ)
ExAttackType.Reisen          鈴仙のEXアタック(膨張する丸弾)
ExAttackType.Cirno           チルノのEXアタック(つらら)
ExAttackType.Lyrica          リリカのEXアタック(弾源)
ExAttackType.Merlin          メルランのEXアタック(弾源)
ExAttackType.Lunasa          ルナサのEXアタック(弾源)
ExAttackType.Mystia_Ex       ミスティアのEXアタック(鳥型弾源)
ExAttackType.Mystia_Charge2  ミスティアのC2(鳥型弾源)
ExAttackType.Mystia_Charge3  ミスティアのC3(鳥型弾源)
ExAttackType.Mystia_Boss1    ミスティアのボスアタック時の弾(鳥型弾源)
ExAttackType.Mystia_Boss2    ミスティアのボスアタック時の弾(鳥型弾源)
ExAttackType.Tewi            てゐのEXアタック(丸い跳ね返るやつ)
ExAttackType.Aya             射命丸のEXアタック(高速丸弾)
ExAttackType.Medicine        メディスンのEXアタック(毒霧)
ExAttackType.Yuuka           幽香のEXアタック(向日葵)
ExAttackType.Komachi         小町のEXアタック(弾源)
ExAttackType.Eiki            映姫のEXアタック(弾幕の罪を表面化するもやもや)
---------------------------- ----------------------------------------------

####ItemType {#ItemType}

アイテムの種別を表す定数群です。

-------------------- ----
ItemType.G           G
ItemType.Bullet      弾
ItemType.Ex          Ex
ItemType.Score       点
-------------------- ----

####HitType {#HitType}

当たり判定の種別を表す定数群です。

---------------------- ----------------
HitType.Rect           矩形
HitType.Circle         円形
HitType.RotatableRect  回転可能な矩形
---------------------- ----------------

####CharacterType {#CharacterType}

自機のキャラクター種別を表す定数群です。

----------------------- -------------------------
CharacterType.Reimu     博麗霊夢
CharacterType.Marisa    霧雨魔理沙
CharacterType.Sakuya    十六夜咲夜
CharacterType.Youmu     魂魄妖夢
CharacterType.Reisen    鈴仙・優曇華院・イナバ
CharacterType.Cirno     チルノ
CharacterType.Lyrica    リリカ・プリズムリバー
CharacterType.Mystia    ミスティア・ローレライ
CharacterType.Tewi      因幡てゐ
CharacterType.Yuuka     風見幽香
CharacterType.Aya       射命丸文
CharacterType.Medicine  メディスン・メランコリー
CharacterType.Komachi   小野塚小町
CharacterType.Eiki      四季映姫・ヤマザナドゥ
CharacterType.Merlin    メルラン・プリズムリバー
CharacterType.Lunasa    ルナサ・プリズムリバー
----------------------- -------------------------

####ChargeType {#ChargeType}

チャージタイプを表す定数群です。

------------------ -----------------------------------
ChargeType.Slow     Z長押しでチャージ、Shiftで低速
ChargeType.Charge   Shiftでチャージ、Z長押しで低速
------------------ -----------------------------------
##雑多

###座標系について

自機の位置や弾の速度など、ゲームに関する位置情報はすべて以下の座標系でのものです。

![ゲーム状態に関する座標系](res/axis.png)

この座標系はY軸が下向きかつ画面上部でYが0であり、X軸が右向きかつ各陣の画面中央でXが0である座標系です。

###当たり判定処理について

花映塚において当たり判定処理は

- 矩形 vs 矩形
- 円形 vs 円形
- 矩形 vs 回転可能な矩形

のいずれかを行うことで実現しています。
[Player](#Player)オブジェクトで述べたように、自機には円形と矩形の両方の当たり判定がありますが、
円形の当たり判定(Player.hitBodyCircle)が用いられるのは円形の当たり判定との当たり判定処理を行うときだけで、それ以外の当たり判定では矩形の当たり判定を用います。

###Tips

- `require`は使用できませんが、`dofile`が使用できるのでこれを用いてスクリプトファイルを分割することができます。
- [Enemy](#Enemy)や[Bullet](#Bullet)、[Item](#Item)や[ExAttack](#ExAttack)が持つ`id`フィールドはこれらのオブジェクト全体においてユニークです。例えば、あるEnemyオブジェクトと別のあるBulletオブジェクトが同じidを持つ、ということは起こりません。
- ラウンドの切り替わりを通知するイベントハンドラはありませんが、グローバル変数[round](#round)の値を監視することでラウンドの切り替わりを検知することができます。同様にGameSide.player.lifeを監視することで被弾を検知することができます。
