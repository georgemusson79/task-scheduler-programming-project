//vector2.cpp
#include "Vector2.h"
#include <cmath>
#include <SDL.h>
double Vector2::distance(Vector2 location) {
	double distsqrd = pow((location.x - this->x), 2) + pow((location.y - this->y), 2);
	return sqrt(distsqrd);
}



Vector2 Vector2::operator+(Vector2 vector) {
	return { this->x + vector.x,this->y + vector.y };
}
Vector2 Vector2::operator-(Vector2 vector) {
	return { this->x - vector.x,this->y - vector.y };
}
Vector2 Vector2::operator/(Vector2 vector) {
	return { this->x / vector.x,this->y / vector.y };
}
Vector2 Vector2::operator*(Vector2 vector) {
	return { this->x * vector.x,this->y * vector.y };
}
void Vector2::operator+=(Vector2 vector) {
	this->x += vector.x;
	this->y += vector.y;
}
void Vector2::operator-=(Vector2 vector) {
	this->x -= vector.x;
	this->y -= vector.y;
}
void Vector2::operator*=(Vector2 vector) {
	this->x *= vector.x;
	this->y *= vector.y;
}
void Vector2::operator/=(Vector2 vector) {
	this->x /= vector.x;
	this->y /= vector.y;
}

bool Vector2::operator<=(Vector2 vector) {
	return (this->x <= vector.x && this->y <= vector.y);
}
bool Vector2::operator>=(Vector2 vector) {
	return (this->x >= vector.x && this->y >= vector.y);
}
bool Vector2::operator>(Vector2 vector) {
	return (this->x > vector.x && this->y > vector.y);
}
bool Vector2::operator<(Vector2 vector) {
	return (this->x < vector.x && this->y < vector.y);
}
bool Vector2::operator==(Vector2 vector) {
	return (this->x == vector.x && this->y == vector.y);
}
bool Vector2::operator!=(Vector2 vector) {
	return (this->x != vector.x || this->y != vector.y);
}
void Vector2::out() {
	std::cout << "(" << this->x << " , " << this->y << ")\n";
}
 Vector2::operator SDL_Point() {
	 return { (int)x,(int)y };
}
