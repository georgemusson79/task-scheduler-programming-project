//vector2.cpp
#include "Vector2.h"
#include <cmath>
double Vector2::distance(Vector2 location) {
	double distsqrd = pow((location.X - this->X), 2) + pow((location.Y - this->Y), 2);
	return sqrt(distsqrd);
}

Vector2 iVector2::convertToVector2() {
	return { (double)X,(double)Y };
}


Vector2 Vector2::operator+(Vector2 vector) {
	return { this->X + vector.X,this->Y + vector.Y };
}
Vector2 Vector2::operator-(Vector2 vector) {
	return { this->X - vector.X,this->Y - vector.Y };
}
Vector2 Vector2::operator/(Vector2 vector) {
	return { this->X / vector.X,this->Y / vector.Y };
}
Vector2 Vector2::operator*(Vector2 vector) {
	return { this->X * vector.X,this->Y * vector.Y };
}
void Vector2::operator+=(Vector2 vector) {
	this->X += vector.X;
	this->Y += vector.Y;
}
void Vector2::operator-=(Vector2 vector) {
	this->X -= vector.X;
	this->Y -= vector.Y;
}
void Vector2::operator*=(Vector2 vector) {
	this->X *= vector.X;
	this->Y *= vector.Y;
}
void Vector2::operator/=(Vector2 vector) {
	this->X /= vector.X;
	this->Y /= vector.Y;
}

bool Vector2::operator<=(Vector2 vector) {
	return (this->X <= vector.X && this->Y <= vector.Y);
}
bool Vector2::operator>=(Vector2 vector) {
	return (this->X >= vector.X && this->Y >= vector.Y);
}
bool Vector2::operator>(Vector2 vector) {
	return (this->X > vector.X && this->Y > vector.Y);
}
bool Vector2::operator<(Vector2 vector) {
	return (this->X < vector.X && this->Y < vector.Y);
}
bool Vector2::operator==(Vector2 vector) {
	return (this->X == vector.X && this->Y == vector.Y);
}
bool Vector2::operator!=(Vector2 vector) {
	return (this->X != vector.X || this->Y != vector.Y);
}
void Vector2::out() {
	std::cout << "(" << this->X << " , " << this->Y << ")\n";
}