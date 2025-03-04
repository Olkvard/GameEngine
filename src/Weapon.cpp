#include "Weapon.h"

Weapon::Weapon(const std::string& name, int damage, WeaponType type)
    : name(name), damage(damage), type(type) {}