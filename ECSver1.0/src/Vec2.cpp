#include "Vec2.h"
#include <cmath>

Vec2::Vec2(float _x, float _y) : x(_x), y(_y)
{

}

bool Vec2::operator==(const Vec2& rhs) const
{
	return this->x == rhs.x && this->y == rhs.y;
}

bool Vec2::operator!=(const Vec2& rhs) const
{
	return this->x != rhs.x || this->y != rhs.y;
}

Vec2 Vec2::operator+(const Vec2& rhs) const
{
	return Vec2(this->x+rhs.x,this->y+rhs.y);
}

Vec2 Vec2::operator-(const Vec2& rhs) const
{
	return Vec2(this->x - rhs.x, this->y - rhs.y);
}

Vec2 Vec2::operator*(const float val) const
{
	return Vec2(this->x * val, this->y * val);
}

Vec2 Vec2::operator/(const float val) const
{
	return Vec2(this->x / val, this->y / val);
}

void Vec2::operator+=(const Vec2& rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
}

void Vec2::operator-=(const Vec2& rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
}

void Vec2::operator*=(const float val)
{
	this->x *= val;
	this->y *= val;
}

void Vec2::operator/=(const float val)
{
	this->x /= val;
	this->y /= val;
}


float Vec2::length() const
{
	return sqrt(this->x*this->x + this->y*this->y);
}

Vec2 Vec2::normalized() const
{
	return *this / this->length();
}

Vec2 Vec2::distance(const Vec2& rhs) const {
	return rhs - *this;
}
