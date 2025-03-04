#include "Projectile.h"

Projectile::Projectile(float x, float y, float dx, float dy, int damage)
    : x(x), y(y), dx(dx), dy(dy), damage(damage), width(5), height(5), alive(true)
{}

void Projectile::update(int windowWidth, int windowHeight) {
    x += dx * 10; // Multiplicador para la velocidad
    y += dy * 10;
    // Si el proyectil sale de la ventana, se marca como no activo.
    if (x < 0 || x > windowWidth || y < 0 || y > windowHeight)
        alive = false;
}

void Projectile::render(SDL_Renderer* renderer) {
    SDL_Rect rectProj = { static_cast<int>(x), static_cast<int>(y), width, height };
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Amarillo
    SDL_RenderFillRect(renderer, &rectProj);
}