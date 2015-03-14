#ifndef DLLAPI_SDL_H
#define DLLAPI_SDL_H

extern "C" {
#include <SDL/SDL.h>
}
namespace sdl {
class api_dll;
class api {
public:
    api();
    virtual ~api();
    int SDL_Init(Uint32 flags);
    void SDL_Quit(void);
    int SDL_PollEvent(SDL_Event *event);
    SDL_Surface* SDL_SetVideoMode(int width, int height, int bpp, Uint32 flags);
    void SDL_WM_SetCaption(const char *title, const char *icon);
private:
    api_dll *dll;
};
} //namespace sdl

#endif // DLLAPI_SDL_H
