#ifndef UTILS_H
#define UTILS_H
#include <SDL2/SDL.h>

bool checkCollision(int x1, int y1, int w1, int h1,
                    int x2, int y2, int w2, int h2);

void resolveRectCollision(SDL_Rect& moving, const SDL_Rect& fixed);

#endif