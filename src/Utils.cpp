#include "Utils.h"

bool checkCollision(int x1, int y1, int w1, int h1,
                    int x2, int y2, int w2, int h2) {
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

void resolveRectCollision(SDL_Rect& moving, const SDL_Rect& fixed)
{
        if (!checkCollision(moving.x, moving.y, moving.w, moving.h, fixed.x, fixed.y, fixed.w, fixed.h))
                return;

        int overlapLeft         = (moving.x + moving.w) - fixed.x;
        int overlapRight        = (fixed.x + fixed.w) - moving.x;
        int overlapTop          = (moving.y + moving.h) - fixed.y;
        int overlapBottom       = (fixed.y + fixed.h) - moving.y;

        int minOverlapX = (overlapLeft < overlapRight) ? overlapLeft : overlapRight;
        int minOverlapY = (overlapTop < overlapBottom) ? overlapTop : overlapBottom;

        if (minOverlapX < minOverlapY)
        {
                if (moving.x < fixed.x)
                        moving.x -= minOverlapX;
                else
                        moving.x += minOverlapX;
        }
        else
        {
                if (moving.y < fixed.y)
                        moving.y -= minOverlapY;
                else
                        moving.y += minOverlapY;
        }
}