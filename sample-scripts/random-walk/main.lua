-- License: CC0 1.0 Universal ( http://creativecommons.org/publicdomain/zero/1.0/legalcode )
-- 

local keyutils = dofile("keyutils.lua");

-- debug only
local enable_time_logging = true; -- trueにすると毎フレームの経過時間のログを取る
local fp;
if (enable_time_logging) then
  fp = io.open(os.date("%Y%m%d%H%M%S").."-"..tostring(player_side).."p.txt","w");
end

local function generateCandidates()
  local candidates = {};
  local keys = { "up", "right", "down", "left", "up" };
  -- stop
  table.insert(candidates, {
    keys={}
  });
  for i=1,4 do
    table.insert(candidates, {
      keys={keys[i]},
    });
    table.insert(candidates, {
      keys={keys[i], "shift"}
    });
    table.insert(candidates, {
      keys={keys[i], keys[i+1]}
    });
    table.insert(candidates, {
      keys={keys[i], keys[i+1], "shift"}
    });
  end
  return candidates;
end

local function choice(candidates)
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
  local candidates = generateCandidates();
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
    fp:write(tostring(time_end - time_start)..","..tostring(round).."\n");
  end
end
