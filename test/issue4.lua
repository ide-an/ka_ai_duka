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

local elapsed_times = {}
function main()
  local opponent_side = 2
  if player_side == 2 then
    opponent_side = 1
  end
  local opponent_character = game_sides[opponent_side].player.character
  assertEqual((opponent_character == CharacterType.Medicine) or (opponent_character == CharacterType.Eiki), true, "opponent is medicine or eiki")
  for i, exattack in ipairs(game_sides[player_side].exAttacks) do
    if elapsed_times[exattack.id] == nil then
      elapsed_times[exattack.id]  = 0
    else
      elapsed_times[exattack.id]  = elapsed_times[exattack.id] + 1
    end
    if elapsed_times[exattack.id] > 200 then -- ignore while ex attack is moving
      assertEqual(exattack.vx, 0, "vx is 0.0f")
      assertEqual(exattack.vy, 0, "vy is 0.0f")
    end
  end
end

