-- issue 10: save snapshot from AI script
-- This AI saves 2 snapshots every 10 seconds.
local frame = 0
function main()
  frame = frame + 1
  if frame%600 <= 1 then
    saveSnapshot()
  end
end
