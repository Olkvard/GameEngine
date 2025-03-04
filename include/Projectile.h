#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SDL2/SDL.h>

class Projectile {
public:
    Projectile(float x, float y, float dx, float dy, int damage);
    
    void update(int windowWidth, int windowHeight);
    void render(SDL_Renderer* renderer);
    
    float x, y;  // Usamos float para movimientos suaves
    int width, height;
    float dx, dy;
    int damage;
    bool alive;
};

#endif