#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Utils.h"
#include "Weapon.h"
#include "WeaponPickup.h"

int main(int argc, char* argv[])
{
    // Inicializar SDL (video).
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Error al inicializar SDL: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    // Crear la ventana.
    SDL_Window* window = SDL_CreateWindow("Kamos_Game",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          1920, 1080, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Error al crear la ventana: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    
    // Crear el renderizador acelerado por hardware.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Error al crear el renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    // Instanciar el jugador en el centro de la ventana.
    Weapon meleeWeapon("Espada basica", 10, WeaponType::Melee, 400, 70.0f, "común", 1, 0,0,0,0);
    Weapon distanceWeapon("Arco Simple", 8, WeaponType::Ranged, 600, 0.0f, "común", 1, 0,0,0,0);
    Player player(400, 300, 30, 30, 100, 5, meleeWeapon, distanceWeapon);

    // Agregamos un arma a distancia al inventario
    Weapon bow("Arco Avanzado", 12, WeaponType::Ranged, 800, 0.0f, "poco común", 1, 200, 200, 30, 30);
    WeaponPickup bowPickup(200, 200, 30, 30, bow);

    // Creamos algunos enemigos
    std::vector<Enemy> enemies;
    enemies.push_back(Enemy(100, 100, 30, 30, 50, 2, 5, 1));
    enemies.push_back(Enemy(200, 200, 30, 30, 50, 2, 5, 1));

    // Vector para almacenar proyectiles
    std::vector<Projectile> projectiles;


    bool running = true;
    bool fullscreen = false;
    bool gameOver = false;
    SDL_Event event;
    bool attackTriggered = false;
    int windowWidth = 0, windowHeight = 0;
    Uint32 deathTime = 0;
    
    // Loop principal del motor.
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            // Salir si se cierra la ventana.
            if (event.type == SDL_QUIT)
                running = false;
            
            // Cambio de arma y ataque
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_RIGHT)
                    player.toggleWeapon();
                if (event.button.button == SDL_BUTTON_LEFT)
                    attackTriggered = true;
            }

            // Toggle de pantalla completa con F11.
            if (event.type == SDL_KEYDOWN && !event.key.repeat)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_F11)
                {
                    fullscreen = !fullscreen;
                    if (fullscreen)
                        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    else
                        SDL_SetWindowFullscreen(window, 0);
                }
                // Muestra el inventario.
                if (event.key.keysym.scancode == SDL_SCANCODE_E)
                    player.inventory.listItems();
            }
            
            // Procesa el evento en el jugador (para movimiento).
            player.processEvent(event);
        }
        
        // Actualiza la lógica del jugador.
        player.update(windowWidth, windowHeight);

        // Gestiona la recogida del item bow.
        if (!bowPickup.pickedUp && checkCollision(player.rect.x, player.rect.y, player.rect.w, player.rect.h, bowPickup.rect.x, bowPickup.rect.y, bowPickup.rect.w, bowPickup.rect.h))
        {
            player.inventory.addWeapon(bowPickup.weapon);
            bowPickup.pickedUp = true;
        }

        for (auto& enemy : enemies) // Hace que los enemigos persigan al jugador.
        {
            enemy.chase(player);
        }

        for(auto& enemy : enemies)  // Evita la superposición de los enemigos con el jugador y le hace daño en caso de estar superpuestos
        {
            SDL_Rect playerRect = player.rect;
            SDL_Rect enemyRect = { enemy.x, enemy.y, enemy.width, enemy.height};
            if(checkCollision(playerRect.x, playerRect.y, playerRect.w, playerRect.h, enemyRect.x, enemyRect.y, enemyRect.w, enemyRect.h))
            {
                player.receiveDamage(enemy.fuerza);
                resolveRectCollision(enemyRect, playerRect);
                enemy.x = enemyRect.x;
                enemy.y = enemyRect.y;
            }
        }

        if (!gameOver && player.health <= 0)
        {
            gameOver = true;
            deathTime = SDL_GetTicks();
            std::cout << "Game Over. El jugador ha muerto. \n";
        }

        for (size_t i = 0; i < enemies.size(); ++i) // Evita la superposición entre los enemigos.
        {
            for(size_t j = i +1; j < enemies.size(); ++j)
            {
                SDL_Rect rect1 = { enemies[i].x, enemies[i].y, enemies[i].width, enemies[i].height};
                SDL_Rect rect2 = { enemies[j].x, enemies[j].y, enemies[j].width, enemies[j].height};
                if (checkCollision(rect1.x, rect1.y, rect1.w, rect1.h, rect2.x, rect2.y, rect2.w, rect2.h))
                {
                    resolveRectCollision(rect2, rect1);
                    enemies[j].x = rect2.x;
                    enemies[j].y = rect2.y;
                }
            }
        }

        SDL_GetWindowSize(window, &windowWidth, &windowHeight);

        // Procesar ataque
        if (attackTriggered)
        {
            player.attack(projectiles, enemies);
            attackTriggered = false;
        }

        // Actualizar proyectiles
        for (auto& proj : projectiles)
            proj.update(windowWidth, windowHeight);

        // Eliminar proyectiles fuera de la ventana
        projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),[](const Projectile& p){ return !p.alive;}), projectiles.end());

        // Comprobar colisiones entre proyectiles y enemigos
        for (auto& proj : projectiles)
        {
            for (auto& enemy : enemies)
            {
                if (checkCollision(proj.x, proj.y, proj.width, proj.height, enemy.x, enemy.y, enemy.width, enemy.height))
                {
                    enemy.health -= proj.damage;
                    proj.alive = false;
                    enemy.triggerDamageBlink();
                    if (enemy.health <= 0)
                        enemy.alive = false;
                }
            }
        }

        // Eliminar enemigos muertos
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const Enemy& e) { return !e.alive; }), enemies.end());

        if (gameOver)   // Si el jugador ha muerto cerramos el programa después de 3 segundos.
        {
            if(SDL_GetTicks() - deathTime >= 3000)
            {
                running = false;
            }
        }
        
        // Renderizado: limpiar la pantalla, dibujar al jugador y presentar el frame.
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        player.render(renderer);
        for (auto& enemy : enemies)
            enemy.render(renderer);
        for (auto& proj : projectiles)
            proj.render(renderer);
        bowPickup.render(renderer);
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