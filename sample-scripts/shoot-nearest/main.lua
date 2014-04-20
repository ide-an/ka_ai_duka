-- License: CC0 1.0 Universal ( http://creativecommons.org/publicdomain/zero/1.0/legalcode )
-- 

local keyutils = dofile("keyutils.lua");
local hitutils = dofile("hitutils.lua");

-- 10フレーム先まで予測したい所存
local predict_frame = 10;

-- debug only
local enable_time_logging = true; -- trueにすると毎フレームの経過時間などのログを取る
local count = 0;
local item_count = 0;
local fp;
if (enable_time_logging) then
  fp = io.open(os.date("%Y%m%d%H%M%S").."-"..tostring(player_side).."p.txt","w");
  fp:write("elapsed time[sec/frame],hittest count,number of objects\n");
end
--

local function findNearestEnemy(player, enemies)
  local px = player.x;
  local nearest_enemy = nil;
  local nearest_distance = 9999;
  for i,enemy in ipairs(enemies) do
    -- 鳥霊やボスは倒しづらいので無視しとく。活性化後の鳥霊は弱いので倒しにいく。
    if not(enemy.isSpirit or enemy.isBoss) or enemy.isActivatedSpirit then
      local d = math.abs(enemy.x - px);
      if d < nearest_distance then
        nearest_distance = d;
        nearest_enemy = enemy;
      end
    end
  end
  return nearest_enemy;
end

local function positionCost(player, dx, dy, nearest_enemy)
  local cost = 0;
  local cx, cy = 0, 300;
  if nearest_enemy then
    cx = nearest_enemy.x;
  end
  if (cx - player.x) * dx <= 0 then
    cost = cost + 0.0001;
  end
  if (cy - player.y) * dy <= 0 then
    cost = cost + 0.0001;
  end
  return cost;
end

-- キー操作の候補。上下左右移動と停止のみ。今回斜め移動や低速移動は扱わない
local function generateCandidates(player, nearest_enemy)
  local candidates = {};
  local keys = { "up", "right", "down", "left"};
  local dx = { 0, 1, 0, -1 };
  local dy = { -1, 0, 1, 0 };
  local speed_fast = player.speedFast;
  -- stop
  table.insert(candidates, {
    vx=0,
    vy=0,
    keys={},
    cost=positionCost(player, 0, 0, nearest_enemy)
  });
  for i=1,4 do
    table.insert(candidates, {
      vx= dx[i] * speed_fast,
      vy= dy[i] * speed_fast,
      keys={keys[i]},
      cost=positionCost(player, dx[i], dy[i], nearest_enemy)
    });
  end
  return candidates;
end

local function choice(candidates)
  table.sort(candidates, function(a,b) return a.cost < b.cost end);
  return candidates[1].keys;
end

-- 画面外に出ないように座標を補正
local function adjustX(x)
  if x < -136 then
    return -136;
  elseif x > 136 then
    return 136
  end
  return x;
end
local function adjustY(y)
  if y < 16 then
    return 16;
  elseif y > 384 then
    return 384
  end
  return y;
end

function calculateHitCost(player, elements, hit_body_for_filter_circle, hit_body_for_filter_rect, candidates, hittest_func)
  local px = player.x;
  local py = player.y;
  local player_hit_body_rect = player.hitBodyRect;
  local player_hit_body_circle = player.hitBodyCircle;
  for idx, elm in ipairs(elements) do
    item_count = item_count + 1; -- debug
    local hit_body = elm.hitBody;
    if hit_body and (hitTest(hit_body_for_filter_circle, hit_body) or hitTest(hit_body_for_filter_rect, hit_body)) then
      local ex = elm.x;
      local ey = elm.y;
      local evx = elm.vx;
      local evy = elm.vy;
      for frame=1,predict_frame do
        hit_body.x = ex + evx * frame;
        hit_body.y = ey + evy * frame;
        for i,c in ipairs(candidates) do
          count = count + 1; -- debug
          player_hit_body_rect.x = adjustX(px + c.vx * frame);
          player_hit_body_rect.y = adjustY(py + c.vy * frame);
          player_hit_body_circle.x = adjustX(px + c.vx * frame);
          player_hit_body_circle.y = adjustY(py + c.vy * frame);
          if hittest_func(player, elm) then
            c.cost = c.cost + 0.5 ^ frame;
          end
        end
      end
    end
  end
end

local shoot_sequence = coroutine.wrap(function()
  while true do
      coroutine.yield(true);
      coroutine.yield(false);
  end
end);

local function shouldShoot(player, nearest_enemy)
  -- 敵と自機のX座標のずれが16px以内なら撃てば当たる気がする
  if nearest_enemy and (player.x - nearest_enemy.x)^2 < 16^2 then
    if player.spellPoint > 500000 then -- ポイント切っときますね
      return false;
    else
      return shoot_sequence();
    end
  end
  return false;
end

function main ()
  count = 0; -- debug
  item_count = 0; -- debug
  -- time logging
  local time_start;
  if enable_time_logging then
    time_start = os.clock();
  end

  local my_side = game_sides[player_side];
  local player = my_side.player;
  local nearest_enemy = findNearestEnemy(player, my_side.enemies);

  -- generate candidates
  local candidates = generateCandidates(player, nearest_enemy);
  -- 予測する弾や敵を絞るための当たり判定。この当たり判定と衝突しないものについては挙動予測を行わない
  local hit_body_for_filter_circle = hitutils.copy(player.hitBodyCircle,fp);
  hit_body_for_filter_circle.radius = 50;
  local hit_body_for_filter_rect = hitutils.copy(player.hitBodyRect,fp);
  hit_body_for_filter_rect.width = 100;
  hit_body_for_filter_rect.height = 100;
  -- hittest
  calculateHitCost(-- player vs enemies
    player,
    my_side.enemies,
    hit_body_for_filter_circle,
    hit_body_for_filter_rect,
    candidates,
    hitutils.playerVsEnemy);
  calculateHitCost(-- player vs bullets
    player,
    my_side.bullets,
    hit_body_for_filter_circle,
    hit_body_for_filter_rect,
    candidates,
    hitutils.playerVsBullet);
  calculateHitCost(-- player vs exAttacks
    player,
    my_side.exAttacks,
    hit_body_for_filter_circle,
    hit_body_for_filter_rect,
    candidates,
    hitutils.playerVsExAttack);
  -- choice
  local keys_to_send = choice(candidates);
  if shouldShoot(player, nearest_enemy) then
    -- 破壊的に書き換えているが、どうぜgenerateCandidatesで毎フレーム生成してるものなのでOK
    table.insert(keys_to_send, "z");
  end

  -- send keys
  local keys = keyutils.newstate()
  for i,key in ipairs(keys_to_send) do
    keys[key] = true;
  end
  keyutils.send(keys);
  -- time logging
  if enable_time_logging then
    local time_end = os.clock();
    fp:write(tostring(time_end - time_start)..","..tostring(count)..","..tostring(item_count).."\n");
  end
end

