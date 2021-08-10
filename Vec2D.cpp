#ifndef VEC2D_cpp
#define VEC2D_cpp

constexpr float F_PI = 3.141592653f;

#include "SDL.h"
#include <math.h>



class Vec2D
{
public:
	static float Q_rsqrt(float number) // inverse sqrt - stolen from QuakeIII
	{
		long i;
		float x2, y;
		const float threehalfs = 1.5F;

		x2 = number * 0.5F;
		y = number;
		i = *(long*)&y;                       // evil floating Vec2D bit level hacking
		i = 0x5f3759df - (i >> 1);               // what the fuck? 
		y = *(float*)&i;
		y = y * (threehalfs - (x2 * y * y));   // 1st iteration
	//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

		return y;
	}

	float x, y;
	Vec2D()
	{
		x = 0;
		y = 0;
	}

	Vec2D(float _x, float _y)
	{
		x = _x;
		y = _y;
	}

	Vec2D(const Vec2D& p)
	{
		x = p.x;
		y = p.y;
	}

	inline void set(const Vec2D& p)
	{
		x = p.x;
		y = p.y;
	}

	inline void set(const float& _x, const float& _y)
	{
		x = _x;
		y = _y;
	}

	bool intersect(const SDL_Rect& rect)
	{
		if (rect.x > x)
			return false;
		if (rect.x + rect.w < x)
			return false;
		if (rect.y > y)
			return false;
		if (rect.y + rect.h < y)
			return false;

		return true;
	}


	inline void add(const float& _x, const float& _y)
	{
		x += _x;
		y += _y;
	}

	inline float magnitude()
	{
		return (1/Q_rsqrt((x * x) + (y * y)));//sqrtf((x * x) + (y * y));
	}
	inline Vec2D normalize()
	{
		float r = Q_rsqrt((x * x) + (y * y));
		return Vec2D(x * r, y * r);
	}
	inline Vec2D perpendicular()
	{
		return Vec2D(-y, x);
	}

	inline Vec2D operator +(const Vec2D& p)
	{
		return Vec2D(x + p.x, y + p.y);
	}
	inline Vec2D operator -(const Vec2D& p)
	{
		return Vec2D(x - p.x, y - p.y);
	}
	inline Vec2D operator *(const float& n)
	{
		return Vec2D(x * n, y * n);
	}
	inline Vec2D operator /(const float& n)
	{
		return Vec2D(x / n, y / n);
	}
	inline float operator [](const int i)
	{
		if (i == 1)
			return y;
		else
			return x;
	}

	inline Vec2D& operator +=(const Vec2D& p)
	{
		this->x += p.x;
		this->y += p.y;
		return *this;
	}
	inline Vec2D& operator -=(const Vec2D& p)
	{
		this->x -= p.x;
		this->y -= p.y;
		return *this;
	}
	inline Vec2D& operator *=(const float& n)
	{
		this->x *= n;
		this->y *= n;
		return *this;
	}
	inline Vec2D& operator /=(const float& n)
	{
		this->x /= n;
		this->y /= n;
		return *this;
	}

	inline static float getDistance(float x, float y, float x2, float y2)
	{
		return (1 / Q_rsqrt((x2 - x) * (x2 - x) + (y2 - y) * (y2 - y)));
	}

	inline static float getDistance(Vec2D& p1, Vec2D& p2)
	{
		return getDistance(p1.x, p1.y, p2.x, p2.y);
	}

	inline float getDistance(Vec2D& p)
	{
		return getDistance(*this, p);
	}

	inline static float getDirection(float x, float y, float x2, float y2)
	{
		float d = getDistance(x, y, x2, y2);
		if (d == 0) d = 1;
		float diff_y = ((y2 - y) / d);
		float diff_x = ((x2 - x) / d);
		float _asin = asinf(diff_y) * 180.0f / F_PI;

		if (diff_x > 0)
			return (90.0f + _asin);
		else
			return (270.0f - _asin);
	}

	inline static float getDirection(Vec2D& p1, Vec2D& p2)
	{
		return getDirection(p1.x, p1.y, p2.x, p2.y);
	}

	inline float getDirection(Vec2D& p)
	{
		return getDirection(*this, p);
	}

	inline float getAngle()
	{
		return getDirection(0, 0, x, y);
	}

	void render(SDL_Renderer* renderer)
	{
		SDL_RenderDrawPoint(renderer, (int)x, (int)y);
	}

	void render(SDL_Renderer* renderer, const Vec2D& camera)
	{
		SDL_RenderDrawPoint(renderer, (int)(x - camera.x), (int)(y - camera.y));
	}
};

#endif
