local enable_time_logging = true;
local keyutils = dofile("keyutils.lua");
local hitutils = dofile("hitutils.lua");

local fp;
if (enable_time_logging) then
  fp = io.open(os.date("%Y%m%d%H%M%S").."-"..tostring(player_side).."p.txt","w");
end

local function generateCandidates(player)
  local candidates = {};
  local keys = { "up", "right", "down", "left", "up" };
  local dx = { 0, 1, 0, -1, 0 };
  local dy = { -1, 0, 1, 0, -1 };
  local px = player.x;
  local py = player.y;
  local speed_fast = player.speedFast;
  local speed_slow = player.speedSlow;
  local speed_fast_div_sqrt2 = speed_fast / math.sqrt(2);
  local speed_slow_div_sqrt2 = speed_slow / math.sqrt(2);
  -- stop
  table.insert(candidates, {
    x=px,
    y=py,
    keys={},
    hit=0
  });
  for i=1,4 do
    table.insert(candidates, {
      x=px + dx[i] * speed_fast,
      y=py + dy[i] * speed_fast,
      keys={keys[i]},
      hit=0
    });
    ---- slow
    --table.insert(candidates, {
    --  x=px + dx[i] * speed_slow,
    --  y=py + dy[i] * speed_slow,
    --  keys={keys[i], "shift"},
    --  hit=0
    --});
    ---- ŽÎ‚ß
    --table.insert(candidates, {
    --  x=px + (dx[i] + dx[i+1]) * speed_fast_div_sqrt2,
    --  y=py + (dy[i] + dy[i+1]) * speed_fast_div_sqrt2,
    --  keys={keys[i], keys[i+1]},
    --  hit=0
    --});
    ---- ŽÎ‚ß slow
    --table.insert(candidates, {
    --  x=px + (dx[i] + dx[i+1]) * speed_slow_div_sqrt2,
    --  y=py + (dy[i] + dy[i+1]) * speed_slow_div_sqrt2,
    --  keys={keys[i], keys[i+1], "shift"},
    --  hit=0
    --});
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
  table.sort(candidates, function(a,b) return a.hit > b.hit end);
  local min_hit = candidates[1].hit;
  candidates = filter(candidates, function(v) return v.hit == min_hit end);
  --fp:write(tostring(#candidates)..":"..candidates[2].keys[1].."\n")
  return candidates[math.random(#candidates)].keys;
end

function main ()
  -- time logging
  local time_start;
  if enable_time_logging then
    time_start = os.clock();
  end

  local my_side = game_sides[player_side];
  local player = my_side.player;

  -- generate candidates
  local candidates = generateCandidates(player);
  -- hittest
  for enemy in my_side.enemies do
    enemy.hitBody.x = enemy.x + enemy.vx;
    enemy.hitBody.y = enemy.y + enemy.vy;
    for i,c in ipairs(candidates) do
      player.hitBodyRect.x = c.x;
      player.hitBodyRect.y = c.y;
      if hitutils.playerVsEnemy(player, enemy) then
        c.hit = c.hit + 1;
      end
    end
  end
  for bullet in my_side.bullets do
    bullet.hitBody.x = bullet.x + bullet.vx;
    bullet.hitBody.y = bullet.y + bullet.vy;
    for i,c in ipairs(candidates) do
      player.hitBodyRect.x = c.x;
      player.hitBodyRect.y = c.y;
      if hitutils.playerVsBullet(player, bullet) then
        c.hit = c.hit + 1;
      end
    end
  end
  for ex_attack in my_side.exAttacks do
    if ex_attack.hittable then
      ex_attack.hitBody.x = ex_attack.x + ex_attack.vx;
      ex_attack.hitBody.y = ex_attack.y + ex_attack.vy;
      for i,c in ipairs(candidates) do
        player.hitBodyRect.x = c.x;
        player.hitBodyRect.x = c.x;
        player.hitBodyRect.y = c.y;
        if hitutils.playerVsExAttack(player, ex_attack) then
          c.hit = c.hit + 1;
        end
      end
    end
  end
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
    fp:write(tostring(time_end - time_start).."\n");
  end
end
