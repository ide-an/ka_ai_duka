local enable_time_logging = true;
local keyutils = dofile("keyutils.lua");
local hitutils = dofile("hitutils.lua");

-- 10ƒtƒŒ[ƒ€æ‚Ü‚Å—\‘ª‚µ‚½‚¢Š‘¶
local predict_frame = 2;

local fp;
if (enable_time_logging) then
  fp = io.open(os.date("%Y%m%d%H%M%S").."-"..tostring(player_side).."p.txt","w");
end
local count = 0;

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
    cost=0
  });
  for i=1,4 do
    table.insert(candidates, {
      vx= dx[i] * speed_fast,
      vy= dy[i] * speed_fast,
      keys={keys[i]},
      cost=0
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
  --for i,c in ipairs(candidates) do
  --  fp:write(tostring(c.cost)..":" .. tostring(c.keys[1]) .. "\n")
  --end
  return candidates[1].keys;
end

function calculateHitCost(player, elements, hit_body_for_filter_circle, hit_body_for_filter_rect, candidates, hittest_func)
  local px = player.x;
  local py = player.y;
  for elm in elements do
    if hitTest(hit_body_for_filter_circle, elm.hitBody) or hitTest(hit_body_for_filter_rect, elm.hitBody) then
      local ex = elm.x;
      local ey = elm.y;
      local evx = elm.vx;
      local evy = elm.vy;
      for frame=1,predict_frame do
        elm.hitBody.x = ex + evx * frame;
        elm.hitBody.y = ey + evy * frame;
        for i,c in ipairs(candidates) do
          count = count + 1;
          player.hitBodyRect.x = px + c.vx * frame;
          player.hitBodyRect.y = py + c.vy * frame;
          player.hitBodyCircle.x = px + c.vx * frame;
          player.hitBodyCircle.y = py + c.vy * frame;
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
  -- time logging
  local time_start;
  if enable_time_logging then
    time_start = os.clock();
  end

  local my_side = game_sides[player_side];
  local player = my_side.player;

  -- generate candidates
  local candidates = generateCandidates(player);
  -- —\‘ª‚·‚é’e‚â“G‚ği‚é‚½‚ß‚Ì“–‚½‚è”»’èB‚±‚Ì“–‚½‚è”»’è‚ÆÕ“Ë‚µ‚È‚¢‚à‚Ì‚É‚Â‚¢‚Ä‚Í‹““®—\‘ª‚ğs‚í‚È‚¢
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
    fp:write(tostring(time_end - time_start).."\n");
  end
  fp:write("count:"..tostring(count).."\n");
end

