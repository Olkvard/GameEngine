#ifndef PLAYER_H
#define PLAYER_H

#include "Mob.h"
#include "Weapon.h"
#include "Inventory.h"
#include <SDL2/SDL.h>
#include <vector>

// Declaración adelantada de Projectile y Enemy.
class Projectile;
class Enemy;

class Player : public Mob {
public:
    // Ahora se recibe la meleeWeapon y la distanceWeapon iniciales.
    Player(int x, int y, int w, int h, int health, int speed,
           const Weapon& meleeWeaponInit, const Weapon& distanceWeaponInit);
    
    // Procesa eventos (teclado) para el movimiento.
    void processEvent(const SDL_Event& event);
    
    // Ejecuta el ataque: si el arma activa es melee, se usa meleeAttack; si es a distancia, se dispara un proyectil.
    void attack(std::vector<Projectile>& projectiles, std::vector<Enemy>& enemies);
    
    // Realiza un ataque melee en forma de arco en la dirección del ratón.
    void meleeAttack(std::vector<Enemy>& enemies);
    
    virtual void update() override;
    virtual void render(SDL_Renderer* renderer) override;
    
    // Alterna entre el arma melee y la a distancia.
    void toggleWeapon();
    
    // Activa el efecto de parpadeo (blink) al recibir daño.
    void triggerDamageBlink();
    
    Inventory inventory;
    
    // Armas fijas del personaje.
    Weapon meleeWeapon;
    Weapon distanceWeapon;
    // Puntero a la arma actualmente equipada.
    Weapon* activeWeapon;
    
    // Se utiliza SDL_Rect para la posición y el tamaño.
    SDL_Rect rect;
    
private:
    int speed;
    int horizontal;
    int vertical;
    Uint32 damageTime;
};

#endif