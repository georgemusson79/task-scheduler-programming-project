#include "renderable.h"
#include "Cursor.h"

Vector2 Cursor::getPos() {
	int x;
	int y;
	SDL_GetMouseState(&x, &y);
	return { x,y };
}

void Cursor::setCursor(SDL_SystemCursor cursorType) {
	if ((Cursor::currentCursorType == cursorType && Cursor::currentCursor!=nullptr) || Cursor::hasCursorChanged) return; //return immediately if cursor has already been updated this frame, also return if cursor to update to is the same as current cursor
	SDL_Cursor* cursor = SDL_CreateSystemCursor(cursorType);
	SDL_SetCursor(cursor);
	SDL_FreeCursor(currentCursor);
	currentCursor = cursor;
	Cursor::currentCursorType = cursorType;
	Cursor::hasCursorChanged = true;
}

bool Cursor::isLeftClicked() {
	return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
}
bool Cursor::isRightClicked() {
	return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT);
}
bool Cursor::isMiddleClicked() {
	return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE);

}

void Cursor::focusOn(Renderable* obj,SDL_SystemCursor cursor) {
	if (obj != nullptr) return;
	Cursor::setCursor(SDL_SYSTEM_CURSOR_HAND);
	Cursor::focusedItem = obj;

}