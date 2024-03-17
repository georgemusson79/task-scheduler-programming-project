#include <SDL.h>
#include "Collision.h"
#include "Vector2.h"

bool Collision::collidesWith(SDL_Rect rect, Vector2 pos) {
	if (pos.X >= rect.x &&
		pos.X <= rect.x + rect.w && 
		pos.Y >= rect.y &&
		pos.Y <= rect.y + rect.h) return true;
	return false;
}
bool Collision::collidesWith(SDL_Rect a, SDL_Rect b) {
	if (a.x<b.x + b.w &&
		a.x + a.w>b.x &&
		a.y<b.y + b.h &&
		a.y + a.h>b.y) return true;
	return false;
}