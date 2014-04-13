local enable_time_logging = true;
local keyutils = dofile("keyutils.lua");

local fp;
if (enable_time_logging) then
  fp = io.open(os.date("%Y%m%d%H%M%S").."-"..tostring(player_side).."p.txt","w");
end

local function generateCandidates(player)
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
  fp:write("hittype:"..tostring(player.hitBodyCircle.type).."\n");

  -- generate candidates
  local candidates = generateCandidates(player);
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
