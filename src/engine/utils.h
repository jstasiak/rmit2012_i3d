#include "precompile.h"

#ifndef UTILS_H
#define UTILS_H

long long millisecondsNow();
void drawAxes(float length);

template<typename T>
class Rect {
public: T x;
public: T y;
public: T width;
public: T height;

public: Rect(T x, T y, T width, T height)
			: x(x), y(y), width(width), height(height)
		{

		}

public: Rect() : x(0), y(0), width(0), height(0) {

	}
};

typedef Rect<float> Rectf;
typedef Rect<int> Recti;


#endif