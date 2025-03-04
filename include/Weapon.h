#ifndef WEAPON_H
#define WEAPON_H

#include <string>

enum class WeaponType { Melee, Ranged };

class Weapon {
public:
    Weapon(const std::string& name, int damage, WeaponType type);
    
    std::string name;
    int damage;
    WeaponType type;
};

#endif