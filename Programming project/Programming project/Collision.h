struct Vector2;
struct SDL_Rect;
namespace Collision {
	bool collidesWith(SDL_Rect rect,Vector2 pos); //Checks if a position collides with a rectangle. Returns true if a position collides with a rectangle, otherwise returns false
	bool collidesWith(SDL_Rect a, SDL_Rect b); //Checks if 2 rectangles collide with each other, returns true if a collision is detected, otherwise returns false
}