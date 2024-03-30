#include "simple_renderables.h"
#include "Cursor.h"
#include "Collision.h"
#include "Main_Functions.h"

Draggable::Draggable(SDL_Renderer* renderer, int x, int y, int w, int h, std::string pathToImg, SDL_Rect* movementBounds, MovementLimitations movementLimits, SDL_RendererFlip flip) : Renderable(renderer) {
	this->bg = new Sprite(renderer, x, y, w, h, pathToImg, flip);
	this->renderScrDims = { x,y,w,h };
	this->movementLimits = movementLimits;
	this->renderPriority = 200;
	this->name = "draggable";

	if (movementBounds == nullptr) {
		int w;
		int h;
		SDL_GetWindowSize(Main::window, &w, &h);
		this->movementBounds = { 0,0,w,h };
	}
	else this->movementBounds = *movementBounds;
}

Draggable::Draggable(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color color, SDL_Color borderColor, SDL_Rect* movementBounds, MovementLimitations movementLimits, SDL_RendererFlip flip) : Renderable(renderer) {
	this->bg = new Rectangle(renderer, x, y, w, h, true, color);
	this->renderScrDims = { x,y,w,h };
	this->movementLimits = movementLimits;
	this->renderPriority = 200;
	this->name = "draggable";

	if (movementBounds == nullptr) {
		int w;
		int h;
		SDL_GetWindowSize(Main::window, &w, &h);
		this->movementBounds = { 0,0,w,h };
	}
	else this->movementBounds = *movementBounds;
}



bool Draggable::setPos(Vector2 pos) {

	Vector2 oldPos = this->getPos();
	bool movement = false;

	/* If the object goes out of movementBounds on one of the axes move the object back to its position on the specified axis
	*  If the objeect moves on either axis return true otherwise return false
	*/

	this->renderScrDims.x = pos.x;
	if (!Collision::isInsideOf(this->movementBounds, this->renderScrDims)) this->renderScrDims.x = oldPos.x;
	else movement = true;

	this->renderScrDims.y = pos.y;
	if (!Collision::isInsideOf(this->movementBounds, this->renderScrDims)) this->renderScrDims.y = oldPos.y;
	else movement = true;

	return movement;
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
		dragPosition = getPos() - Cursor::getPos();
		trySetFocus();
	}


	//handles movement of object relative to cursor if object is picked up
	if (focused) {

		Vector2 newPos = Cursor::getPos() + dragPosition;
		Vector2 currentPos = this->getPos();
		switch (movementLimits) {
		case (ONLY_X):
			setPos({ newPos.x,currentPos.y });
			break;
		case (ONLY_Y):
			setPos({ currentPos.x,newPos.y });
			break;
		default:
			setPos(newPos);

		}
		//put the object back down if the user lets go of the cursor
		if (!clicked) tryRemoveFocus();
	}

}


void Draggable::render() {
	if (this->bg != nullptr) {
		SDL_Rect pos = this->getRenderingDims();
		this->bg->setRenderingDims(pos.x, pos.y, pos.w, pos.h);
		this->bg->render();
	}
}

Draggable::~Draggable() {
	delete bg;
}