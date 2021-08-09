#ifndef Rect_cpp
#define Rect_cpp

#include "SDL.h"
#include "Vec2D.cpp"

class Rect
{
public:
	int x, y, w, h;
	static bool intersect(const Rect& r1, const Rect& r2)
	{
		if (r1.x + r1.w < r2.x)
			return false;
		if (r2.x + r2.w < r1.x)
			return false;
		if (r1.y + r1.h < r2.y)
			return false;
		if (r2.y + r2.h < r1.y)
			return false;
		return true;
	}

	static bool intersect(const Rect& r1, const Vec2D& p)
	{
		if (r1.x + r1.w < p.x)
			return false;
		if (r1.x > p.x)
			return false;
		if (r1.y + r1.h < p.y)
			return false;
		if (r1.y > p.y)
			return false;
		return true;
	}

	SDL_Rect getSDL_Rect(float xoff = .0f, float yoff = .0f)
	{
		return { (int)(x + xoff), (int)(y + yoff), w, h };
	}

	Rect& copy() {
		Rect r;
		r.x = x;
		r.y = y;
		r.w = w;
		r.h = h;

		return r;
	}

	Rect(int x = 0, int y = 0, int w = 0, int h = 0)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}
};

#endif
