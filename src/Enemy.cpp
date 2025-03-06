#include "Enemy.h"
#include "Player.h"

Enemy::Enemy(int x, int y, int w, int h, int health, int speed, int fuerza, int level)
    : Mob(x, y, w, h, health, speed),
      alive(true),
      damageTime(0),
      fuerza(fuerza),
      level(level)
{}

void Enemy::update()
{
    // La IA (como chase) se invoca desde el main.
}

void Enemy::render(SDL_Renderer* renderer)
{
    const Uint32 BLINK_DURATION = 500;
    const Uint32 BLINK_INTERVAL = 100;

    Uint32 now = SDL_GetTicks();
    if (now - damageTime < BLINK_DURATION)
    {
        if(((now - damageTime) / BLINK_INTERVAL)% 2 == 0)
            return;
    }

    SDL_Rect rect = { x, y, width, height };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rojo para enemigos
    SDL_RenderFillRect(renderer, &rect);
}

void Enemy::chase(const Player& player)
{
    int targetX = player.rect.x + player.rect.w / 2;
    int targetY = player.rect.y + player.rect.h / 2;
    int enemyCenterX = x + width / 2;
    int enemyCenterY = y + height / 2;
    
    if (enemyCenterX < targetX) x += speed;
    if (enemyCenterX > targetX) x -= speed;
    if (enemyCenterY < targetY) y += speed;
    if (enemyCenterY > targetY) y -= speed;
}

void Enemy::triggerDamageBlink()
{
    damageTime = SDL_GetTicks();
}