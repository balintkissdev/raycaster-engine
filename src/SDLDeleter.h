#ifndef SDL_DELETER
#define SDL_DELETER

#include <SDL2/SDL.h>

struct SDLDeleter
{
    void operator()(SDL_Window* window) { SDL_DestroyWindow(window); }
    void operator()(SDL_Renderer* render) { SDL_DestroyRenderer(render); }
    void operator()(SDL_Surface* surface) { SDL_FreeSurface(surface); }
    void operator()(SDL_Texture* texture) { SDL_DestroyTexture(texture); }
};

#endif
