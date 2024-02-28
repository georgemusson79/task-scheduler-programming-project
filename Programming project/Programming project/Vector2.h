#pragma once
#include <iostream>
struct Vector2 {
	template <typename V1, typename V2>
	Vector2(V1 X, V2 Y) : X(X), Y(Y) {
		static_assert((std::is_arithmetic_v<V1> && std::is_arithmetic_v<V2>), "value is not numeric");
	};
	Vector2() : X(0), Y(0) {};
	double X;
	double Y;
	double distance(Vector2 location);
	Vector2 operator+(Vector2 vector);
	Vector2 operator-(Vector2 vector);
	Vector2 operator/(Vector2 vector);
	Vector2 operator*(Vector2 vector);
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
	void out();



};

struct iVector2 {
	int X;
	int Y;
	Vector2 convertToVector2();
};