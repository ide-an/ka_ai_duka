local f = io.open("log.txt", "w")
local i = 0
function main()
  local player = game_sides[player_side].player
  i = i + 1
  f:write(tostring(i) .. ":" .. tostring(player.x) .. " " .. tostring(player.y) .. "\n")
end
