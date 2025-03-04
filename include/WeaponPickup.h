#ifndef WEAPONPICKUP_H
#define WEAPONPICKUP_H

#include "Weapon.h"
#include <SDL2/SDL.h>

class WeaponPickup {
public:
    WeaponPickup(int x, int y, int w, int h, const Weapon& weapon);
    void render(SDL_Renderer* renderer);
    
    SDL_Rect rect;
    Weapon weapon;
    bool pickedUp;
};

#endif