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
  test_name = "issue9"
  for i, enemy in ipairs(game_sides[player_side].enemies) do
    if enemy.isLily then
      assertEqual(enemy.isBoss, false, "boss is not Lily")
    end
  end
end

