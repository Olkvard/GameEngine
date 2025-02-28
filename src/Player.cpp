#include "Player.h"

// Inicializa el jugador con la posición, tamaño y velocidad indicados.
Player::Player(int x, int y, int w, int h, int speed)
    : speed(speed), horizontal(0), vertical(0)
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
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}