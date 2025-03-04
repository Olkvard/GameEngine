#include "Player.h"
#include "Projectile.h"
#include <cmath>
#include <iostream>

Player::Player(int x, int y, int w, int h, int health, int speed, const Weapon& weapon)
    : Mob(x, y, w, h, health, speed), currentWeapon(weapon), speed(speed), horizontal(0), vertical(0), damageTime(0)
{
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

// Procesa los eventos KEYDOWN y KEYUP para actualizar la dirección de movimiento.
// Se actualiza de forma que, al presionar A o D (o W/S), se use la última tecla presionada.
void Player::processEvent(const SDL_Event& event) {
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
                // Si se suelta W, se comprueba si S sigue presionado.
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

void Player::attack(std::vector<Projectile>& projectiles) {
    if (currentWeapon.type == WeaponType::Melee) {
        std::cout << "Ataque cuerpo a cuerpo con " << currentWeapon.name << "!\n";
        // La detección de colisiones se procesa en el main.
    } else if (currentWeapon.type == WeaponType::Ranged) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        int centerX = rect.x + rect.w / 2;
        int centerY = rect.y + rect.h / 2;
        float diffX = static_cast<float>(mouseX - centerX);
        float diffY = static_cast<float>(mouseY - centerY);
        float length = std::sqrt(diffX * diffX + diffY * diffY);
        float normX = 0, normY = 0;
        if (length != 0) {
            normX = diffX / length;
            normY = diffY / length;
        } else {
            normY = -1; // Valor por defecto
        }
        projectiles.push_back(Projectile(centerX, centerY, normX, normY, currentWeapon.damage));
        std::cout << "Ataque a distancia con " << currentWeapon.name << "!\n";
    }
}

// Actualiza la posición del jugador según la dirección almacenada.
// Se normaliza la velocidad en el caso de movimiento diagonal.
void Player::update() {
    float dx = horizontal * speed;
    float dy = vertical * speed;

    // Normalización: si se mueve en diagonal, se ajusta para mantener la misma velocidad.
    if (horizontal != 0 && vertical != 0) {
        dx *= 0.7071f; // Aproximadamente 1/sqrt(2)
        dy *= 0.7071f;
    }

    rect.x += static_cast<int>(dx);
    rect.y += static_cast<int>(dy);
}

// Renderiza el jugador como un rectángulo blanco.
void Player::render(SDL_Renderer* renderer) {
    // Configuramos constantes para el parpadeo
    const Uint32 BLINK_DURATION = 500; // Duración total del parpadeo
    const Uint32 BLINK_INTERVAL = 100; // Intervalo de parpadeo en ms

    Uint32 now = SDL_GetTicks();
    // Si se activó el parpadeo y estamos dentro del periodo, alternamos el renderizado.
    if(now - damageTime < BLINK_DURATION)
    {
        if (((now - damageTime) / BLINK_INTERVAL)% 2 == 0)
            return;
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void Player::triggerDamageBlink()
{
    damageTime = SDL_GetTicks();
}