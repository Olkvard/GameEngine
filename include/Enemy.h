#ifndef ENEMY_H
#define ENEMY_H

#include "Mob.h"
#include <SDL2/SDL.h>

// Declaración adelantada para evitar dependencias circulares
class Player;

class Enemy : public Mob {
public:
    Enemy(int x, int y, int w, int h, int health, int speed);
    
    virtual void update() override;
    virtual void render(SDL_Renderer* renderer) override;
    
    // Movimiento básico para perseguir al jugador
    void chase(const Player& player);
    void triggerDamageBlink();
    
    bool alive;

private:

    Uint32 damageTime;
};

#endif