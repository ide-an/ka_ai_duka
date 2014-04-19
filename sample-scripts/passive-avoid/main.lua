local enable_time_logging = true;
local keyutils = dofile("keyutils.lua");
local hitutils = dofile("hitutils.lua");

-- 10ÉtÉåÅ[ÉÄêÊÇ‹Ç≈ó\ë™ÇµÇΩÇ¢èäë∂
local predict_frame = 10;

local fp;
if (enable_time_logging) then
  fp = io.open(os.date("%Y%m%d%H%M%S").."-"..tostring(player_side).."p.txt","w");
end
local count = 0;
local item_count = 0;

local function positionCost(player, dx, dy)
  local cost = 0;
  local cx, cy = 0, 300;
  if (cx - player.x) * dx <= 0 then
    cost = cost + 0.0001;
  end
  if (cy - player.y) * dy <= 0 then
    cost = cost + 0.0001;
  end
  return cost;
end

local function generateCandidates(player)
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
    cost=positionCost(player, 0, 0)
  });
  for i=1,4 do
    table.insert(candidates, {
      vx= dx[i] * speed_fast,
      vy= dy[i] * speed_fast,
      keys={keys[i]},
      cost=positionCost(player, dx[i], dy[i])
    });
  end
  return candidates;
end

local function filter(tbl, pred)
  local new_tbl = {};
  for i,v in ipairs(tbl) do
    if pred(v) then
      table.insert(new_tbl, v);
    end
  end
  return new_tbl;
end

local function choice(candidates)
  table.sort(candidates, function(a,b) return a.cost < b.cost end);
  return candidates[1].keys;
end

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
    item_count = item_count + 1;
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
          count = count + 1;
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

function main ()
  count = 0;
  item_count = 0;
  -- time logging
  local time_start;
  if enable_time_logging then
    time_start = os.clock();
  end

  local my_side = game_sides[player_side];
  local player = my_side.player;

  -- generate candidates
  local candidates = generateCandidates(player);
  -- ó\ë™Ç∑ÇÈíeÇ‚ìGÇçiÇÈÇΩÇﬂÇÃìñÇΩÇËîªíËÅBÇ±ÇÃìñÇΩÇËîªíËÇ∆è’ìÀÇµÇ»Ç¢Ç‡ÇÃÇ…Ç¬Ç¢ÇƒÇÕãììÆó\ë™ÇçsÇÌÇ»Ç¢
  local hit_body_for_filter_circle = hitutils.copy(player.hitBodyCircle,fp);
  hit_body_for_filter_circle.radius = 50;
  local hit_body_for_filter_rect = hitutils.copy(player.hitBodyRect,fp);
  hit_body_for_filter_rect.width = 100;
  hit_body_for_filter_rect.height = 100;
  -- hittest
  calculateHitCost(
    player,
    my_side.enemies,
    hit_body_for_filter_circle,
    hit_body_for_filter_rect,
    candidates,
    hitutils.playerVsEnemy);
  calculateHitCost(
    player,
    my_side.bullets,
    hit_body_for_filter_circle,
    hit_body_for_filter_rect,
    candidates,
    hitutils.playerVsBullet);
  calculateHitCost(
    player,
    my_side.exAttacks,
    hit_body_for_filter_circle,
    hit_body_for_filter_rect,
    candidates,
    hitutils.playerVsExAttack);
  -- choice
  local keys_to_send = choice(candidates);

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
  --fp:write("count:"..tostring(count).."\n");
end

