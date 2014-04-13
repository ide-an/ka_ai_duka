do
  local key_mask = {
    right  = 2 ^ 7,
    left   = 2 ^ 6,
    down   = 2 ^ 5,
    up     = 2 ^ 4,
    shift  = 2 ^ 2,
    x      = 2 ^ 1,
    z      = 2 ^ 0
  };
  local function newstate()
    return {
      z=false,
      x=false,
      right=false,
      left=false,
      up=false,
      down=false,
      shift=false,
    }
  end
  return {
    newstate=newstate,
    send=function(state)
      local keynum = 0;
      for i,v in pairs(state) do
        if v then
          keynum = keynum + key_mask[i];
        end
      end
      sendKeys(keynum)
    end
  };
end
