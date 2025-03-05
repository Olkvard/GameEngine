#ifndef WEAPON_H
#define WEAPON_H

#include "Item.h"
#include <string>

enum class WeaponType { Melee, Ranged };

class Weapon : public Item {
public:
    // x,y,w,h se usan para posicionar el ítem en el mapa (para pickups) o 0 si no se renderiza como ítem.
    Weapon(const std::string& name, int damage, WeaponType type, int x = 0, int y = 0, int w = 0, int h = 0);
    
    int damage;
    WeaponType type;
};

#endif