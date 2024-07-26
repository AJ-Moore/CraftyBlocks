#pragma once 
#include <Common.h>

template <typename T = uint32>
class Rect {
public:
	Rect() = default;
	Rect(T x, T y, T width, T height) {
		X = x;
		Y = y;
		Width = width;
		Height = height;
	}

	T X = 0;
	T Y = 0;
	T Width = 0;
	T Height = 0;

	T Left() { return X; }
	T Right() { return Width; }
	T Top() { return Y; }
	T Bottom() { return Height; }
};

typedef Rect<> uRect;