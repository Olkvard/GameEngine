#include "WeaponPickup.h"

WeaponPickup::WeaponPickup(int x, int y, int w, int h, const Weapon& weapon)
    : weapon(weapon), pickedUp(false)
{
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

void WeaponPickup::render(SDL_Renderer* renderer) {
    if (pickedUp) return;
    // Representamos el pickup como un rectángulo azul.
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}