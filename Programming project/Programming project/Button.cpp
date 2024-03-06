#include "renderable.h"
#include "Main_Functions.h"
void Button::update() {
	Vector2 pos = Main::getCursorPos();
	if (pos.X >= this->renderScrDims.x && pos.X <= this->renderScrDims.x + this->renderScrDims.w && pos.Y >= this->renderScrDims.y && pos.Y <= this->renderScrDims.y + this->renderScrDims.h && (SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))) this->onClick();
}