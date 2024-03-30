#include <array>
struct Vector2;
struct SDL_Rect;
namespace Collision {

	//Checks if a position collides with a rectangle. Returns true if a position collides with a rectangle, otherwise returns false
	bool collidesWith(SDL_Rect rect,Vector2 pos);
	
	//Checks if 2 rectangles collide with each other, returns true if a collision is detected, otherwise returns false
	bool collidesWith(SDL_Rect a, SDL_Rect b); 
	
	//similar to collidesWith but checks if outside completely covers inside
	bool isInsideOf(SDL_Rect outside, SDL_Rect inside);

	//gets corners in a clockwise motion
	std::array<Vector2,4> getCorners(SDL_Rect r);
}