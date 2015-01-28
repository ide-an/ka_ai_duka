-- issue 7のテストコード。このコードがエラーなく動けばおｋ。
-- main.lua
dofile("test.lua")

local count = 0
local roundCount = 1
local prevLife = 10

local pastPlayer = {}

function main()
  i = 0
  local ownSide = game_sides[player_side]
  local player = ownSide.player
  local tempKey = 0x00

  count = count + 1

  if(count==1) then
    test1()
  end
  sendKeys(test2())
end
