-- License: CC0 1.0 Universal ( http://creativecommons.org/publicdomain/zero/1.0/legalcode )
-- 

do
  return {
    playerVsEnemy=function(player, enemy)
      return hitTest(player.hitBodyRect, enemy.hitBody);
    end,
    playerVsBullet=function(player, bullet)
      return hitTest(player.hitBodyRect, bullet.hitBody);
    end,
    playerVsExAttack=function(player, ex_attack)
      if not(ex_attack.hittable) then
        return false;
      end
      if ex_attack.hitBody.type == HitType.CIRCLE then
        return hitTest(player.hitBodyCircle, ex_attack.hitBody);
      else
        return hitTest(player.hitBodyRect, ex_attack.hitBody);
      end
    end,
    copy=function(hit_body,fp)
      local obj = {};
      for k,v in pairs(hit_body) do
        obj[k] = v;
      end
      return obj;
    end
  }
end
