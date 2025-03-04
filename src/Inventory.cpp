#include "Inventory.h"

void Inventory::addWeapon(const Weapon& weapon) {
    weapons.push_back(weapon);
}

void Inventory::listItems() {
    std::cout << "Inventario:\n";
    for (const auto &w : weapons) {
        std::cout << "  " << w.name << " (Daño: " << w.damage << ")\n";
    }
}