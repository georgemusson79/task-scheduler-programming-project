#include "renderable.h"
#include "Main_Functions.h"
#include "Cursor.h"
void Button::update() {
	Vector2 pos = Cursor::getPos();
	if (pos.X >= this->renderScrDims.x && pos.X <= this->renderScrDims.x + this->renderScrDims.w && pos.Y >= this->renderScrDims.y && pos.Y <= this->renderScrDims.y + this->renderScrDims.h) {
		Cursor::setCursor(SDL_SYSTEM_CURSOR_HAND);
		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) this->onClick();
	}
	else Cursor::setCursor(SDL_SYSTEM_CURSOR_ARROW);
}