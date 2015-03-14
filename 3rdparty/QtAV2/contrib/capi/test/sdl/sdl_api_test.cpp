#include "sdl_api.h"
extern "C" {
//#include <SDL/SDL.h>
}
class sdl_test : public sdl::api {
public:
    void run() {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_SetVideoMode(640, 480, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
        SDL_WM_SetCaption("SDL Start", 0);
        SDL_Event event;
        bool gameRunning = true;
        while (gameRunning) {
            if (SDL_PollEvent(&event))
                gameRunning = event.type != SDL_QUIT;
        }
        SDL_Quit();
    }
};

//#undef main //SDL_main
int main(int argc, char *argv[])
{
    sdl_test test;
    test.run();
    return 0;
}
