#include "renderable.h"
#include "Main_Functions.h"

void Rectangle::render() {
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(Main::renderer, &r, &g, &b, &a); //record current color in main renderer

	SDL_SetRenderDrawColor(Main::renderer, colour.r, colour.g, colour.b, colour.a); //set renderer color to specified color then draw rectangle
	int error= (this->fill) ? SDL_RenderFillRect(Main::renderer, &renderScrDims) : SDL_RenderDrawRect(Main::renderer, &renderScrDims); 
	if (error != 0) std::cerr << "Unable to draw rectangle: " << SDL_GetError(); //raise exception if unable to render to screen
	

	SDL_SetRenderDrawColor(Main::renderer, r, g, b, a); //set renderer colours back to before
}

Rectangle::Rectangle(int x, int y, int w, int h, SDL_Color colour) {
	this->colour = colour;
	this->renderScrDims = { x,y,w,h };
}