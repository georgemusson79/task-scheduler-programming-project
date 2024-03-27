#include "renderable.h"
#include "Cursor.h"

Vector2 Cursor::getPos() {
	int x;
	int y;
	SDL_GetMouseState(&x, &y);
	return { x,y };
}

bool Cursor::setCursor(SDL_SystemCursor cursorType) {
	//return immediately if cursor has already been updated this frame
	if (Cursor::hasCursorChanged) return false;


	//return if cursor to update to is the same as current cursor, behaves exactly as if cursor has been updated
	if (Cursor::currentCursorType == cursorType && Cursor::currentCursor != nullptr) {
		Cursor::hasCursorChanged = true;
		return true;
	}

	SDL_Cursor* cursor = SDL_CreateSystemCursor(cursorType);
	SDL_SetCursor(cursor);
	SDL_FreeCursor(currentCursor);
	currentCursor = cursor;
	Cursor::currentCursorType = cursorType;
	Cursor::hasCursorChanged = true;
	std::cout << "cursor chabge\n";
	return true;
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

