#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

class Player {
public:
    // Constructor: posición (x,y), tamaño (w,h) y velocidad (en píxeles por frame).
    Player(int x, int y, int w, int h, int speed);

    // Procesa eventos relacionados al movimiento del jugador.
    void processEvent(const SDL_Event& event);

    // Actualiza la posición del jugador.
    void update();

    // Renderiza el jugador usando el renderizador de SDL.
    void render(SDL_Renderer* renderer);

private:
    SDL_Rect rect;  // Rectángulo que representa la posición y tamaño.
    int speed;      // Velocidad de movimiento.

    // Dirección actual en cada eje: -1, 0 o 1.
    int horizontal;
    int vertical;
};

#endif