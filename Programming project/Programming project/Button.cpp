#include "renderable.h"
#include "Main_Functions.h"
#include "Cursor.h"
void Button::update() {
	Vector2 pos = Cursor::getPos();
	if (pos.x >= this->renderScrDims.x && pos.x <= this->renderScrDims.x + this->renderScrDims.w && pos.y >= this->renderScrDims.y && pos.y <= this->renderScrDims.y + this->renderScrDims.h) {
		Cursor::setCursor(SDL_SYSTEM_CURSOR_HAND);
		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) this->onClick();
	}
	else Cursor::setCursor(SDL_SYSTEM_CURSOR_ARROW);
}