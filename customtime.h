#ifndef CUSTOMTIME_H
#define CUSTOMTIME_H

#include <SDL2/SDL.h>

class CustomTime
{
private:

    Uint32 currentTime = SDL_GetTicks();
    Uint32 lastTime = 0;

public:
    float deltaTime = 0;

    CustomTime()
    {

    }

    void calcDeltaTime()
    {
        if (currentTime > lastTime)
            lastTime = currentTime;
        currentTime = SDL_GetTicks();
        deltaTime = ((currentTime - lastTime)/1000.0f);
    }
};

#endif // CUSTOMTIME_H
