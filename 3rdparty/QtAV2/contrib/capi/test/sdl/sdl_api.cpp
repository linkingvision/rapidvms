#include "sdl_api.h"
#define DEBUG_LOAD
#define DEBUG_RESOLVE
#include "capi.h"
#include <QtCore/QLibrary>
namespace sdl {

//DEFINE_DLL_INSTANCE_N("sdl", "SDL", "SDL32", "SDL-1.2", NULL)
static const char* sdl_names[] = { "SDL", "SDL32", "sdl", "SDL-1.2", NULL };
static const int sdl_vers[] = { capi::NoVersion, 2, 1, capi::EndVersion };
CAPI_BEGIN_DLL_VER(sdl_names, sdl_vers, QLibrary)
CAPI_DEFINE_M_RESOLVER(int, SDLCALL, SDL_Init, CAPI_ARG1(Uint32))
CAPI_DEFINE_M_RESOLVER(void, SDLCALL, SDL_WM_SetCaption, CAPI_ARG2(const char*, const char*))
CAPI_DEFINE_M_RESOLVER(int, SDLCALL, SDL_PollEvent, CAPI_ARG1(SDL_Event*))
CAPI_DEFINE_M_RESOLVER(SDL_Surface*, SDLCALL, SDL_SetVideoMode, CAPI_ARG4(int, int, int, Uint32))
CAPI_DEFINE_M_RESOLVER(void, SDLCALL, SDL_Quit, CAPI_ARG0())
CAPI_END_DLL()
CAPI_DEFINE(int, SDL_Init, CAPI_ARG1(Uint32))
CAPI_DEFINE(void, SDL_WM_SetCaption, CAPI_ARG2(const char*, const char*))
CAPI_DEFINE(int, SDL_PollEvent, CAPI_ARG1(SDL_Event*))
CAPI_DEFINE(SDL_Surface*, SDL_SetVideoMode, CAPI_ARG4(int, int, int, Uint32))
CAPI_DEFINE(void, SDL_Quit, CAPI_ARG0())

api::api() : dll(new api_dll()) {}
api::~api() { delete dll;}
} //namespace sdl
