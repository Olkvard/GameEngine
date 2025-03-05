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
    
    void processEvent(const SDL_Event& event);  // Procesa eventos (teclado) para el movimiento.
    void attack(std::vector<Projectile>& projectiles, std::vector<Enemy>& enemies); // Ejecuta el ataque: si el arma activa es melee, se usa meleeAttack; si es a distancia, se dispara un proyectil.
    void meleeAttack(std::vector<Enemy>& enemies);  // Realiza un ataque melee en forma de arco en la dirección del ratón y actualiza el indicador visual.
    virtual void update() override;
    virtual void render(SDL_Renderer* renderer) override;
    void toggleWeapon();    // Alterna entre el arma melee y la a distancia.
    void triggerDamageBlink();  // Activa el efecto de parpadeo (blink) al recibir daño.
    
    Inventory inventory;    // Inventario del personaje
    Weapon meleeWeapon; // Armas fijas del personaje.
    Weapon distanceWeapon;
    Weapon* activeWeapon;   // Puntero a la arma actualmente equipada.
    
    SDL_Rect rect;  // Se utiliza SDL_Rect para la posición y el tamaño.
    
private:
    int speed;
    int horizontal;
    int vertical;
    Uint32 damageTime;
    Uint32 meleeAttackTime; // Para el efecto visual del ataque melee.
    float lastAttackAngle;  // Ángulo en el momento del ataque melee.
    Uint32 lastAttackTime;
    
    static const Uint32 MELEE_ATTACK_DURATION = 100;    // Duración en milisegundos
    static const Uint32 ATTACK_COOLDOWN = 500;
};

#endif