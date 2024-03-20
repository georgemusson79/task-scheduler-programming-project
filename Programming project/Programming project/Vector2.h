#pragma once
//vector2.h
#include <iostream>
struct SDL_Point;
struct Vector2 {
	template <typename V1, typename V2>
	Vector2(V1 x, V2 y) : x(x), y(y) {
		static_assert((std::is_arithmetic_v<V1> && std::is_arithmetic_v<V2>), "value is not numeric"); //raise error if either value is not a number
	};
	Vector2() : x(0), y(0) {};
	double x;
	double y;
	double distance(Vector2 location); //get distance between 2 vectors
	Vector2 operator+(Vector2 vector); //add 2 vectors together
	Vector2 operator-(Vector2 vector); //subtrack 2 vectors
	Vector2 operator/(Vector2 vector); //divide 2 vectors
	Vector2 operator*(Vector2 vector); //multiply 2 vectors
	void operator+=(Vector2 vector);
	void operator-=(Vector2 vector);
	void operator*=(Vector2 vector);
	void operator/=(Vector2 vector);
	bool operator<=(Vector2 vector);
	bool operator>=(Vector2 vector);
	bool operator>(Vector2 vector);
	bool operator<(Vector2 vector);
	bool operator==(Vector2 vector);
	bool operator!=(Vector2 vector);
	operator SDL_Point(); //allow for type casting from SDL_point to Vector2
	void out(); //debug tool for outputting vector to console



};
