#include <SDL2/SDL.h>
#include <iostream>
#include "Player.cpp"

int main(int argc, char* argv[]) {
    // Inicializar SDL (video).
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Error al inicializar SDL: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    // Crear la ventana.
    SDL_Window* window = SDL_CreateWindow("Motor de Juego - Movimiento del Personaje",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Error al crear la ventana: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    
    // Crear el renderizador acelerado por hardware.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Error al crear el renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    // Instanciar el jugador en el centro de la ventana.
    Player player(400 - 25, 300 - 25, 50, 50, 5);

    bool running = true;
    bool fullscreen = false;
    SDL_Event event;
    
    // Loop principal del motor.
    while (running) {
        while (SDL_PollEvent(&event)) {
            // Salir si se cierra la ventana.
            if (event.type == SDL_QUIT)
                running = false;

            // Toggle de pantalla completa con F11.
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_F11) {
                fullscreen = !fullscreen;
                if (fullscreen)
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                else
                    SDL_SetWindowFullscreen(window, 0);
            }
            
            // Procesa el evento en el jugador (para movimiento).
            player.processEvent(event);
        }
        
        // Actualiza la lógica del jugador.
        player.update();
        
        // Renderizado: limpiar la pantalla, dibujar al jugador y presentar el frame.
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        player.render(renderer);
        SDL_RenderPresent(renderer);
        
        // Control aproximado a ~60 FPS.
        SDL_Delay(16);
    }
    
    // Liberar recursos.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}