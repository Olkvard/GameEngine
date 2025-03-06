#include "Weapon.h"

Weapon::Weapon(const std::string& name, int damage, WeaponType type,
               Uint32 cooldown, float alcance, const std::string& quality, int level,
               int x, int y, int w, int h)
    : Item(x, y, w, h, name),
      damage(damage),
      type(type),
      cooldown(cooldown),
      alcance(alcance),
      quality(quality),
      level(level)
{}