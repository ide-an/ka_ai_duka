local test_name = ""
local function assertEqual(actual, expect, msg)
  if not msg then
    msg = ""
  end
  if actual ~= expect then
    msg = test_name.." : expect ="..tostring(expect).." but actual = "..tostring(actual).." : "..msg
    error(msg)
  end
end

function main()
  local player = game_sides[player_side].player
  assertEqual(player.hitBodyRect.width < player.hitBodyForItem.width, true, "hit rect is smaller than hit body for item");
  assertEqual(player.hitBodyRect.height < player.hitBodyForItem.height, true, "hit rect is smaller than hit body for item");
end
