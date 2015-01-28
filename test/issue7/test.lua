-- test.lua
local testl = {{0x20}}

function test1()
  local m = {}
  for i=1,3 do
    local l = {0x10}
    m[i] = l
  end
  testl = m
end

function test2()
  for i,x in ipairs(testl) do
    return x[1]
  end
end

