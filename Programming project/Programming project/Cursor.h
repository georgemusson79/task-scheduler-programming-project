#pragma once
#include "Vector2.h"
#include <SDL.h>

class Renderable;


namespace Cursor {
	inline bool hasCursorChanged = false; //to be reset at start of every frame, used to check if cursor has been updated once in a frame already, if so dont update again
	inline SDL_SystemCursor currentCursorType = SDL_SYSTEM_CURSOR_ARROW;
	inline SDL_Cursor* currentCursor= nullptr;
	inline bool isFocused = false;
	inline Renderable* focusedItem = nullptr;

	Vector2 getPos();
	bool isLeftClicked();
	bool isRightClicked();
	bool isMiddleClicked();
	void setCursor(SDL_SystemCursor cursorType);
	void focusOn(Renderable* obj, SDL_SystemCursor);
}