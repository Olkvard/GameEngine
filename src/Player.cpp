#include "Player.h"
#include "Projectile.h"
#include "Enemy.h"
#include <cmath>
#include <iostream>

const float PI = 3.14159265f;

Player::Player(int x, int y, int w, int h, int health, int speed,
               const Weapon& meleeWeaponInit, const Weapon& distanceWeaponInit)
    : Mob(x, y, w, h, health, speed),
      meleeWeapon(meleeWeaponInit),
      distanceWeapon(distanceWeaponInit),
      speed(speed),
      horizontal(0),
      vertical(0),
      damageTime(0),
      meleeAttackTime(0),
      lastAttackAngle(0),
      lastAttackTime(0)
{
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    activeWeapon = &this->meleeWeapon;  // Por defecto, arma activa es la melee.
}

void Player::processEvent(const SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN && !event.key.repeat) {
        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
                vertical = -1;
                break;
            case SDL_SCANCODE_S:
                vertical = 1;
                break;
            case SDL_SCANCODE_A:
                horizontal = -1;
                break;
            case SDL_SCANCODE_D:
                horizontal = 1;
                break;
            default:
                break;
        }
    } else if (event.type == SDL_KEYUP && !event.key.repeat) {
        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
                if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S])
                    vertical = 1;
                else
                    vertical = 0;
                break;
            case SDL_SCANCODE_S:
                if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_W])
                    vertical = -1;
                else
                    vertical = 0;
                break;
            case SDL_SCANCODE_A:
                if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D])
                    horizontal = 1;
                else
                    horizontal = 0;
                break;
            case SDL_SCANCODE_D:
                if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A])
                    horizontal = -1;
                else
                    horizontal = 0;
                break;
            default:
                break;
        }
    }
}

void Player::attack(std::vector<Projectile>& projectiles, std::vector<Enemy>& enemies)
{
    Uint32 now = SDL_GetTicks();

    if (now - lastAttackTime < activeWeapon->cooldown)
    {
        return;
    }
    lastAttackTime = now;

    if (activeWeapon->type == WeaponType::Melee) {
        meleeAttack(enemies);
    } else {  // Ataque a distancia.
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        int centerX = rect.x + rect.w / 2;
        int centerY = rect.y + rect.h / 2;
        float diffX = static_cast<float>(mouseX - centerX);
        float diffY = static_cast<float>(mouseY - centerY);
        float length = std::sqrt(diffX * diffX + diffY * diffY);
        float normX = (length != 0) ? diffX / length : 0;
        float normY = (length != 0) ? diffY / length : -1;
        projectiles.push_back(Projectile(centerX, centerY, normX, normY, activeWeapon->damage));
    }
}

void Player::meleeAttack(std::vector<Enemy>& enemies)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    int centerX = rect.x + rect.w / 2;
    int centerY = rect.y + rect.h / 2;
    float attackAngle = std::atan2(static_cast<float>(mouseY - centerY), static_cast<float>(mouseX - centerX));
    
    // Guardamos el ángulo y la hora del ataque para el efecto visual.
    lastAttackAngle = attackAngle;
    meleeAttackTime = SDL_GetTicks();
    
    const float meleeRange = activeWeapon->alcance; // Rango máximo del ataque melee.
    const float arcHalfAngle = 30.0f * (PI / 180.0f); // 30° a cada lado (60° total).
    
    for (auto& enemy : enemies) {
        int enemyCenterX = enemy.x + enemy.width / 2;
        int enemyCenterY = enemy.y + enemy.height / 2;
        float dx = static_cast<float>(enemyCenterX - centerX);
        float dy = static_cast<float>(enemyCenterY - centerY);
        float distance = std::sqrt(dx * dx + dy * dy);
        
        if (distance > meleeRange)
            continue;
        
        float enemyAngle = std::atan2(dy, dx);
        float angleDiff = std::fabs(attackAngle - enemyAngle);
        if (angleDiff > PI)
            angleDiff = 2 * PI - angleDiff;
        
        if (angleDiff <= arcHalfAngle) {
            enemy.health -= activeWeapon->damage;
            enemy.triggerDamageBlink();
            if (enemy.health <= 0)
                enemy.alive = false;
        }
    }
}

void Player::receiveDamage(int damage)
{
    Uint32 now = SDL_GetTicks();
    if (now - damageTime >= DAMAGE_COOLDOWN)
    {
        health -= damage;
        triggerDamageBlink();
        damageTime = now;
    }
}

void Player::update()
{
    float dx = horizontal * speed;
    float dy = vertical * speed;
    if (horizontal != 0 && vertical != 0) {
        dx *= 0.7071f;
        dy *= 0.7071f;
    }
    rect.x += static_cast<int>(dx);
    rect.y += static_cast<int>(dy);
    if (rect.x < 0) rect.x = 0;
    if (rect.y < 0) rect.y = 0;
    if (rect.x > 800 - rect.w) rect.x = 800 - rect.w;
    if (rect.y > 600 - rect.h) rect.y = 600 - rect.h;
}

void Player::render(SDL_Renderer* renderer)
{
    // Primero, renderizamos el jugador.
    const Uint32 BLINK_DURATION = 500;
    const Uint32 BLINK_INTERVAL = 100;
    Uint32 now = SDL_GetTicks();
    if (now - damageTime < BLINK_DURATION) {
        if (((now - damageTime) / BLINK_INTERVAL) % 2 == 0)
            return;
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
    
    // Si se realizó un ataque melee recientemente, dibujamos el indicador del área de ataque.
    if (now - meleeAttackTime < MELEE_ATTACK_DURATION) {
        // Definimos los parámetros del ataque melee (mismos que en meleeAttack).
        const float meleeRange = 100.0f;
        const float arcHalfAngle = 30.0f * (PI / 180.0f);
        
        int centerX = rect.x + rect.w / 2;
        int centerY = rect.y + rect.h / 2;
        
        // Calculamos los puntos límite del ataque.
        float leftAngle = lastAttackAngle - arcHalfAngle;
        float rightAngle = lastAttackAngle + arcHalfAngle;
        
        int leftX = centerX + static_cast<int>(meleeRange * std::cos(leftAngle));
        int leftY = centerY + static_cast<int>(meleeRange * std::sin(leftAngle));
        int rightX = centerX + static_cast<int>(meleeRange * std::cos(rightAngle));
        int rightY = centerY + static_cast<int>(meleeRange * std::sin(rightAngle));
        
        // Establecemos un color (por ejemplo, rojo) y dibujamos el triángulo.
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, centerX, centerY, leftX, leftY);
        SDL_RenderDrawLine(renderer, centerX, centerY, rightX, rightY);
        SDL_RenderDrawLine(renderer, leftX, leftY, rightX, rightY);
    }
}

void Player::toggleWeapon()
{
    // Alterna entre meleeWeapon y distanceWeapon.
    if (activeWeapon == &meleeWeapon)
        activeWeapon = &distanceWeapon;
    else
        activeWeapon = &meleeWeapon;
}

void Player::triggerDamageBlink()
{
    damageTime = SDL_GetTicks();
}