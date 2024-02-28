//main_functions.cpp
#include "Main_Functions.h"
#include "Vector2.h"
#include <SDL.h>
Vector2 Main::getDisplayDims() {
	SDL_Rect r;
	SDL_GetDisplayBounds(0, &r);
	return {r.w, r.h};
}