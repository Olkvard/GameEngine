#ifndef MOB_H
#define MOB_H

class Mob {
public:
    Mob(int x, int y, int w, int h, int health, int speed);
    virtual ~Mob() = default;
    
    virtual void update(int windowWidth, int windowHeight) = 0;
    virtual void render(class SDL_Renderer* renderer) = 0;
    
    int x, y;
    int width, height;
    int health;
    int speed;
};

#endif