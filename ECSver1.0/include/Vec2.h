#pragma once

class Vec2 {
public:
	float x = 0;
	float y = 0;

	Vec2() = default;
	Vec2(float _x, float _y);

	bool operator == (const Vec2& rhs) const;
	bool operator != (const Vec2& rhs) const;

	Vec2 operator + (const Vec2& rhs) const;
	Vec2 operator - (const Vec2& rhs) const;
	Vec2 operator * (const float val) const;
	Vec2 operator / (const float val) const;

	void operator += (const Vec2& rhs);
	void operator -= (const Vec2& rhs);
	void operator *= (const float val);
	void operator /= (const float val);

	float length() const;
	Vec2 normalized() const;
	Vec2 distance(const Vec2& rhs) const;
};