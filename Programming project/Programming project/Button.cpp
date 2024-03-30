#include "simple_renderables.h"
#include "Main_Functions.h"
#include "Collision.h"
#include "Cursor.h"
void Button::update() {
	Vector2 pos = Cursor::getPos();
	bool cursorCollides = Collision::collidesWith(this->renderScrDims, pos);
	bool leftClick = Cursor::isLeftClicked();
	if (cursorCollides && !Cursor::isFocused) {
		this->onHover();
		Cursor::setCursor(SDL_SYSTEM_CURSOR_HAND);
		if (leftClick && !alreadyClicked) this->onClick();
	}
	else {

	}
	alreadyClicked = leftClick;
}