#include "Weapon.h"

Weapon::Weapon(const std::string& name, int damage, WeaponType type, int x, int y, int w, int h)
    : Item(x, y, w, h, name), damage(damage), type(type)
{}