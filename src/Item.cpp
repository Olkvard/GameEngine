#include "Item.h"
#include <SDL2/SDL.h>

Item::Item(int x, int y, int w, int h, const std::string& name)
    : name(name), collected(false)
{
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

void Item::render(SDL_Renderer* renderer) {
    if (collected) return;
    // Renderizamos el ítem como un rectángulo verde (se puede sustituir por un sprite)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}