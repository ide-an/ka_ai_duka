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
      if ex_attack.hitBody.type == HittableObject.HIT_CIRCLE then
        return hitTest(player.hitBodyCircle, ex_attack.hitBody);
      else
        return hitTest(player.hitBodyRect, ex_attack.hitBody);
      end
    end
  }
end
