#include "renderable.h"
#include "Cursor.h"
#include "Collision.h"

Draggable::Draggable(int x, int y, int w, int h, std::string pathToImg, MovementLimitations movementLimits, SDL_RendererFlip flip) : Sprite(x,y,w,h,pathToImg,flip) {
	this->movementLimits = movementLimits;
	this->renderPriority = 200;
	this->name = "draggable";
}

void Draggable::update() {
	const bool alreadyClicked = clicked;
	const bool cursorIntersects = Collision::collidesWith(renderScrDims, Cursor::getPos());
	clicked = Cursor::isLeftClicked();

	//change the cursor to a hand if the user is hovering over or has already picked up the object
	if (cursorIntersects || focused) Cursor::setCursor(SDL_SYSTEM_CURSOR_HAND);


	//if the user wasnt clicking before and has clicked on the object prepare the object for moving

	//drag position is the difference between the object position (top left corner) and the cursor position
	//this is so that the position of the object relative to the cursor is always the same as the object is being moved
	if (!alreadyClicked && clicked && cursorIntersects) {
		dragPosition = getPosition() - Cursor::getPos();
		trySetFocus();
	}


	//handles movement of object relative to cursor if object is picked up
	if (focused) {

		Vector2 newPos = Cursor::getPos() + dragPosition;
		switch (movementLimits) {
			case (ONLY_X):
				this->renderScrDims.x = newPos.x;
				break;
			case (ONLY_Y):
				this->renderScrDims.y = newPos.y;
				break;
			default:
				setPos(newPos);

		}
		//put the object back down if the user lets go of the cursor
		if (!clicked) tryRemoveFocus();
	}

}

