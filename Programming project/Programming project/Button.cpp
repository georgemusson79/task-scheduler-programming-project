#include "renderable.h"
#include "Main_Functions.h"
#include "Collision.h"
#include "Cursor.h"
void Button::update() {
	Vector2 pos = Cursor::getPos();
	bool leftClick = Cursor::isLeftClicked();
	if (Collision::collidesWith(this->renderScrDims,pos) && !Cursor::isFocused) {
		Cursor::setCursor(SDL_SYSTEM_CURSOR_HAND);
		if (leftClick && !alreadyClicked) this->onClick();
	}
	else {

	}
	alreadyClicked = leftClick;
}