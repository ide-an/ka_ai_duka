f = io.open('log.js', 'w')
f:write('var l=[];\n')
f:write('l.p=function(v){this.push(v)};\n')

function main()
  local player = game_sides[player_side].player
  f:write('l.p({x:'..tostring(player.x)..',y:'..tostring(player.y)..'});\n')
end
