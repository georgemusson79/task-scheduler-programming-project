//main_functions.cpp

#include "Main_Functions.h"
#include "Vector2.h"
#include <SDL.h>
Vector2 Main::getDisplayDims() {
	SDL_Rect r;
	SDL_GetDisplayBounds(0, &r);
	return {r.w, r.h};
}

Vector2 Main::getCursorPos() {
	int x;
	int y;
	SDL_GetMouseState(&x, &y);
	return { x,y };
}

void Main::updateRenderables() {
	SDL_RenderClear(Main::renderer);
	for (Renderable* r : Main::renderables) {
		r->render();
		r->update();
	}
	SDL_RenderPresent(Main::renderer);
}