#ifndef INVENTORY_H
#define INVENTORY_H

#include "Weapon.h"
#include <vector>
#include <iostream>

class Inventory {
public:
    void addWeapon(const Weapon& weapon);
    void listItems();
    
private:
    std::vector<Weapon> weapons;
};

#endif