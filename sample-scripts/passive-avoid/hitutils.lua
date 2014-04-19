do
  local HittableObject = {
    HIT_RECT = 0,
    HIT_CIRCLE = 1,
    HIT_ROTATABLE_RECT = 2
  };
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
      if ex_attack.hitBody.type == HittableObject.HIT_CIRCLE then
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
      --if hit_body.type == HittableObject.HIT_CIRCLE then
      --  return HittableCircle(hit_body.x, hit_body.y, hit_body.radius);
      --elseif hit_body.type == HittableObject.HIT_RECT then
      --  return HittableRect(hit_body.x, hit_body.y, hit_body.width, hit_body.height);
      --else
      --  return HittableRotatableRect(hit_body.x, hit_body.y, hit_body.width, hit_body.height, hit_body.angle);
      --end
    end
  }
end
