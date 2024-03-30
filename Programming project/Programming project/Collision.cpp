#include <SDL.h>
#include <array>
#include "Collision.h"
#include "Vector2.h"

bool Collision::collidesWith(SDL_Rect rect, Vector2 pos) {
	if (pos.x >= rect.x &&
		pos.x <= rect.x + rect.w && 
		pos.y >= rect.y &&
		pos.y <= rect.y + rect.h) return true;
	return false;
}
bool Collision::collidesWith(SDL_Rect a, SDL_Rect b) {
	if (a.x<b.x + b.w &&
		a.x + a.w>b.x &&
		a.y<b.y + b.h &&
		a.y + a.h>b.y) return true;
	return false;
}

std::array<Vector2, 4> Collision::getCorners(SDL_Rect r) {
	std::array<Vector2, 4> corners;
	corners[0] = { r.x,r.y };
	corners[1] = { r.x + r.w,r.y };
	corners[2] = { r.x + r.w,r.y + r.h };
	corners[3] = { r.x,r.y + r.h };
	return corners;
}

bool Collision::isInsideOf(SDL_Rect outside, SDL_Rect inside) {
	std::array<Vector2, 4> cornersOutside = Collision::getCorners(outside);
	std::array<Vector2, 4> cornersInside = Collision::getCorners(inside);

	if (cornersInside[0] >= cornersOutside[0] && cornersInside[2] <= cornersOutside[2]) return true;
	return false;

}
