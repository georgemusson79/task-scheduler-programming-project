#include "simple_renderables.h"
#include "Main_Functions.h"

void RenderableRect::render() {
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a); //record current color in main renderer

	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a); //set renderer color to specified color then draw RenderableRect
	int error= (this->fill) ? SDL_RenderFillRect(renderer, &renderScrDims) : SDL_RenderDrawRect(renderer, &renderScrDims); 
	if (error != 0) std::cerr << "Unable to draw RenderableRect: " << SDL_GetError(); //raise exception if unable to render to screen
	

	if (this->renderWithBorder) {
		SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a); //set renderer color to specified color then draw RenderableRect

		SDL_RenderDrawRect(renderer, &this->renderScrDims);
	}

	SDL_SetRenderDrawColor(renderer, r, g, b, a); //set renderer colours back to before

}

RenderableRect::RenderableRect(SDL_Renderer* renderer, int x, int y, int w, int h, bool fill,SDL_Color colour, bool renderWithBorder,SDL_Color borderColor) : Renderable(renderer) {
	this->colour = colour;
	this->renderScrDims = { x,y,w,h };
	this->fill = fill;
	this->renderWithBorder = renderWithBorder;
	this->borderColor = borderColor;
}