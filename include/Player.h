#ifndef PLAYER_H
#define PLAYER_H

#include "Mob.h"
#include "Weapon.h"
#include "Inventory.h"
#include <SDL2/SDL.h>
#include <vector>

// Declaración adelantada de Projectile
class Projectile;

class Player : public Mob {
public:
    Player(int x, int y, int w, int h, int health, int speed, const Weapon& weapon);
    
    void processEvent(const SDL_Event& event);
    void attack(std::vector<Projectile>& projectiles);
    
    virtual void update() override;
    virtual void render(SDL_Renderer* renderer) override;

    void triggerDamageBlink();
    
    Inventory inventory;
    Weapon currentWeapon;
    SDL_Rect rect; // Rectángulo que representa la posición y tamaño.
    
private:  
    int speed;      // Velocidad de movimiento.

    // Dirección actual en cada eje: -1, 0 o 1.
    int horizontal;
    int vertical;

    Uint32 damageTime;
};

#endif