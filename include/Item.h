#ifndef ITEM_H
#define ITEM_H

#include <SDL2/SDL.h>
#include <string>

class Item {
public:
    Item(int x, int y, int w, int h, const std::string& name);
    virtual ~Item() {}
    virtual void render(SDL_Renderer* renderer);
    
    SDL_Rect rect;
    bool collected;
    std::string name;
};

#endif
